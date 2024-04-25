#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"

// data memory start address is implied to be at 0x400 until 0x7FF and I/O memory is at 0x000 to 0x01F (WIO, RIO)

typedef struct
{
    unsigned char opcode;
    int needs_adding;
} INSTRUCTION;

const char *INSTRUCTION_TO_HEX_KEYS[] = {
    "ADD", "SUB", "MUL", "AND", "OR", "NOT", "XOR", "SHL", "SHR",
    "WM", "RM", "RIO", "WIO", "WB", "WIB", "WACC", "RACC",
    "SWAP", "BR", "BRE", "BRNE", "BRGT", "BRLT", "EOP"};

const int NUM_INSTRUCTIONS = sizeof(INSTRUCTION_TO_HEX_KEYS) / sizeof(INSTRUCTION_TO_HEX_KEYS[0]);

const INSTRUCTION INSTRUCTION_TO_HEX_VALUES[] = {
    {0xF0, 0}, {0xE8, 0}, {0xD8, 0}, {0xD0, 0}, {0xC8, 0}, {0xC0, 0}, {0xB8, 0}, {0xB0, 0}, {0xA8, 0}, {0x08, 1}, {0x10, 1}, {0x20, 1}, {0x28, 1}, {0x30, 0}, {0x38, 0}, {0x48, 0}, {0x58, 0}, {0x70, 0}, {0x18, 1}, {0xA0, 1}, {0x98, 1}, {0x90, 1}, {0x88, 1}, {0xF8, 0}};

int assembleToC(char **assembledLines, char **extractedLines, int numLines)
{
    int eopExists = checkIfEOPExists(extractedLines, numLines);
    unsigned int startingAddress = getStartingAddress(extractedLines, numLines);
    if (eopExists && startingAddress < 0xFFFF)
    {
        char **branchKeys = malloc(numLines * sizeof(char *));
        unsigned int *branchValues = malloc(numLines * sizeof(unsigned int));
        int numBranches = getBranches(extractedLines, numLines, branchKeys, branchValues, startingAddress);
        

        return 1;
    }
    else
    {
        printf("ERROR!");
        return 0;
    }
}

int checkIfEOPExists(char **extractedLines, int numLines)
{
    int i;
    for (i = 0; i < numLines; i++)
    {
        char *eopExists = strstr(extractedLines[i], "EOP");
        if (eopExists)
        {
            return 1;
        }
    }
    return 0;
}

unsigned int getStartingAddress(char **extractedLines, int numLines)
{
    int i;
    unsigned int startingAddress = 0x00;
    char extraChar; // Variable to check for extra characters after the hex number

    for (i = 0; i < numLines; i++)
    {
        char *orgExists = strstr(extractedLines[i], "ORG");
        if (orgExists)
        {
            int parsed = sscanf(extractedLines[i], "ORG %x%c", &startingAddress, &extraChar);
            if (parsed == 1)
            {
                return startingAddress;
            }
            else if (parsed == 2)
            {
                printf("[ERROR] Invalid memory address due to unexpected characters.\n");
                return 0xFFFF;
            }
            else
            {
                printf("[ERROR] Invalid memory address format.\n");
                return 0xFFFF;
            }
        }
    }
    return startingAddress;
}

int getBranches(char **extractedLines, int numLines, char **branchKeys, unsigned int *branchValues, unsigned int startingAddress)
{
    int i, j, numBranches = 0;
    char *token;
    char tempLine[256];
    for (i = 0; i < numLines; i++)
    {
        strcpy(tempLine, extractedLines[i]);
        token = strtok(tempLine, " \t");
        int matchFlag = 0;
        if (token != NULL)
        {
            // Check if the token is in the INSTRUCTION_TO_HEX_KEYS array
            for (j = 0; j < NUM_INSTRUCTIONS; j++)
            {
                if (strcmp(token, INSTRUCTION_TO_HEX_KEYS[j]) == 0 || strcmp(token, "ORG") == 0)
                {
                    matchFlag = 1;
                    break;
                }
            }
            if (!matchFlag)
            {
                branchKeys[numBranches] = strdup(token);
                branchValues[numBranches] = startingAddress;
                numBranches++;
            }
            matchFlag = 0;
        }
        startingAddress += 2;
    }
    return numBranches;
}