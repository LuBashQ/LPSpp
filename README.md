# LSPpp

LSPpp is an interpreter for the simple assembly-like programming language LSP. It's written in C++1x.

## How to compile and run

You can either open the solution in Visual Studio 2019 or build using Cmake.

## Syntax

Instructions **must** be written in capital letters. Instructions are separated by a new line and the arguments are separated by spaces.
White spaces are allowed and do not count as instructions. `x` denotes a integer literal, `@x` refers to the x-th register starting from 0
and `:` denotes an integer label (only allowed in `JMP` instruction), denoting the number of instructions that will be skipped.
Comments are preceded by `;`.

## How are instructions interpreted

Each instruction has a `type` and a `class`, so that the interpreter can distinguish between valid and invalid operations and allow overloading.

An instruction belongs to at least one of the following classes:

|Class|Semantic|
|--|--|
|`STACK`| The instruction involves the stack|
|`REGISTER`| The instruction involves the registers|
|`SPECIAL`| The instruction does special operations which are neither on the stack nor on the registers|
|`IMMEDIATE`| The instruction involves a literal|
|`JUMP`|The instruction allows for relative jumps|
|`COMPARISON`|The instruction executes a comparison|

Note that `SPECIAL` cannot be combined with any other class.

A type is a possible instruction. Currently there are `9` valid instructions:

|Type|Classes|Semantic|
|--|--|---|
|`PUSH (x \| @x)`| `STACK \| IMMEDIATE`|Pushes `x` or `@x` onto the stack |
|`POP @x`|`STACK \| REGISTER`|Pops the top of the stack into `@x`|
|`PRINT @x`|`REGISTER`|Prints the content of `@x`|
|`HLT`|`SPECIAL`|Terminates the execution of the program|
|`ADD,SUB,MUL,DIV (x \| @x) @y`|`REGISTER \| IMMEDIATE`|Executes the arithmetic operation and saves the result into `@y`|
|`MOVE (x \| @x) @y`|`REGISTER \| IMMEDIATE`| Copies the content of the `@x`, or `x`, into `@y`|
|`GEQ, LEQ, GT, LT (x \| @x) @y`| `REGISTER \| IMMEDIATE`| Executes a comparison between `x` (or `@x`) and `@y`, and sets `@8` to `1` if true|
|`JMP (:x \| :@x)`|`STACK \| REGISTER \| JUMP`|Jumps to the `x` or `@x`-th instruction|

