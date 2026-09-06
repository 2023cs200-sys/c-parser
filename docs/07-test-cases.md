# Test Cases

Compile the parser before running these cases. Expected output assumes the current implementation and a successful executable named `c-parser`.

## Valid input: `input.txt`

```text
int y = 5;
int x = 20;
int z = x + y;
print(z);
```

Expected output:

```text
25
```

## Provided error input: `error_input.txt`

```text
int x = 5;
int x = 10;
print(x + y);
```

Actual result: parsing stops on line 2, so the later undeclared `y` is never reached.

```text
Semantic Error at line 2: Variable 'x' is already declared
```

## Additional cases

| Case | Input | Expected result |
| --- | --- | --- |
| Assignment | `int x = 4; x = x + 3; print(x);` | `7` |
| Precedence | `print(2 + 3 * 4);` | `14` |
| Parentheses | `print((2 + 3) * 4);` | `20` |
| Undeclared name | `print(y);` | Semantic error for `y` |
| Missing semicolon | `int x = 4` | Syntax error expecting `;` |
| Division by zero | `print(8 / 0);` | Runtime error for division by zero |
| Duplicate declaration | `int x = 1; int x = 2;` | Semantic error for `x` |

The exact error text and line number should be checked when validating an implementation change.