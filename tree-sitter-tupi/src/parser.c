#include "tree_sitter/parser.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#define LANGUAGE_VERSION 14
#define STATE_COUNT 16
#define LARGE_STATE_COUNT 2
#define SYMBOL_COUNT 16
#define ALIAS_COUNT 0
#define TOKEN_COUNT 9
#define EXTERNAL_TOKEN_COUNT 2
#define FIELD_COUNT 0
#define MAX_ALIAS_SEQUENCE_LENGTH 5
#define PRODUCTION_ID_COUNT 1

enum ts_symbol_identifiers {
  sym_space = 1,
  sym_word = 2,
  sym_math = 3,
  sym_symbol = 4,
  sym_nl = 5,
  anon_sym_DASH = 6,
  sym_indent = 7,
  sym_list_indent = 8,
  sym_source_file = 9,
  sym_line = 10,
  sym_list = 11,
  sym__inline = 12,
  aux_sym_source_file_repeat1 = 13,
  aux_sym_line_repeat1 = 14,
  aux_sym_list_repeat1 = 15,
};

static const char * const ts_symbol_names[] = {
  [ts_builtin_sym_end] = "end",
  [sym_space] = "space",
  [sym_word] = "word",
  [sym_math] = "math",
  [sym_symbol] = "symbol",
  [sym_nl] = "nl",
  [anon_sym_DASH] = "-",
  [sym_indent] = "indent",
  [sym_list_indent] = "list_indent",
  [sym_source_file] = "source_file",
  [sym_line] = "line",
  [sym_list] = "list",
  [sym__inline] = "_inline",
  [aux_sym_source_file_repeat1] = "source_file_repeat1",
  [aux_sym_line_repeat1] = "line_repeat1",
  [aux_sym_list_repeat1] = "list_repeat1",
};

static const TSSymbol ts_symbol_map[] = {
  [ts_builtin_sym_end] = ts_builtin_sym_end,
  [sym_space] = sym_space,
  [sym_word] = sym_word,
  [sym_math] = sym_math,
  [sym_symbol] = sym_symbol,
  [sym_nl] = sym_nl,
  [anon_sym_DASH] = anon_sym_DASH,
  [sym_indent] = sym_indent,
  [sym_list_indent] = sym_list_indent,
  [sym_source_file] = sym_source_file,
  [sym_line] = sym_line,
  [sym_list] = sym_list,
  [sym__inline] = sym__inline,
  [aux_sym_source_file_repeat1] = aux_sym_source_file_repeat1,
  [aux_sym_line_repeat1] = aux_sym_line_repeat1,
  [aux_sym_list_repeat1] = aux_sym_list_repeat1,
};

static const TSSymbolMetadata ts_symbol_metadata[] = {
  [ts_builtin_sym_end] = {
    .visible = false,
    .named = true,
  },
  [sym_space] = {
    .visible = true,
    .named = true,
  },
  [sym_word] = {
    .visible = true,
    .named = true,
  },
  [sym_math] = {
    .visible = true,
    .named = true,
  },
  [sym_symbol] = {
    .visible = true,
    .named = true,
  },
  [sym_nl] = {
    .visible = true,
    .named = true,
  },
  [anon_sym_DASH] = {
    .visible = true,
    .named = false,
  },
  [sym_indent] = {
    .visible = true,
    .named = true,
  },
  [sym_list_indent] = {
    .visible = true,
    .named = true,
  },
  [sym_source_file] = {
    .visible = true,
    .named = true,
  },
  [sym_line] = {
    .visible = true,
    .named = true,
  },
  [sym_list] = {
    .visible = true,
    .named = true,
  },
  [sym__inline] = {
    .visible = false,
    .named = true,
  },
  [aux_sym_source_file_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_line_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_list_repeat1] = {
    .visible = false,
    .named = false,
  },
};

static const TSSymbol ts_alias_sequences[PRODUCTION_ID_COUNT][MAX_ALIAS_SEQUENCE_LENGTH] = {
  [0] = {0},
};

static const uint16_t ts_non_terminal_alias_map[] = {
  0,
};

