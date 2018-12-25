# Intro

It is simple NES emulator written in C. It is not complete yet, but it is easy extendable and very modular.

# Status

For now it boots only Castelvania and Metal Gear, but it wasn't tested much.

# Mappers

If you want to add mapper you can look at [UXROM mapper](https://github.com/i1i1/NESemul/blob/master/src/uxrom.c). There are only 3 functions.

Also you need to add references of functions to [mapper structure](https://github.com/i1i1/NESemul/blob/master/src/cpu.c).

# CPU

Some [unofficial instructions](https://github.com/i1i1/NESemul/blob/master/src/opcodes.c) are uncomplete yet.

