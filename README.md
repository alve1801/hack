# Nano2Tetris Hack emulator

Emulates a Hack processor from the [nand2tetris](https://www.nand2tetris.org/) project. Note: it implements the version used in [the browser game](https://nandgame.com/) instead of the book, since I found the ALU arguments for that one to be neater.

## Language description

XXX

## Compilation

`gcc eval.c`

First argument is the file to load into ROM (as defined in the book, also see language description), second (optional) file are inputs to the machine (currently doesn't work)

## Files

Included a couple pre-made programs.

- `add.hack` adds two numbers (6 and 7) and stores the result in R3 (0xd)
- `loop.hack` implements an endless loop - made to test end conditions of the emulator
- `read.hack` is likewise a test file for reading data (currently doesn't work)
- `sum.hack` implements a for-loop to sum all numbers between 0 and 100, likewise outputting in R3 (0x13ad)
