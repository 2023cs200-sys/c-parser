# Error Handling

Errors are reported to standard output with a line number when the relevant token is available. The program exits with status `1` after reporting the first error.

## Syntax errors

Syntax errors occur when tokens do not match the grammar. Examples include a missing semicolon, a missing variable name, an invalid statement start, and an unexpected closing parenthesis.

Typical messages include:

```text
Syntax Error at line 1: Expected ';' but found 'print'
Syntax Error at line 2: Expected variable name after 'int'
Syntax Error at line 3: Unexpected statement start token '@'
```

`expect()` handles required punctuation and keywords. The statement and factor functions handle invalid statement starts and invalid expression tokens.

## Lexical errors

Unsupported characters are classified as `TOKEN_ERROR`. They are not accepted as valid statement starts or expression factors, so the parser reports a syntax error when it encounters one.

## Runtime errors

Division by zero is checked in `term()` immediately before integer division:

```text
Runtime Error at line 4: Division by zero
```

## Command-line errors

Running without a source file prints the usage message. A file that cannot be opened produces an error naming the requested path. Memory allocation failure is also reported by `main()`.