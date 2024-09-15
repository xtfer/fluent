// ast.c
// Implementation of the AST functions for Fluent language

#include "ast.h"
#include <stdlib.h>
#include <string.h>

ASTNode* create_ast_node(ASTNodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->value = NULL;
    node->left = NULL;
    node->right = NULL;
    node->expr = NULL;
    node->var_name = NULL;
    node->is_mutable = 0;
    node->op = TOKEN_UNKNOWN;
    node->next = NULL;
    node->statements = NULL;
    node->condition = NULL;
    node->then_branch = NULL;
    node->else_branch = NULL;
    node->func_name = NULL;
    node->params = NULL;
    node->body = NULL;
    return node;
}

void free_ast(ASTNode* node) {
    if (!node) return;

    if (node->value) free(node->value);
    if (node->var_name) free(node->var_name);
    if (node->func_name) free(node->func_name);

    if (node->left) free_ast(node->left);
    if (node->right) free_ast(node->right);
    if (node->expr) free_ast(node->expr);
    if (node->next) free_ast(node->next);
    if (node->statements) free_ast(node->statements);
    if (node->condition) free_ast(node->condition);
    if (node->then_branch) free_ast(node->then_branch);
    if (node->else_branch) free_ast(node->else_branch);
    if (node->body) free_ast(node->body);

    free(node);
}
