#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_PATH "./data/assembly.asm"
#define OUTPUT_PATH "./output/assembled.txt"

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

int getLinesFromFile(FILE *currentFile, char **lines);
int assembleToC(char **assembledLines, char **extractedLines, int numLines);
int convertAsmToC(char **assembledLines, char **extractedLines, int numLines, unsigned int startingAddress);
int checkIfEOPExists(char **extractedLines, int numLines);
unsigned int getStartingAddress(char **extractedLines, int numLines);
int checkValidAddress(char *instruction, unsigned int operand);
unsigned int findBranch(char *stringOperand, char **extractedLines, unsigned int *currentAddress, int numLines, int currentLine);
char *getInstructionString(unsigned int instruction, unsigned int *currentAddress);
char *getOperandString(unsigned int operand, unsigned int *currentAddress);

int main()
{
    int numLines = 0;
    int conversionStatus;
    char *lines[1024];
    FILE *currentFile = fopen(FILE_PATH, "r");
    numLines = getLinesFromFile(currentFile, lines);
    if (numLines > 0)
    {
        char **assembledLines = malloc(2 * numLines * sizeof(char *));
        if (assembledLines == NULL)
        {
            // If malloc fails, handle the memory allocation failure
            fprintf(stderr, "Error: Memory allocation for assembledLines failed.\n");
            return 1; // Return an error code or handle it as needed
        }
        conversionStatus = assembleToC(assembledLines, lines, numLines);
        if (conversionStatus)
        {
            FILE *file = fopen(OUTPUT_PATH, "w");
            if (file == NULL)
            {
                perror("Failed to open file");
                return 1;
            }

            // Write each string to the file

            for (int i = 0; i < conversionStatus; i++)
            {
                fprintf(file, "%s\n", assembledLines[i]); // Adds a newline after each string
            }

            // Close the file
            fclose(file);
            printf("[SUCCESS] Done assembling %s!\n", FILE_PATH);
        }
        else
        {
            printf("[ERROR] Error assembling %s.\n", FILE_PATH);
        }
    }
    else
    {
        printf("[ERROR] Unable to open file %s or file is empty.\n", FILE_PATH);
    }
}

int getLinesFromFile(FILE *currentFile, char **lines)
{
    if (currentFile != NULL)
    {
        int numLines = 0;
        char line[1024];
        while (fgets(line, sizeof(line), currentFile) != NULL)
        {
            line[strcspn(line, "\n")] = '\0'; // Replace \n in files with \0 null terminator.
            lines[numLines++] = strdup(line); // Copy string to lines array
        }
        fclose(currentFile);
        return numLines;
    }
    else
    {
        return 0;
    }
}

int assembleToC(char **assembledLines, char **extractedLines, int numLines)
{
    int eopExists = checkIfEOPExists(extractedLines, numLines);
    unsigned int startingAddress = getStartingAddress(extractedLines, numLines);
    if (eopExists && startingAddress < 0xFFFF)
    {
        int numAssembledLines = convertAsmToC(assembledLines, extractedLines, numLines, startingAddress);
        return numAssembledLines;
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

int convertAsmToC(char **assembledLines, char **extractedLines, int numLines, unsigned int startingAddress)
{
    int i, j, numAssembledLines = 0;

    unsigned int *currentAddress = &startingAddress;
    int validAddress = 1, orgFlag = 0;
    char *token;
    char tempLine[256];
    for (i = 0; i < numLines; i++)
    {
        orgFlag = 0;
        unsigned int instruction, operand;
        strcpy(tempLine, extractedLines[i]);
        token = strtok(tempLine, " \t");

        if (!validAddress)
        {
            return 0;
        }

        while (token != NULL && !orgFlag)
        {
            for (j = 0; j < NUM_INSTRUCTIONS; j++)
            {
                if(strcmp(token, "ORG") == 0)
                {
                    orgFlag = 1;
                    break;
                }
                if (strcmp(token, INSTRUCTION_TO_HEX_KEYS[j]) == 0)
                {
                    instruction = INSTRUCTION_TO_HEX_VALUES[j].opcode;
                    char *stringOperand = strtok(NULL, " \t");
                    if (stringOperand != NULL)
                    {
                        char prefix[3] = {0};
                        int parsed = sscanf(stringOperand, "%2s%x", prefix, &operand);
                        validAddress = checkValidAddress(token, operand);
                        if (validAddress)
                        {
                            if (strcmp(prefix, "0x") != 0)
                            {
                                operand = findBranch(stringOperand, extractedLines, currentAddress, numLines, i);
                            }
                            if (INSTRUCTION_TO_HEX_VALUES[j].needs_adding)
                            {
                                unsigned int current_instruction = (instruction << 8) + operand;
                                instruction = (current_instruction >> 8) & 0xFF;
                                operand = current_instruction & 0xFF;
                            }
                            token = NULL;
                            assembledLines[numAssembledLines++] = getInstructionString(instruction, currentAddress);
                            assembledLines[numAssembledLines++] = getInstructionString(operand, currentAddress);
                            break;
                        }
                        else
                        {
                            free(assembledLines);
                            assembledLines = NULL;
                            return 0;
                        }
                    }
                    else
                    {
                        operand = 0x00;
                        token = NULL;
                        assembledLines[numAssembledLines++] = getInstructionString(instruction, currentAddress);
                        assembledLines[numAssembledLines++] = getInstructionString(operand, currentAddress);
                        break;
                    }
                }
            }
            if (token != NULL)
            {
                token = strtok(NULL, " \t");
            }
        }
    }
    return numAssembledLines;
}

int checkValidAddress(char *instruction, unsigned int operand)
{
    if (strcmp(instruction, "WIO") == 0 || strcmp(instruction, "RIO") == 0)
    {
        return operand <= 0x01F;
    }
    else if (strcmp(instruction, "WM") == 0 || strcmp(instruction, "RM") == 0)
    {
        return operand >= 0x400 && operand <= 0x7FF;
    }
    else
    {
        return 1;
    }
}

unsigned int findBranch(char *stringOperand, char **extractedLines, unsigned int *currentAddress, int numLines, int currentLine)
{
    unsigned int operand = 0x00;
    if (stringOperand)
    {
        if (strcmp(stringOperand, ";") != 0)
        {
            for (int k = 1; k < numLines; k++)
            {
                char *branchFound = strstr(extractedLines[k], stringOperand);
                if (branchFound && currentLine != k)
                {
                    return operand;
                }
                operand += 2;
            }
        }
    }
    return operand;
}

char *getInstructionString(unsigned int instruction, unsigned int *currentAddress)
{
    char buffer[50];
    int max_len = sizeof(buffer);

    int len = snprintf(buffer, max_len, "ADDR=0x%02X;BUS=0x%02X;MainMemory();", *currentAddress, instruction);
    (*currentAddress)++;
    if (len >= 0 && len < max_len)
    {
        char *result = malloc(len + 1);
        if (result)
        {
            strcpy(result, buffer);
            return result;
        }
    }
    return NULL;
}

char *getOperandString(unsigned int operand, unsigned int *currentAddress)
{
    char buffer[50];
    int max_len = sizeof(buffer);

    int len = snprintf(buffer, max_len, "ADDR=0x%02X;BUS=0x%02X;MainMemory();", *currentAddress, operand);
    (*currentAddress)++;
    if (len >= 0 && len < max_len)
    {
        char *result = malloc(len + 1);
        if (result)
        {
            strcpy(result, buffer);
            return result;
        }
    }
    return NULL;
}