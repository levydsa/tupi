#include "tree_sitter/parser.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

enum TokenType {
    INDENT,
    LIST_INDENT,
};

typedef struct {
    uint8_t current_indent;
} Scanner;

bool tree_sitter_tupi_external_scanner_scan(
    void *payload, TSLexer *lexer, const bool *valid_symbols
) {
    Scanner *scanner = (Scanner *)payload;
    uint8_t space = 0;

    if (valid_symbols[INDENT] || valid_symbols[LIST_INDENT]) {
        while (!lexer->eof(lexer)) {
            if (lexer->lookahead == ' ' || lexer->lookahead == '\t') {
                printf("- %d %p\n", space, scanner);
                lexer->advance(lexer, true);
                space++;
            } else {
                printf("* %d %p\n", space, scanner);
                goto non_space;
            }
        }

        if (lexer->eof(lexer) && space == 0) {
            return false;
        }

non_space:
        if (space < scanner->current_indent) {
            return false;
        }

        lexer->mark_end(lexer);

        if (valid_symbols[INDENT]) {
            lexer->result_symbol = INDENT;
        } else if (valid_symbols[LIST_INDENT]) {
            scanner->current_indent++;
            lexer->result_symbol = LIST_INDENT;
        }

        return true;
    }

    return false;
}

unsigned
tree_sitter_tupi_external_scanner_serialize(void *payload, char *buffer) {
    Scanner *scanner = (Scanner *)payload;
    size_t size = 0;

    buffer[size++] = (char)scanner->current_indent;

    return size;
}

void tree_sitter_tupi_external_scanner_deserialize(
    void *payload, const char *buffer, unsigned length
) {
    Scanner *scanner = (Scanner *)payload;
    size_t size = 0;

    if (length > 0) {
        scanner->current_indent = (uint8_t)buffer[size++];
        assert(size <= length);
    }
}

void *tree_sitter_tupi_external_scanner_create() {
    Scanner *scanner = calloc(1, sizeof(Scanner));
    tree_sitter_tupi_external_scanner_deserialize(scanner, NULL, 0);
    return scanner;
}

void tree_sitter_tupi_external_scanner_destroy(void *payload) {
    Scanner *scanner = (Scanner *)payload;
    free(scanner);
}
