# Tokenizer

The tokenizer is implemented by `lex()` in `parser.c`. It reads characters from the source buffer and stores one lookahead token in the global `nextToken` object.

## Token types

| Token | Source text | Purpose |
| --- | --- | --- |
| `TOKEN_INT` | `int` | Declaration keyword |
| `TOKEN_PRINT` | `print` | Output keyword |
| `TOKEN_ID` | `total`, `_x2` | Variable identifier |
| `TOKEN_NUM` | `0`, `123` | Integer literal |
| `TOKEN_ASSIGN` | `=` | Assignment or initialization |
| `TOKEN_ADD` | `+` | Addition |
| `TOKEN_SUB` | `-` | Subtraction |
| `TOKEN_MUL` | `*` | Multiplication |
| `TOKEN_DIV` | `/` | Division |
| `TOKEN_LPAREN` | `(` | Opens an expression in `print` or a factor |
| `TOKEN_RPAREN` | `)` | Closes a parenthesized expression |
| `TOKEN_SEMICOLON` | `;` | Ends a statement |
| `TOKEN_EOF` | end of file | Ends the program |
| `TOKEN_ERROR` | unsupported character | Marks an invalid character |

## Lexing rules

- Whitespace is ignored. Newline characters increment `line_num` for diagnostics.
- An identifier starts with a letter or underscore and continues with letters, digits, or underscores.
- The exact identifier text `int` becomes `TOKEN_INT`, and `print` becomes `TOKEN_PRINT`; other names become `TOKEN_ID`.
- A number is a sequence of decimal digits and is converted with `atoi()`.
- Operators and punctuation are single-character tokens.
- Any unrecognized character becomes `TOKEN_ERROR`.

Each token also stores its original lexeme, numeric value when applicable, and source line number.