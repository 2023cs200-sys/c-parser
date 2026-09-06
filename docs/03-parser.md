# Parser

The parser is a recursive-descent parser. Each major grammar rule has a corresponding C function, and `nextToken` provides one-token lookahead.

## Control flow

1. `main()` loads the input file into memory and calls `lex()` once to initialize lookahead.
2. `program()` calls `stmt_list()` and then requires `TOKEN_EOF`.
3. `stmt()` chooses a parser based on the first token: `int` means declaration, `print` means output, and an identifier means assignment.
4. Statement functions consume their required tokens with `expect()`.
5. `expr()`, `term()`, and `factor()` parse and evaluate expressions using precedence levels.
6. The parser stops immediately when it reports an error.

## Function-to-rule mapping

| Grammar rule | C function |
| --- | --- |
| `<program>` | `program()` |
| `<stmt_list>` | `stmt_list()` |
| `<stmt>` | `stmt()` |
| `<decl_stmt>` | `decl_stmt()` |
| `<assign_stmt>` | `assign_stmt()` |
| `<print_stmt>` | `print_stmt()` |
| `<expr>` | `expr()` |
| `<term>` | `term()` |
| `<factor>` | `factor()` |

`expect()` validates the current token, consumes it by calling `lex()`, or prints a syntax error and exits with status `1`.

## Example parse

For `print(x + 2);`, the parser enters `print_stmt()`, consumes `print` and `(`, calls `expr()`, then consumes `)` and `;`. `expr()` parses `x` as a factor, sees `+`, parses `2` as another factor, and returns their sum.