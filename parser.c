#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Lexical Analyzer (Tokenizer) Definitions
typedef enum {
    TOKEN_INT,        // 'int' keyword
    TOKEN_PRINT,      // 'print' keyword
    TOKEN_ID,         // Identifier (variable name e.g., x, y, z)
    TOKEN_NUM,        // Numeric integer constant (e.g., 5, 20)
    TOKEN_ASSIGN,     // '=' operator
    TOKEN_ADD,        // '+' operator
    TOKEN_SUB,        // '-' operator
    TOKEN_MUL,        // '*' operator
    TOKEN_DIV,        // '/' operator
    TOKEN_LPAREN,     // '(' parenthesis
    TOKEN_RPAREN,     // ')' parenthesis
    TOKEN_SEMICOLON,  // ';' statement terminator
    TOKEN_EOF,        // End of File
    TOKEN_ERROR       // Invalid token/lexical error marker
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[100];  
    int value;         
    int line;          
} Token;

// Global tokenizer variables
const char *src_code;  
int src_pos = 0;       
int line_num = 1;      
Token nextToken;       

// Symbol Table Definitions
#define MAX_VARS 100

typedef struct {
    char name[100];    
    int value;         
    int is_declared;  
} Symbol;

Symbol symbol_table[MAX_VARS];
int symbol_count = 0;

