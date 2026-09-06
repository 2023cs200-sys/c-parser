# Semantic Analysis

The parser performs semantic checks while it parses. Variable information is stored in a fixed-size symbol table containing a name, integer value, and declaration flag.

## Symbol table operations

- `lookup_variable(name)` searches existing entries and returns an index or `-1`.
- `add_variable(name, value)` inserts a declaration, rejects duplicate names with `-2`, and returns `-3` if the table reaches `MAX_VARS` (100 entries).
- An assignment updates the value of an existing symbol.

## Declaration rules

Declarations must include an initializer:

```text
int x = 5;
```

Declaring the same name twice is an error:

```text
int x = 5;
int x = 10;
```

Output:

```text
Semantic Error at line 2: Variable 'x' is already declared
```

## Use-before-declaration

Identifiers used in assignments, print expressions, and arithmetic factors must already exist in the symbol table. For example, `print(y);` fails if `y` has not been declared.

```text
Semantic Error at line 3: Variable 'y' is undeclared
```

The current implementation is single-pass: declarations and assignments affect all later statements, but there is no separate intermediate representation or later semantic-analysis pass.