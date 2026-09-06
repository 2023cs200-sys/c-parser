# Mini C Parser and Interpreter Report

## 1. Overview

This project implements a small integer language in C. It demonstrates the main stages of a language processor: lexical analysis, syntax analysis, semantic checks, and direct execution.

## 2. Language features

The language supports integer declarations, assignments, arithmetic expressions, parenthesized expressions, and printing. Identifiers may contain letters, digits, and underscores, provided the first character is a letter or underscore.

## 3. Design

The lexer converts source characters into tokens. A recursive-descent parser then follows the context-free grammar. The expression grammar is divided into `expr`, `term`, and `factor`, which naturally implements arithmetic precedence. A fixed symbol table stores declared variables and current values.

## 4. Semantic and runtime checks

The implementation rejects duplicate declarations and uses of undeclared variables. It also prevents division by zero. Syntax, semantic, and runtime failures report a line number and terminate with a nonzero status.

## 5. Evaluation strategy

The parser is also an interpreter. Expression functions compute integer values immediately, declaration and assignment statements update the symbol table, and `print` writes results as soon as it is parsed. No syntax tree is constructed.

## 6. Validation

The supplied `input.txt` produces `25`. The supplied `error_input.txt` stops at its duplicate declaration on line 2. Additional tests cover precedence, parentheses, assignment, missing punctuation, undeclared variables, and division by zero; see [07-test-cases.md](07-test-cases.md).

## 7. Limitations and future work

The language has no comments, strings, floating-point values, booleans, functions, control flow, unary operators, or modulo operator. The symbol table is limited to 100 variables, lexemes are limited to 99 stored characters, and integer overflow is not detected. Future versions could add comments, unary minus, richer diagnostics, an abstract syntax tree, and separate semantic-analysis and execution phases.

## 8. Conclusion

The project provides a focused example of how a grammar maps to recursive-descent functions and how a parser can be extended into a small interpreter. Its compact design makes the token flow, precedence rules, and semantic checks easy to inspect and test.