static const TSStateId ts_primary_state_ids[STATE_COUNT] = {
  [0] = 0,
  [1] = 1,
  [2] = 2,
  [3] = 3,
  [4] = 4,
  [5] = 5,
  [6] = 6,
  [7] = 7,
  [8] = 8,
  [9] = 9,
  [10] = 10,
  [11] = 11,
  [12] = 12,
  [13] = 13,
  [14] = 14,
  [15] = 15,
};

static bool ts_lex(TSLexer *lexer, TSStateId state) {
  START_LEXER();
  eof = lexer->eof(lexer);
  switch (state) {
    case 0:
      if (eof) ADVANCE(4);
      if (lookahead == '$') ADVANCE(2);
      if (lookahead == '-') ADVANCE(11);
      if (('\t' <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') SKIP(3);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(6);
      END_STATE();
    case 1:
      if (lookahead == '\n') ADVANCE(10);
      if (lookahead == ' ') ADVANCE(5);
      if (lookahead == '$') ADVANCE(2);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(7);
      if (lookahead != 0) ADVANCE(9);
      END_STATE();
    case 2:
      if (lookahead == '$') ADVANCE(8);
      if (lookahead != 0) ADVANCE(2);
      END_STATE();
    case 3:
      if (eof) ADVANCE(4);
      if (lookahead == '-') ADVANCE(11);
      if (('\t' <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') SKIP(3);
      END_STATE();
    case 4:
      ACCEPT_TOKEN(ts_builtin_sym_end);
      END_STATE();
    case 5:
      ACCEPT_TOKEN(sym_space);
      if (lookahead == ' ') ADVANCE(5);
      END_STATE();
    case 6:
      ACCEPT_TOKEN(sym_word);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(6);
      END_STATE();
    case 7:
      ACCEPT_TOKEN(sym_word);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(7);
      if (lookahead != 0 &&
          lookahead != '\n' &&
          lookahead != ' ' &&
          lookahead != '$') ADVANCE(9);
      END_STATE();
    case 8:
      ACCEPT_TOKEN(sym_math);
      END_STATE();
    case 9:
      ACCEPT_TOKEN(sym_symbol);
      if (lookahead != 0 &&
          lookahead != '\n' &&
          lookahead != ' ' &&
          lookahead != '$') ADVANCE(9);
      END_STATE();
    case 10:
      ACCEPT_TOKEN(sym_nl);
      END_STATE();
    case 11:
      ACCEPT_TOKEN(anon_sym_DASH);
      END_STATE();
    default:
      return false;
  }
}

static const TSLexMode ts_lex_modes[STATE_COUNT] = {
  [0] = {.lex_state = 0, .external_lex_state = 1},
  [1] = {.lex_state = 0, .external_lex_state = 2},
  [2] = {.lex_state = 1},
  [3] = {.lex_state = 1},
  [4] = {.lex_state = 1},
  [5] = {.lex_state = 1},
  [6] = {.lex_state = 0, .external_lex_state = 2},
  [7] = {.lex_state = 0, .external_lex_state = 2},
  [8] = {.lex_state = 0, .external_lex_state = 1},
  [9] = {.lex_state = 0, .external_lex_state = 1},
  [10] = {.lex_state = 0, .external_lex_state = 1},
  [11] = {.lex_state = 0, .external_lex_state = 1},
  [12] = {.lex_state = 0, .external_lex_state = 1},
  [13] = {.lex_state = 0},
  [14] = {.lex_state = 0},
  [15] = {.lex_state = 1},
};

static const uint16_t ts_parse_table[LARGE_STATE_COUNT][SYMBOL_COUNT] = {
  [0] = {
    [ts_builtin_sym_end] = ACTIONS(1),
    [sym_word] = ACTIONS(1),
    [sym_math] = ACTIONS(1),
    [anon_sym_DASH] = ACTIONS(1),
    [sym_indent] = ACTIONS(1),
    [sym_list_indent] = ACTIONS(1),
  },
  [1] = {
    [sym_source_file] = STATE(14),
    [sym_list] = STATE(6),
    [aux_sym_source_file_repeat1] = STATE(6),
    [ts_builtin_sym_end] = ACTIONS(3),
    [sym_list_indent] = ACTIONS(5),
  },
};

static const uint16_t ts_small_parse_table[] = {
  [0] = 4,
    STATE(8), 1,
      sym_line,
    ACTIONS(7), 2,
      sym_space,
      sym_math,
    ACTIONS(9), 2,
      sym_word,
      sym_symbol,
    STATE(3), 2,
      sym__inline,
      aux_sym_line_repeat1,
  [16] = 4,
    ACTIONS(15), 1,
      sym_nl,
    ACTIONS(11), 2,
      sym_space,
      sym_math,
    ACTIONS(13), 2,
      sym_word,
      sym_symbol,
    STATE(5), 2,
      sym__inline,
      aux_sym_line_repeat1,
  [32] = 4,
    STATE(12), 1,
      sym_line,
    ACTIONS(7), 2,
      sym_space,
      sym_math,
    ACTIONS(9), 2,
      sym_word,
      sym_symbol,
    STATE(3), 2,
      sym__inline,
      aux_sym_line_repeat1,
  [48] = 4,
    ACTIONS(23), 1,
      sym_nl,
    ACTIONS(17), 2,
      sym_space,
      sym_math,
    ACTIONS(20), 2,
      sym_word,
      sym_symbol,
    STATE(5), 2,
      sym__inline,
      aux_sym_line_repeat1,
  [64] = 3,
    ACTIONS(5), 1,
      sym_list_indent,
    ACTIONS(25), 1,
      ts_builtin_sym_end,
    STATE(7), 2,
      sym_list,
      aux_sym_source_file_repeat1,
  [75] = 3,
    ACTIONS(27), 1,
      ts_builtin_sym_end,
    ACTIONS(29), 1,
      sym_list_indent,
    STATE(7), 2,
      sym_list,
      aux_sym_source_file_repeat1,
  [86] = 3,
    ACTIONS(34), 1,
      sym_indent,
    STATE(9), 1,
      aux_sym_list_repeat1,
    ACTIONS(32), 2,
      sym_list_indent,
      ts_builtin_sym_end,
  [97] = 3,
    ACTIONS(34), 1,
      sym_indent,
    STATE(10), 1,
      aux_sym_list_repeat1,
    ACTIONS(36), 2,
      sym_list_indent,
      ts_builtin_sym_end,
  [108] = 3,
    ACTIONS(40), 1,
      sym_indent,
    STATE(10), 1,
      aux_sym_list_repeat1,
    ACTIONS(38), 2,
      sym_list_indent,
      ts_builtin_sym_end,
  [119] = 1,
    ACTIONS(43), 3,
      sym_indent,
      sym_list_indent,
      ts_builtin_sym_end,
  [125] = 1,
    ACTIONS(38), 3,
      sym_indent,
      sym_list_indent,
      ts_builtin_sym_end,
  [131] = 1,
    ACTIONS(45), 1,
      anon_sym_DASH,
  [135] = 1,
    ACTIONS(47), 1,
      ts_builtin_sym_end,
  [139] = 1,
    ACTIONS(49), 1,
      sym_space,
};

static const uint32_t ts_small_parse_table_map[] = {
  [SMALL_STATE(2)] = 0,
  [SMALL_STATE(3)] = 16,
  [SMALL_STATE(4)] = 32,
  [SMALL_STATE(5)] = 48,
  [SMALL_STATE(6)] = 64,
  [SMALL_STATE(7)] = 75,
  [SMALL_STATE(8)] = 86,
  [SMALL_STATE(9)] = 97,
  [SMALL_STATE(10)] = 108,
  [SMALL_STATE(11)] = 119,
  [SMALL_STATE(12)] = 125,
  [SMALL_STATE(13)] = 131,
  [SMALL_STATE(14)] = 135,
  [SMALL_STATE(15)] = 139,
};

static const TSParseActionEntry ts_parse_actions[] = {
  [0] = {.entry = {.count = 0, .reusable = false}},
  [1] = {.entry = {.count = 1, .reusable = false}}, RECOVER(),
  [3] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 0, 0, 0),
  [5] = {.entry = {.count = 1, .reusable = true}}, SHIFT(13),
  [7] = {.entry = {.count = 1, .reusable = true}}, SHIFT(3),
  [9] = {.entry = {.count = 1, .reusable = false}}, SHIFT(3),
  [11] = {.entry = {.count = 1, .reusable = true}}, SHIFT(5),
  [13] = {.entry = {.count = 1, .reusable = false}}, SHIFT(5),
  [15] = {.entry = {.count = 1, .reusable = true}}, SHIFT(11),
  [17] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_line_repeat1, 2, 0, 0), SHIFT_REPEAT(5),
  [20] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_line_repeat1, 2, 0, 0), SHIFT_REPEAT(5),
  [23] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_line_repeat1, 2, 0, 0),
  [25] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 1, 0, 0),
  [27] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0),
  [29] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(13),
  [32] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_list, 4, 0, 0),
  [34] = {.entry = {.count = 1, .reusable = true}}, SHIFT(4),
  [36] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_list, 5, 0, 0),
  [38] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_list_repeat1, 2, 0, 0),
  [40] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_list_repeat1, 2, 0, 0), SHIFT_REPEAT(4),
  [43] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_line, 2, 0, 0),
  [45] = {.entry = {.count = 1, .reusable = true}}, SHIFT(15),
  [47] = {.entry = {.count = 1, .reusable = true}},  ACCEPT_INPUT(),
  [49] = {.entry = {.count = 1, .reusable = true}}, SHIFT(2),
};

