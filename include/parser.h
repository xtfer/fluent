// parser.h
// Fluent Language Parser Header File

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

// Function prototypes
ASTNode* parse_program(void);

#endif // PARSER_H