// Returns the index of a variable if found, otherwise returns -1
int lookup_variable(const char *name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Registers a variable in the symbol table with a value.

int add_variable(const char *name, int value) {
    int index = lookup_variable(name);
    if (index != -1) {
        return -2; // Semantic Error: Duplicate declaration
    }
    if (symbol_count >= MAX_VARS) {
        return -3; // Symbol Table limit exceeded
    }
    strcpy(symbol_table[symbol_count].name, name);
    symbol_table[symbol_count].value = value;
    symbol_table[symbol_count].is_declared = 1;
    symbol_count++;
    return symbol_count - 1;
}

// Lexical Analyzer Implementation (lex())

void lex() {
    // Skip any leading whitespace and update line count
    while (src_code[src_pos] != '\0' && isspace((unsigned char)src_code[src_pos])) {
        if (src_code[src_pos] == '\n') {
            line_num++;
        }
        src_pos++;
    }

    nextToken.line = line_num;
    nextToken.lexeme[0] = '\0';
    nextToken.value = 0;

    // End of File reached
    if (src_code[src_pos] == '\0') {
        nextToken.type = TOKEN_EOF;
        strcpy(nextToken.lexeme, "EOF");
        return;
    }

    char c = src_code[src_pos];

    // Case 1: Identifiers and Keywords (begins with a letter or underscore)
    if (isalpha((unsigned char)c) || c == '_') {
        int len = 0;
        while (src_code[src_pos] != '\0' && (isalnum((unsigned char)src_code[src_pos]) || src_code[src_pos] == '_')) {
            if (len < 99) {
                nextToken.lexeme[len++] = src_code[src_pos];
            }
            src_pos++;
        }
        nextToken.lexeme[len] = '\0';

        // Categorise as keyword or custom identifier
        if (strcmp(nextToken.lexeme, "int") == 0) {
            nextToken.type = TOKEN_INT;
        } else if (strcmp(nextToken.lexeme, "print") == 0) {
            nextToken.type = TOKEN_PRINT;
        } else {
            nextToken.type = TOKEN_ID;
        }
        return;
    }

    // Case 2: Integer Constants (sequences of digits)
    if (isdigit((unsigned char)c)) {
        int len = 0;
        while (src_code[src_pos] != '\0' && isdigit((unsigned char)src_code[src_pos])) {
            if (len < 99) {
                nextToken.lexeme[len++] = src_code[src_pos];
            }
            src_pos++;
        }
        nextToken.lexeme[len] = '\0';
        nextToken.type = TOKEN_NUM;
        nextToken.value = atoi(nextToken.lexeme);
        return;
    }

    // Case 3: Single-character Operators and Punctuation symbols
    src_pos++; // Consume character
    nextToken.lexeme[0] = c;
    nextToken.lexeme[1] = '\0';

    switch (c) {
        case '=': nextToken.type = TOKEN_ASSIGN; break;
        case '+': nextToken.type = TOKEN_ADD; break;
        case '-': nextToken.type = TOKEN_SUB; break;
        case '*': nextToken.type = TOKEN_MUL; break;
        case '/': nextToken.type = TOKEN_DIV; break;
        case '(': nextToken.type = TOKEN_LPAREN; break;
        case ')': nextToken.type = TOKEN_RPAREN; break;
        case ';': nextToken.type = TOKEN_SEMICOLON; break;
        default:
            nextToken.type = TOKEN_ERROR;
            break;
    }
}

// Recursive Descent Parser

void program();
void stmt_list();
void stmt();
void decl_stmt();
void assign_stmt();
void print_stmt();
int expr();
int term();
int factor();

// Helper function to assert and consume expected tokens
void expect(TokenType type, const char *expected_desc) {
    if (nextToken.type == type) {
        lex(); // Consume the expected token
    } else {
        printf("Syntax Error at line %d: Expected %s but found '%s'\n", nextToken.line, expected_desc, nextToken.lexeme);
        exit(1);
    }
}

// Rule: <program> ::= <stmt_list>
void program() {
    stmt_list();
    if (nextToken.type != TOKEN_EOF) {
        printf("Syntax Error at line %d: Unexpected tokens after program end '%s'\n", nextToken.line, nextToken.lexeme);
        exit(1);
    }
}

// Rule: <stmt_list> ::= <stmt> <stmt_list> | epsilon
void stmt_list() {
    while (nextToken.type != TOKEN_EOF) {
        stmt();
    }
}

// Rule: <stmt> ::= <decl_stmt> | <assign_stmt> | <print_stmt>
void stmt() {
    if (nextToken.type == TOKEN_INT) {
        decl_stmt();
    } else if (nextToken.type == TOKEN_PRINT) {
        print_stmt();
    } else if (nextToken.type == TOKEN_ID) {
        assign_stmt();
    } else {
        printf("Syntax Error at line %d: Unexpected statement start token '%s'\n", nextToken.line, nextToken.lexeme);
        exit(1);
    }
}

// Rule: <decl_stmt> ::= 'int' <id> '=' <expr> ';'
void decl_stmt() {
    int start_line = nextToken.line;
    expect(TOKEN_INT, "'int'");

    char var_name[100];
    if (nextToken.type == TOKEN_ID) {
        strcpy(var_name, nextToken.lexeme);
        lex();
    } else {
        printf("Syntax Error at line %d: Expected variable name after 'int'\n", nextToken.line);
        exit(1);
    }

    expect(TOKEN_ASSIGN, "'='");

    int value = expr();

    expect(TOKEN_SEMICOLON, "';'");

    // Semantic Check: Duplicate declaration error detection
    int res = add_variable(var_name, value);
    if (res == -2) {
        printf("Semantic Error at line %d: Variable '%s' is already declared\n", start_line, var_name);
        exit(1);
    }
}

// Rule: <assign_stmt> ::= <id> '=' <expr> ';'
void assign_stmt() {
    char var_name[100];
    int start_line = nextToken.line;
    if (nextToken.type == TOKEN_ID) {
        strcpy(var_name, nextToken.lexeme);
        lex();
    } else {
        printf("Syntax Error at line %d: Expected variable name\n", nextToken.line);
        exit(1);
    }

    expect(TOKEN_ASSIGN, "'='");

    int value = expr();

    expect(TOKEN_SEMICOLON, "';'");

    // Semantic Check: Undeclared variable usage detection
    int index = lookup_variable(var_name);
    if (index == -1) {
        printf("Semantic Error at line %d: Variable '%s' is undeclared\n", start_line, var_name);
        exit(1);
    }
    symbol_table[index].value = value;
}

// Rule: <print_stmt> ::= 'print' '(' <expr> ')' ';'
void print_stmt() {
    expect(TOKEN_PRINT, "'print'");
    expect(TOKEN_LPAREN, "'('");

    // We allow general expressions to be printed, which adds incredible functionality
    int value = expr();

    expect(TOKEN_RPAREN, "')'");
    expect(TOKEN_SEMICOLON, "';'");

    // Display execution logic output to the console
    printf("%d\n", value);
}

// Rule: <expr> ::= <term> { ('+' | '-') <term> }
int expr() {
    int val = term();
    while (nextToken.type == TOKEN_ADD || nextToken.type == TOKEN_SUB) {
        TokenType op = nextToken.type;
        lex();
        int right = term();
        if (op == TOKEN_ADD) {
            val += right;
        } else {
            val -= right;
        }
    }
    return val;
}

// Rule: <term> ::= <factor> { ('*' | '/') <factor> }
int term() {
    int val = factor();
    while (nextToken.type == TOKEN_MUL || nextToken.type == TOKEN_DIV) {
        TokenType op = nextToken.type;
        int op_line = nextToken.line;
        lex();
        int right = factor();
        if (op == TOKEN_MUL) {
            val *= right;
        } else {
            // Semantic / Runtime Check: Division by zero prevention
            if (right == 0) {
                printf("Runtime Error at line %d: Division by zero\n", op_line);
                exit(1);
            }
            val /= right;
        }
    }
    return val;
}

// Rule: <factor> ::= <id> | <num> | '(' <expr> ')'
int factor() {
    int val = 0;
    if (nextToken.type == TOKEN_ID) {
        // Semantic Check: Undeclared variable usage in arithmetic expression
        int index = lookup_variable(nextToken.lexeme);
        if (index == -1) {
            printf("Semantic Error at line %d: Variable '%s' is undeclared\n", nextToken.line, nextToken.lexeme);
            exit(1);
        }
        val = symbol_table[index].value;
        lex();
    } else if (nextToken.type == TOKEN_NUM) {
        val = nextToken.value;
        lex();
    } else if (nextToken.type == TOKEN_LPAREN) {
        lex();
        val = expr();
        expect(TOKEN_RPAREN, "')'");
    } else {
        printf("Syntax Error at line %d: Unexpected token '%s'\n", nextToken.line, nextToken.lexeme);
        exit(1);
    }
    return val;
}

// Command-Line Entry Point
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error: Missing source file argument.\n");
        printf("Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        printf("Error: Could not open file '%s'\n", argv[1]);
        return 1;
    }

    // Determine file size and load content to buffer
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(size + 1);
    if (!buffer) {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return 1;
    }

    size_t read_bytes = fread(buffer, 1, size, file);
    buffer[read_bytes] = '\0';
    fclose(file);

    src_code = buffer;
    src_pos = 0;
    line_num = 1;

    lex();

    program();

    // Free resources
    free(buffer);
    return 0;
}
