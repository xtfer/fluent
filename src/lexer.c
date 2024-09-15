// lexer.c
// Implementation of the Fluent language lexer

#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static const char* src;
static int pos = 0;
static int line = 1;
static int column = 1;

#define MAX_INDENT_LEVELS 100
static int indent_levels[MAX_INDENT_LEVELS];
static int indent_level = 0;
static int indent_stack_top = 0;

void init_lexer(const char* source_code) {
    src = source_code;
    pos = 0;
    line = 1;
    column = 1;
    indent_levels[0] = 0;
    indent_level = 0;
    indent_stack_top = 0;
}

static char peek() {
    return src[pos];
}

static char advance() {
    char c = src[pos++];
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

static void skip_whitespace() {
    while (peek() == ' ' || peek() == '\t') {
        advance();
    }
}

static void skip_comment() {
    while (peek() != '\n' && peek() != '\0') {
        advance();
    }
}

Token* get_next_token(void) {
    static int at_line_start = 1;

    if (peek() == '\0') {
        // Handle remaining dedents
        if (indent_stack_top > 0) {
            indent_stack_top--;
            return &(Token){TOKEN_DEDENT, strdup("<DEDENT>"), line, column};
        }
        return &(Token){TOKEN_EOF, strdup("<EOF>"), line, column};
    }

    if (at_line_start) {
        at_line_start = 0;
        int spaces = 0;
        while (peek() == ' ') {
            advance();
            spaces++;
        }
        if (peek() == '\n' || peek() == '\0') {
            // Empty line
            return get_next_token();
        }
        if (spaces > indent_levels[indent_stack_top]) {
            indent_stack_top++;
            if (indent_stack_top >= MAX_INDENT_LEVELS) {
                fprintf(stderr, "Too many indentation levels\n");
                exit(1);
            }
            indent_levels[indent_stack_top] = spaces;
            return &(Token){TOKEN_INDENT, strdup("<INDENT>"), line, column};
        } else {
            while (spaces < indent_levels[indent_stack_top]) {
                indent_stack_top--;
                return &(Token){TOKEN_DEDENT, strdup("<DEDENT>"), line, column};
            }
        }
    }

    skip_whitespace();

    char c = peek();

    if (c == '\n') {
        advance();
        at_line_start = 1;
        return &(Token){TOKEN_NEWLINE, strdup("<NEWLINE>"), line - 1, column};
    }

    if (c == '#') {
        advance();
        skip_comment();
        return get_next_token();
    }

    if (isalpha(c) || c == '_') {
        // Handle identifiers and keywords
        int start_pos = pos;
        int start_column = column;
        while (isalnum(peek()) || peek() == '_') {
            advance();
        }
        int length = pos - start_pos;
        char* text = strndup(&src[start_pos], length);

        TokenType type = TOKEN_IDENTIFIER;
        if (strcmp(text, "func") == 0) type = TOKEN_FUNC;
        else if (strcmp(text, "let") == 0) type = TOKEN_LET;
        else if (strcmp(text, "var") == 0) type = TOKEN_VAR;
        else if (strcmp(text, "if") == 0) type = TOKEN_IF;
        else if (strcmp(text, "else") == 0) type = TOKEN_ELSE;
        else if (strcmp(text, "elif") == 0) type = TOKEN_ELIF;
        else if (strcmp(text, "for") == 0) type = TOKEN_FOR;
        else if (strcmp(text, "while") == 0) type = TOKEN_WHILE;
        else if (strcmp(text, "return") == 0) type = TOKEN_RETURN;

        Token* token = malloc(sizeof(Token));
        token->type = type;
        token->value = text;
        token->line = line;
        token->column = start_column;
        return token;
    }

    // Handle numbers
    if (isdigit(c)) {
        int start_pos = pos;
        int start_column = column;
        while (isdigit(peek())) {
            advance();
        }
        // Handle decimal point
        if (peek() == '.') {
            advance();
            while (isdigit(peek())) {
                advance();
            }
        }
        int length = pos - start_pos;
        char* text = strndup(&src[start_pos], length);

        Token* token = malloc(sizeof(Token));
        token->type = TOKEN_NUMBER;
        token->value = text;
        token->line = line;
        token->column = start_column;
        return token;
    }

    // Handle strings
    if (c == '"' || c == '\'') {
        char quote = advance(); // Consume the opening quote
        int start_pos = pos;
        int start_column = column;
        while (peek() != quote && peek() != '\0') {
            advance();
        }
        if (peek() == '\0') {
            fprintf(stderr, "Unterminated string at line %d, column %d\n", line, column);
            exit(1);
        }
        advance(); // Consume closing quote
        int length = pos - start_pos - 1;
        char* text = strndup(&src[start_pos], length);

        Token* token = malloc(sizeof(Token));
        token->type = TOKEN_STRING;
        token->value = text;
        token->line = line;
        token->column = start_column;
        return token;
    }

    // Handle operators and punctuation
    switch (c) {
        case '+':
            advance();
            return &(Token){TOKEN_PLUS, strdup("+"), line, column - 1};
        case '-':
            advance();
            return &(Token){TOKEN_MINUS, strdup("-"), line, column - 1};
        case '*':
            advance();
            return &(Token){TOKEN_ASTERISK, strdup("*"), line, column - 1};
        case '/':
            advance();
            return &(Token){TOKEN_SLASH, strdup("/"), line, column - 1};
        case '(':
            advance();
            return &(Token){TOKEN_LPAREN, strdup("("), line, column - 1};
        case ')':
            advance();
            return &(Token){TOKEN_RPAREN, strdup(")"), line, column - 1};
        case ':':
            advance();
            return &(Token){TOKEN_COLON, strdup(":"), line, column - 1};
        case ',':
            advance();
            return &(Token){TOKEN_COMMA, strdup(","), line, column - 1};
        case '=':
            advance();
            if (peek() == '=') {
                advance();
                return &(Token){TOKEN_EQUAL, strdup("=="), line, column - 2};
            } else {
                return &(Token){TOKEN_ASSIGN, strdup("="), line, column - 1};
            }
        case '!':
            advance();
            if (peek() == '=') {
                advance();
                return &(Token){TOKEN_NOT_EQUAL, strdup("!="), line, column - 2};
            } else {
                fprintf(stderr, "Unexpected character '!' at line %d, column %d\n", line, column);
                exit(1);
            }
        case '<':
            advance();
            if (peek() == '=') {
                advance();
                return &(Token){TOKEN_LESS_EQUAL, strdup("<="), line, column - 2};
            } else {
                return &(Token){TOKEN_LESS, strdup("<"), line, column - 1};
            }
        case '>':
            advance();
            if (peek() == '=') {
                advance();
                return &(Token){TOKEN_GREATER_EQUAL, strdup(">="), line, column - 2};
            } else {
                return &(Token){TOKEN_GREATER, strdup(">"), line, column - 1};
            }
        default:
            fprintf(stderr, "Unknown character '%c' at line %d, column %d\n", c, line, column);
            exit(1);
    }
}

void free_token(Token* token) {
    if (token) {
        free(token->value);
        free(token);
    }
}
