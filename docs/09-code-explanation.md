# Code Explanation

`parser.c` is a compact lexer, recursive-descent parser, symbol table, and interpreter.

## Important data structures

- `TokenType` enumerates every lexical category.
- `Token` stores a token type, text lexeme, integer value, and source line.
- `Symbol` stores a variable name, current integer value, and declaration flag.
- `symbol_table` is a fixed array of up to `MAX_VARS` (100) symbols.

## Global parser state

`src_code` points to the loaded source buffer. `src_pos` is the current character offset, `line_num` tracks source lines, and `nextToken` is the one-token lookahead used by the parser.

## Main functions

| Function | Responsibility |
| --- | --- |
| `lex()` | Skips whitespace and creates the next token. |
| `lookup_variable()` | Finds a name in the symbol table. |
| `add_variable()` | Adds a declaration and detects duplicates or capacity overflow. |
| `expect()` | Requires and consumes one token. |
| `program()` / `stmt_list()` | Parse the complete source and its statements. |
| `decl_stmt()` | Parse an initialized declaration and register it. |
| `assign_stmt()` | Parse an assignment and update a stored value. |
| `print_stmt()` | Parse an output statement and print its result. |
| `expr()` / `term()` / `factor()` | Parse and evaluate expressions by precedence. |
| `main()` | Read the file, initialize parsing, execute the program, and free memory. |

## Execution model

There is no abstract syntax tree. Expression functions return values directly, and statements update the symbol table or print results during the parse. This keeps the implementation small but means the language executes in source order and does not support a later optimization or code-generation phase.