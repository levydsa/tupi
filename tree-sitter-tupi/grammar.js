/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

module.exports = grammar({
  name: "tupi",

  externals: $ => [
    $.indent,
    $.list_indent,
  ],

  rules: {
    source_file: $ => repeat($.list), // repeat(seq($.indent, $.word)),

    space: _ => token.immediate(/ +/),
    word: _ => token.immediate(/\w+/),
    math: _ => token.immediate(seq('$', /[^$]*/, '$')),
    symbol: _ => token.immediate(/[^$ \n]+/),
    nl: _ => token.immediate('\n'),

    line: $ => seq(repeat1($._inline), $.nl),
    list: $ => seq($.list_indent, '-', $.space, $.line, repeat(seq($.indent, $.line))),

    _inline: $ => choice(
      $.word,
      $.space,
      $.math,
      $.symbol,
    )
  }
});
