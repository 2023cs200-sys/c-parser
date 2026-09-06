# Arithmetic Evaluation

Expressions are evaluated as they are parsed. The return value of each expression function is an integer.

## Evaluation layers

- `factor()` evaluates a number, looks up an identifier, or recursively evaluates a parenthesized expression.
- `term()` combines factors with `*` and `/` from left to right.
- `expr()` combines terms with `+` and `-` from left to right.

This structure implements precedence without a separate evaluator. For example:

```text
int x = 2 + 3 * 4;
print(x);
```

The result is `14`, because `term()` evaluates `3 * 4` before `expr()` adds `2`.

Parentheses create a nested call to `expr()`:

```text
print((2 + 3) * 4);
```

The result is `20`.

Arithmetic uses C `int` operations. Division is integer division, and division by zero is rejected before the division occurs. There is no unary minus, floating-point arithmetic, overflow detection, or modulo operator.