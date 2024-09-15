// main.c
// Fluent Compiler Main File

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include "ast.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s source.flu\n", argv[0]);
        return 1;
    }

    // Read source code from file
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        perror("Could not open source file");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    rewind(file);
    char* source_code = malloc(fsize + 1);
    fread(source_code, 1, fsize, file);
    fclose(file);
    source_code[fsize] = '\0';

    // Initialize lexer and parse the source code
    init_lexer(source_code);
    ASTNode* ast = parse_program();

    // Generate code
    generate_code(ast);

    // Clean up
    free_ast(ast);
    free(source_code);
    return 0;
}
