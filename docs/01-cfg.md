# Context-Free Grammar

This project implements a small statement-oriented language. The grammar is written in EBNF, where `{ ... }` means zero or more repetitions and `|` means choice.

## Grammar

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

## Rule explanations

| Rule | Meaning |
| --- | --- |
| `<program>` | The complete input is a list of statements followed by end-of-file. |
| `<stmt_list>` | Allows any number of statements, including an empty file. |
| `<stmt>` | Selects a declaration, assignment, or print statement. |
| `<decl_stmt>` | Declares and initializes an integer variable. |
| `<assign_stmt>` | Replaces the value of an existing variable. |
| `<print_stmt>` | Evaluates an expression and writes its integer result. |
| `<expr>` | Handles addition and subtraction. |
| `<term>` | Handles multiplication and division. |
| `<factor>` | Handles identifiers, integer literals, and parenthesized expressions. |

## Precedence

The grammar gives multiplication and division higher precedence than addition and subtraction. For example, `2 + 3 * 4` is evaluated as `2 + (3 * 4)`. Parentheses override the normal precedence: `(2 + 3) * 4` is evaluated first inside the parentheses.

Unary operators are not part of the grammar, so negative literals such as `-5` are not accepted.