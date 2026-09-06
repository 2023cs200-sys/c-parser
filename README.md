# c-parser

A tiny language interpreter written in a single C file (`parser.c`). It combines a **lexical analyzer (lexer)**, a **recursive descent parser**, a **symbol table**, and an **interpreter** that executes statements as it parses them.

The language supports integer variable declarations, assignments, arithmetic expressions with standard operator precedence, and a `print` statement.

## Project Structure

```
c-parser/
│
├── parser.c          # The complete implementation (lexer, parser, interpreter)
├── input.txt         # Example valid program
├── error_input.txt   # Example program containing semantic errors
├── README.md         # This file
├── .gitignore
│
└── docs/             # Detailed documentation
    ├── 01-cfg.md                 # Context-Free Grammar and rule-by-rule explanation
    ├── 02-tokenizer.md           # Token types, keywords, identifiers, operators
    ├── 03-parser.md              # How the parser follows the CFG
    ├── 04-error-handling.md      # Syntax-error detection and messages
    ├── 05-semantic-analysis.md  # Semantic errors (undeclared/duplicate variables)
    ├── 06-arithmetic-evaluation.md # How expressions like x + y are evaluated
    ├── 07-test-cases.md          # Test inputs with expected/actual results
    ├── 08-program-execution.md   # How to compile and run
    ├── 09-code-explanation.md   # Functions and data structures explained
    └── 10-report.md              # Final report (PDF-ready)
```

## Features

- **Lexer** — tokenizes identifiers, keywords (`int`, `print`), integer constants, operators (`+ - * / =`), parentheses, and semicolons; skips whitespace and tracks line numbers.
- **Recursive descent parser** — parses the language according to the grammar below and reports syntax errors with line numbers.
- **Symbol table** — up to 100 variables with declaration tracking.
- **Semantic checks** — duplicate variable declaration and use of undeclared variables are rejected.
- **Runtime check** — division by zero is detected.
- **Direct execution** — statements are evaluated during parsing (`print` writes the result to stdout).

## Grammar (EBNF)

```ebnf
<program>     ::= <stmt_list> EOF

<stmt_list>   ::= { <stmt> }

<stmt>        ::= <decl_stmt> | <assign_stmt> | <print_stmt>

<decl_stmt>   ::= 'int' <id> '=' <expr> ';'

<assign_stmt> ::= <id> '=' <expr> ';'

<print_stmt>  ::= 'print' '(' <expr> ')' ';'

<expr>        ::= <term> { ('+' | '-') <term> }

<term>        ::= <factor> { ('*' | '/') <factor> }

<factor>      ::= <id> | <num> | '(' <expr> ')'
```

Operator precedence is built into the grammar: `*` and `/` bind tighter than `+` and `-`, and parentheses can override it.

## Language Overview

| Construct | Example | Notes |
|---|---|---|
| Declaration | `int x = 5;` | Variables must be initialized when declared |
| Assignment | `x = x + 1;` | Only previously declared variables |
| Print | `print(x * 2);` | Prints the evaluated expression followed by a newline |

Identifiers start with a letter or `_` and may contain letters, digits, and `_`. Integer literals are sequences of digits. Statements are terminated with `;`.

## Getting Started

### Prerequisites

- A C compiler (e.g., `gcc`, `clang`, or MSVC)

### Build

```sh
gcc -o c-parser parser.c
```

### Run

```sh
./c-parser input.txt
```

`input.txt` is a small valid program shipped with the project; `error_input.txt` demonstrates error detection:

```sh
./c-parser error_input.txt
```

You can also write your own program in any text file and pass it to the parser, for example `example.txt`:

```
int x = 5;
int y = 20;
print(x + y);

x = x * 2;
print(x);

print((x + y) / 3);
```

Running it:

```sh
./c-parser example.txt
```

Output:

```
25
10
10
```

## Architecture

`parser.c` is organized into three layers:

1. **Lexer (`lex()`)** — reads the source one character at a time from a global buffer and fills the lookahead token (`nextToken`). Keywords are recognized by comparing identifier lexemes against `int` and `print`.
2. **Symbol table (`lookup_variable()`, `add_variable()`)** — a fixed-size array of `{ name, value, is_declared }` entries used to back semantic checks and variable storage.
3. **Recursive descent parser** — one function per grammar rule (`program`, `stmt_list`, `stmt`, `decl_stmt`, `assign_stmt`, `print_stmt`, `expr`, `term`, `factor`). The `expect()` helper consumes tokens or aborts with a syntax error. Because the grammar encodes precedence, `expr`/`term`/`factor` also evaluate the expression during parsing.

## Error Handling

All errors abort execution with `exit(1)` and include the offending line number.

| Kind | Example message | Trigger |
|---|---|---|
| Syntax | `Syntax Error at line 3: Expected ';' but found 'x'` | Malformed statement |
| Semantic | `Semantic Error at line 4: Variable 'x' is already declared` | Redeclaring a variable |
| Semantic | `Semantic Error at line 2: Variable 'y' is undeclared` | Using an unknown variable |
| Runtime | `Runtime Error at line 5: Division by zero` | `x / 0` |
| Usage | `Error: Missing source file argument.` | No file passed on the command line |

## Documentation

Each aspect of the implementation is documented in depth in [`docs/`](docs/). Start with [`docs/10-report.md`](docs/10-report.md) for the full write-up, or jump to a specific topic:

- [Context-Free Grammar](docs/01-cfg.md)
- [Tokenizer](docs/02-tokenizer.md)
- [Parser](docs/03-parser.md)
- [Error Handling](docs/04-error-handling.md)
- [Semantic Analysis](docs/05-semantic-analysis.md)
- [Arithmetic Evaluation](docs/06-arithmetic-evaluation.md)
- [Test Cases](docs/07-test-cases.md)
- [Program Execution](docs/08-program-execution.md)
- [Code Explanation](docs/09-code-explanation.md)
- [Final Report](docs/10-report.md)

## Limitations

- Integer-only arithmetic (no floating point, strings, or booleans)
- No negative literals (unary minus is not supported)
- Maximum of 100 variables and 99-character identifiers/lexemes
- A single parse-and-execute pass: a `print` before a later assignment sees the earlier value
