# Program Execution

## Compile with GCC or Clang

From the project root:

```sh
gcc -Wall -Wextra -std=c11 -o c-parser parser.c
```

Clang uses the same command shape:

```sh
clang -Wall -Wextra -std=c11 -o c-parser parser.c
```

On Windows with MinGW, the output is usually `c-parser.exe` and can be run as:

```powershell
gcc -Wall -Wextra -std=c11 -o c-parser.exe parser.c
.\\c-parser.exe input.txt
```

## Run a source file

```sh
./c-parser input.txt
./c-parser error_input.txt
```

The executable requires exactly one source path in normal use. The parser loads the entire file into memory, tokenizes it, parses statements, and evaluates them immediately. Printed expression values go to standard output; errors are also printed to standard output and terminate the process with exit status `1`.

## Create a custom program

Save a program such as this in `example.txt`:

```text
int x = 5;
int y = 20;
print(x + y);
x = x * 2;
print(x);
```

Then run `./c-parser example.txt`. The output is `25` followed by `10`.