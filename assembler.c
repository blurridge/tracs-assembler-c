#include <stdio.h>
#include <string.h>

typedef struct
{
    unsigned char opcode;
    int needs_adding;
} INSTRUCTION;

const char **INSTRUCTION_TO_HEX_KEYS = {
    "ADD", "SUB", "MUL", "AND", "OR", "NOT", "XOR", "SHL", "SHR",
    "WM", "RM", "RIO", "WIO", "WB", "WIB", "WACC", "RACC",
    "SWAP", "BR", "BRE", "BRNE", "BRGT", "BRLT", "EOP"};

const INSTRUCTION INSTRUCTION_TO_HEX_VALUES = {
    {0xF0, 0}, {0xE8, 0}, {0xD8, 0}, {0xD0, 0}, {0xC8, 0}, {0xC0, 0}, {0xB8, 0}, {0xB0, 0}, {0xA8, 0}, {0x08, 1}, {0x10, 1}, {0x20, 1}, {0x28, 1}, {0x30, 0}, {0x38, 0}, {0x48, 0}, {0x58, 0}, {0x70, 0}, {0x18, 1}, {0xA0, 1}, {0x98, 1}, {0x90, 1}, {0x88, 1}, {0xF8, 0}};

