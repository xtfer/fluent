// ast.h
// Fluent Language AST Header File

#ifndef AST_H
#define AST_H

#include "lexer.h"

typedef enum {
    AST_PROGRAM,
    AST_BLOCK,
    AST_VAR_DECL,
    AST_ASSIGNMENT,
    AST_FUNC_DECL,
    AST_IF_STMT,
    AST_WHILE_STMT,
    AST_FOR_STMT,
    AST_RETURN_STMT,
    AST_BIN_OP,
    AST_NUMBER,
    AST_IDENTIFIER,
    AST_NOOP
    // Add other AST node types as needed
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    char* value;                  // For identifiers and literals
    struct ASTNode* left;
    struct ASTNode* right;
    struct ASTNode* expr;         // For variable declarations and assignments
    char* var_name;               // Variable name
    int is_mutable;               // 1 for 'var', 0 for 'let'
    TokenType op;                 // Operator for binary operations
    struct ASTNode* next;         // For linked list of statements
    struct ASTNode* statements;   // For blocks and programs
    struct ASTNode* condition;    // For control flow
    struct ASTNode* then_branch;  // For 'if' statements
    struct ASTNode* else_branch;  // For 'if' statements
    char* func_name;              // Function name
    struct ASTNode* params;       // Function parameters
    struct ASTNode* body;         // Function body
} ASTNode;

// Function prototypes
ASTNode* create_ast_node(ASTNodeType type);
void free_ast(ASTNode* node);

#endif // AST_H
