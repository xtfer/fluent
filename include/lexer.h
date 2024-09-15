// lexer.h
// Fluent Language Lexer Header File

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

typedef enum {
    // Single-character tokens
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_ASTERISK,
    TOKEN_SLASH,
    TOKEN_ASSIGN,
    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_LESS,
    TOKEN_GREATER,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER_EQUAL,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_COLON,
    TOKEN_COMMA,

    // Keywords
    TOKEN_FUNC,
    TOKEN_LET,
    TOKEN_VAR,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_ELIF,
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_RETURN,

    // Literals
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,

    // Special tokens
    TOKEN_EOF,
    TOKEN_INDENT,
    TOKEN_DEDENT,
    TOKEN_NEWLINE,

    // Others
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char* value;
    int line;
    int column;
} Token;

// Function prototypes
void init_lexer(const char* source_code);
Token* get_next_token(void);
void free_token(Token* token);

#endif // LEXER_H