enum ts_external_scanner_symbol_identifiers {
  ts_external_token_indent = 0,
  ts_external_token_list_indent = 1,
};

static const TSSymbol ts_external_scanner_symbol_map[EXTERNAL_TOKEN_COUNT] = {
  [ts_external_token_indent] = sym_indent,
  [ts_external_token_list_indent] = sym_list_indent,
};

static const bool ts_external_scanner_states[3][EXTERNAL_TOKEN_COUNT] = {
  [1] = {
    [ts_external_token_indent] = true,
    [ts_external_token_list_indent] = true,
  },
  [2] = {
    [ts_external_token_list_indent] = true,
  },
};

#ifdef __cplusplus
extern "C" {
#endif
void *tree_sitter_tupi_external_scanner_create(void);
void tree_sitter_tupi_external_scanner_destroy(void *);
bool tree_sitter_tupi_external_scanner_scan(void *, TSLexer *, const bool *);
unsigned tree_sitter_tupi_external_scanner_serialize(void *, char *);
void tree_sitter_tupi_external_scanner_deserialize(void *, const char *, unsigned);

#ifdef TREE_SITTER_HIDE_SYMBOLS
#define TS_PUBLIC
#elif defined(_WIN32)
#define TS_PUBLIC __declspec(dllexport)
#else
#define TS_PUBLIC __attribute__((visibility("default")))
#endif

TS_PUBLIC const TSLanguage *tree_sitter_tupi(void) {
  static const TSLanguage language = {
    .version = LANGUAGE_VERSION,
    .symbol_count = SYMBOL_COUNT,
    .alias_count = ALIAS_COUNT,
    .token_count = TOKEN_COUNT,
    .external_token_count = EXTERNAL_TOKEN_COUNT,
    .state_count = STATE_COUNT,
    .large_state_count = LARGE_STATE_COUNT,
    .production_id_count = PRODUCTION_ID_COUNT,
    .field_count = FIELD_COUNT,
    .max_alias_sequence_length = MAX_ALIAS_SEQUENCE_LENGTH,
    .parse_table = &ts_parse_table[0][0],
    .small_parse_table = ts_small_parse_table,
    .small_parse_table_map = ts_small_parse_table_map,
    .parse_actions = ts_parse_actions,
    .symbol_names = ts_symbol_names,
    .symbol_metadata = ts_symbol_metadata,
    .public_symbol_map = ts_symbol_map,
    .alias_map = ts_non_terminal_alias_map,
    .alias_sequences = &ts_alias_sequences[0][0],
    .lex_modes = ts_lex_modes,
    .lex_fn = ts_lex,
    .external_scanner = {
      &ts_external_scanner_states[0][0],
      ts_external_scanner_symbol_map,
      tree_sitter_tupi_external_scanner_create,
      tree_sitter_tupi_external_scanner_destroy,
      tree_sitter_tupi_external_scanner_scan,
      tree_sitter_tupi_external_scanner_serialize,
      tree_sitter_tupi_external_scanner_deserialize,
    },
    .primary_state_ids = ts_primary_state_ids,
  };
  return &language;
}
#ifdef __cplusplus
}
#endif
