// parser.c
// Implementation of the Fluent language parser

#include "parser.h"
#include "lexer.h"
#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static Token* current_token;

static void advance_token(void);
static ASTNode* parse_statement(void);
static ASTNode* parse_expression(void);
static ASTNode* parse_term(void);
static ASTNode* parse_factor(void);
static ASTNode* parse_block(void);
static ASTNode* parse_variable_declaration(void);
static ASTNode* parse_assignment_or_function_call(void);
static ASTNode* parse_function_declaration(void);
static ASTNode* parse_if_statement(void);
static ASTNode* parse_while_statement(void);
static ASTNode* parse_for_statement(void);

ASTNode* parse_program(void) {
    advance_token();
    ASTNode* program = create_ast_node(AST_PROGRAM);
    program->statements = NULL; // Initialize statements list

    ASTNode* last_stmt = NULL;

    while (current_token->type != TOKEN_EOF) {
        ASTNode* stmt = parse_statement();
        if (stmt) {
            if (last_stmt == NULL) {
                program->statements = stmt;
            } else {
                last_stmt->next = stmt;
            }
            last_stmt = stmt;
        }
    }

    return program;
}

static void advance_token(void) {
    if (current_token) {
        free_token(current_token);
    }
    current_token = get_next_token();
}

static ASTNode* parse_statement(void) {
    if (current_token->type == TOKEN_LET || current_token->type == TOKEN_VAR) {
        return parse_variable_declaration();
    } else if (current_token->type == TOKEN_IDENTIFIER) {
        return parse_assignment_or_function_call();
    } else if (current_token->type == TOKEN_FUNC) {
        return parse_function_declaration();
    } else if (current_token->type == TOKEN_IF) {
        return parse_if_statement();
    } else if (current_token->type == TOKEN_WHILE) {
        return parse_while_statement();
    } else if (current_token->type == TOKEN_FOR) {
        return parse_for_statement();
    } else if (current_token->type == TOKEN_RETURN) {
        advance_token(); // Consume 'return'
        ASTNode* expr = parse_expression();
        ASTNode* return_stmt = create_ast_node(AST_RETURN_STMT);
        return_stmt->expr = expr;
        return return_stmt;
    } else if (current_token->type == TOKEN_NEWLINE) {
        advance_token(); // Consume newline
        return parse_statement();
    } else if (current_token->type == TOKEN_EOF) {
        return NULL;
    } else {
        // Expression as statement
        ASTNode* expr = parse_expression();
        return expr;
    }
}

static ASTNode* parse_variable_declaration(void) {
    TokenType var_type = current_token->type; // TOKEN_LET or TOKEN_VAR
    advance_token(); // Consume 'let' or 'var'

    if (current_token->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Expected identifier after 'let' or 'var'\n");
        exit(1);
    }

    char* var_name = strdup(current_token->value);
    advance_token(); // Consume identifier

    if (current_token->type != TOKEN_ASSIGN) {
        fprintf(stderr, "Expected '=' after variable name\n");
        exit(1);
    }

    advance_token(); // Consume '='

    ASTNode* expr = parse_expression();

    ASTNode* var_decl = create_ast_node(AST_VAR_DECL);
    var_decl->var_name = var_name;
    var_decl->expr = expr;
    var_decl->is_mutable = (var_type == TOKEN_VAR);

    if (current_token->type == TOKEN_NEWLINE) {
        advance_token(); // Consume newline
    }

    return var_decl;
}

static ASTNode* parse_assignment_or_function_call(void) {
    char* identifier = strdup(current_token->value);
    advance_token(); // Consume identifier

    if (current_token->type == TOKEN_ASSIGN) {
        // Assignment
        advance_token(); // Consume '='
        ASTNode* expr = parse_expression();

        ASTNode* assignment = create_ast_node(AST_ASSIGNMENT);
        assignment->var_name = identifier;
        assignment->expr = expr;

        if (current_token->type == TOKEN_NEWLINE) {
            advance_token(); // Consume newline
        }

        return assignment;
    } else if (current_token->type == TOKEN_LPAREN) {
        // Function call (not implemented yet)
        fprintf(stderr, "Function calls not implemented\n");
        exit(1);
    } else {
        fprintf(stderr, "Unexpected token after identifier\n");
        exit(1);
    }
}

static ASTNode* parse_expression(void) {
    ASTNode* node = parse_term();

    while (current_token->type == TOKEN_PLUS || current_token->type == TOKEN_MINUS) {
        TokenType op = current_token->type;
        advance_token(); // Consume '+' or '-'

        ASTNode* right = parse_term();

        ASTNode* bin_op = create_ast_node(AST_BIN_OP);
        bin_op->left = node;
        bin_op->right = right;
        bin_op->op = op; // Store the operator

        node = bin_op;
    }

    return node;
}

