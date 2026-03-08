# Simplified C Compiler (Flex + Bison)

This project implements a compiler for a simplified version of the C language using **Flex** and **Bison**.

The compiler performs lexical analysis, syntax analysis, semantic validation, and generates assembly code for the input program.

---

## Technologies

- Flex
- Bison
- C
- NASM Assembly

---

## Language Features

The compiler supports:

- global variable declarations
- local variables in the main function
- integer, float, and string types
- assignments
- arithmetic expressions
- conditional statements (if / else)
- loops (while)
- read and write commands

---

## Symbol Table Design

Two symbol tables are implemented:

### Global scope

Stores variables declared outside the main block.

### Local scope

Stores variables declared inside the main block.

Variable lookup follows this rule:

1. check local symbol table
2. check global symbol table

Local variables override global variables with the same name.

---

## Example Program

```c
float total;
float media;
int impar;
string empresa;

{
 string nome;
 float x;
 float y;

 empresa = "UFMT";

 read(nome);

 read(x);
 read(y);

 total = x + y;
 media = total / 2;

 write(media);
}

```
## Compilation Process

```
Source Code
↓
Lexical Analysis (Flex)
↓
Syntax Analysis (Bison)
↓
Semantic Analysis
↓
Assembly Code Generation
```
↓
Semantic Analysis
↓
Assembly Code Generation

