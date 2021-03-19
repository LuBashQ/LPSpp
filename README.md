# LSPpp

LSPpp is an interpreter for the simple assembly-like programming language LSP. It's written in (mostly) C++11.

## How are instructions interpreted

Each instruction has a `type` and a `class`, so that the interpreter can distinguish between valid and invalid operations and allow overloading.

An instruction belongs to at least one of the following classes:

|Class|Semantic|
|--|--|
|`STACK`| The instruction involves the stack|
|`REGISTER`| The instruction involves the registers|
|`SPECIAL`| The instruction does special operations which are neither on the stack nor on the registers|
|`IMMEDIATE`| The instruction involves a literal|

Note that `SPECIAL` cannot be combined with any other class.

A type is a possible instruction. Currently there are `9` valid instructions:

|Type|Classes|Semantic|
|--|--|---|
|`PUSH (x \| @x)`| `STACK \| IMMEDIATE`|Pushes `x` onto the stack or into the `x` register|
|`POP @x`|`STACK \| REGISTER`|Pops the top of the stack into the `x` register|
|`PRINT @x`|`REGISTER`|Prints the content of the `x` register|
|`HLT`|`SPECIAL`|Terminates the execution of the program|
|`ADD,SUB,MUL,DIV (x \| @x) @y`|`REGISTER \| IMMEDIATE`|Executes the arithmetic operation and saves the result into the `y` register|
|`MOVE (x \| @x) @y`|`REGISTER \| IMMEDIATE`| Copies the content of the `x` register, or the literal `x`, into the `y` register|