static ASTNode* parse_term(void) {
    ASTNode* node = parse_factor();

    while (current_token->type == TOKEN_ASTERISK || current_token->type == TOKEN_SLASH) {
        TokenType op = current_token->type;
        advance_token(); // Consume '*' or '/'

        ASTNode* right = parse_factor();

        ASTNode* bin_op = create_ast_node(AST_BIN_OP);
        bin_op->left = node;
        bin_op->right = right;
        bin_op->op = op; // Store the operator

        node = bin_op;
    }

    return node;
}

static ASTNode* parse_factor(void) {
    ASTNode* node = NULL;

    if (current_token->type == TOKEN_NUMBER) {
        node = create_ast_node(AST_NUMBER);
        node->value = strdup(current_token->value);
        advance_token(); // Consume number
    } else if (current_token->type == TOKEN_IDENTIFIER) {
        node = create_ast_node(AST_IDENTIFIER);
        node->value = strdup(current_token->value);
        advance_token(); // Consume identifier
    } else if (current_token->type == TOKEN_LPAREN) {
        advance_token(); // Consume '('
        node = parse_expression();
        if (current_token->type != TOKEN_RPAREN) {
            fprintf(stderr, "Expected ')' after expression\n");
            exit(1);
        }
        advance_token(); // Consume ')'
    } else {
        fprintf(stderr, "Unexpected token in factor\n");
        exit(1);
    }

    return node;
}

static ASTNode* parse_function_declaration(void) {
    advance_token(); // Consume 'func'

    if (current_token->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Expected function name after 'func'\n");
        exit(1);
    }

    char* func_name = strdup(current_token->value);
    advance_token(); // Consume function name

    // Parameters (not implemented yet)
    if (current_token->type == TOKEN_LPAREN) {
        fprintf(stderr, "Function parameters not implemented\n");
        exit(1);
    }

    if (current_token->type != TOKEN_COLON) {
        fprintf(stderr, "Expected ':' after function name\n");
        exit(1);
    }

    advance_token(); // Consume ':'

    ASTNode* body = parse_block();

    ASTNode* func_decl = create_ast_node(AST_FUNC_DECL);
    func_decl->func_name = func_name;
    func_decl->body = body;

    return func_decl;
}

static ASTNode* parse_block(void) {
    if (current_token->type != TOKEN_INDENT) {
        fprintf(stderr, "Expected indentation\n");
        exit(1);
    }

    advance_token(); // Consume TOKEN_INDENT

    ASTNode* block = create_ast_node(AST_BLOCK);
    block->statements = NULL;

    ASTNode* last_stmt = NULL;

    while (current_token->type != TOKEN_DEDENT && current_token->type != TOKEN_EOF) {
        if (current_token->type == TOKEN_NEWLINE) {
            advance_token(); // Consume newline
            continue;
        }

        ASTNode* stmt = parse_statement();
        if (stmt) {
            if (last_stmt == NULL) {
                block->statements = stmt;
            } else {
                last_stmt->next = stmt;
            }
            last_stmt = stmt;
        }
    }

    if (current_token->type == TOKEN_DEDENT) {
        advance_token(); // Consume TOKEN_DEDENT
    } else {
        fprintf(stderr, "Expected dedentation\n");
        exit(1);
    }

    return block;
}

static ASTNode* parse_if_statement(void) {
    advance_token(); // Consume 'if'

    ASTNode* condition = parse_expression();

    if (current_token->type != TOKEN_COLON) {
        fprintf(stderr, "Expected ':' after if condition\n");
        exit(1);
    }
    advance_token(); // Consume ':'

    ASTNode* then_block = parse_block();

    ASTNode* if_stmt = create_ast_node(AST_IF_STMT);
    if_stmt->condition = condition;
    if_stmt->then_branch = then_block;
    if_stmt->else_branch = NULL;

    // Handle 'else' or 'elif'
    if (current_token->type == TOKEN_ELSE) {
        advance_token(); // Consume 'else'
        if (current_token->type != TOKEN_COLON) {
            fprintf(stderr, "Expected ':' after 'else'\n");
            exit(1);
        }
        advance_token(); // Consume ':'
        ASTNode* else_block = parse_block();
        if_stmt->else_branch = else_block;
    }

    return if_stmt;
}

static ASTNode* parse_while_statement(void) {
    advance_token(); // Consume 'while'

    ASTNode* condition = parse_expression();

    if (current_token->type != TOKEN_COLON) {
        fprintf(stderr, "Expected ':' after while condition\n");
        exit(1);
    }
    advance_token(); // Consume ':'

    ASTNode* body = parse_block();

    ASTNode* while_stmt = create_ast_node(AST_WHILE_STMT);
    while_stmt->condition = condition;
    while_stmt->body = body;

    return while_stmt;
}

static ASTNode* parse_for_statement(void) {
    fprintf(stderr, "'for' loops not implemented yet\n");
    exit(1);
}

void free_parser(void) {
    if (current_token) {
        free_token(current_token);
    }
}
