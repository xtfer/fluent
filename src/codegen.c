// codegen.c
// Implementation of the Fluent language code generator

#include "codegen.h"
#include <stdio.h>
#include <string.h>

// Function prototypes
void generate_function(ASTNode* node); // Added function prototype
void generate_statement(ASTNode* node);
void generate_expression(ASTNode* node);
void generate_block(ASTNode* node);

void generate_code(ASTNode* ast) {
    printf("#include <stdio.h>\n\n");

    // Generate code for function declarations
    ASTNode* stmt = ast->statements;
    while (stmt) {
        if (stmt->type == AST_FUNC_DECL) {
            generate_function(stmt);
        } else {
            // Global code (e.g., variable declarations)
            generate_statement(stmt);
        }
        stmt = stmt->next;
    }

    // If no 'main' function, create an empty main
    printf("int main() {\n");
    printf("    // Call the main function if it exists\n");
    printf("    if (main) main();\n");
    printf("    return 0;\n");
    printf("}\n");
}

void generate_function(ASTNode* node) {
    printf("void %s() {\n", node->func_name);
    // Generate function body
    generate_block(node->body);
    printf("}\n");
}

void generate_block(ASTNode* node) {
    ASTNode* stmt = node->statements;
    while (stmt) {
        generate_statement(stmt);
        stmt = stmt->next;
    }
}

void generate_statement(ASTNode* node) {
    switch (node->type) {
        case AST_VAR_DECL:
            if (node->is_mutable) {
                printf("    int %s = ", node->var_name);
            } else {
                printf("    const int %s = ", node->var_name);
            }
            generate_expression(node->expr);
            printf(";\n");
            break;
        case AST_ASSIGNMENT:
            printf("    %s = ", node->var_name);
            generate_expression(node->expr);
            printf(";\n");
            break;
        case AST_RETURN_STMT:
            printf("    return ");
            generate_expression(node->expr);
            printf(";\n");
            break;
        case AST_IF_STMT:
            printf("    if (");
            generate_expression(node->condition);
            printf(") {\n");
            generate_block(node->then_branch);
            printf("    }");
            if (node->else_branch) {
                printf(" else {\n");
                generate_block(node->else_branch);
                printf("    }");
            }
            printf("\n");
            break;
        case AST_WHILE_STMT:
            printf("    while (");
            generate_expression(node->condition);
            printf(") {\n");
            generate_block(node->body);
            printf("    }\n");
            break;
        case AST_BIN_OP:
        case AST_NUMBER:
        case AST_IDENTIFIER:
            printf("    ");
            generate_expression(node);
            printf(";\n");
            break;
        default:
            // No operation
            break;
    }
}

void generate_expression(ASTNode* node) {
    switch (node->type) {
        case AST_NUMBER:
            printf("%s", node->value);
            break;
        case AST_IDENTIFIER:
            printf("%s", node->value);
            break;
        case AST_BIN_OP:
            printf("(");
            generate_expression(node->left);
            switch (node->op) {
                case TOKEN_PLUS:
                    printf(" + ");
                    break;
                case TOKEN_MINUS:
                    printf(" - ");
                    break;
                case TOKEN_ASTERISK:
                    printf(" * ");
                    break;
                case TOKEN_SLASH:
                    printf(" / ");
                    break;
                case TOKEN_EQUAL:
                    printf(" == ");
                    break;
                case TOKEN_NOT_EQUAL:
                    printf(" != ");
                    break;
                case TOKEN_LESS:
                    printf(" < ");
                    break;
                case TOKEN_GREATER:
                    printf(" > ");
                    break;
                case TOKEN_LESS_EQUAL:
                    printf(" <= ");
                    break;
                case TOKEN_GREATER_EQUAL:
                    printf(" >= ");
                    break;
                default:
                    break;
            }
            generate_expression(node->right);
            printf(")");
            break;
        default:
            break;
    }
}
