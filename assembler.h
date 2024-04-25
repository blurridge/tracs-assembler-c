#ifndef ASSEMBLER_H_   
#define ASSEMBLER_H_

int assembleToC(char **assembledLines, char **extractedLines, int numLines);
int convertAsmToC(char **assembledLines, char **extractedLines, int numLines, unsigned int startingAddress);
int checkIfEOPExists(char **extractedLines, int numLines);
unsigned int getStartingAddress(char **extractedLines, int numLines);
int checkValidAddress(char *instruction, unsigned int operand);
unsigned int findBranch(char *stringOperand, char **extractedLines, unsigned int *currentAddress, int numLines, int currentLine);
char *getInstructionString(unsigned int instruction, unsigned int *currentAddress);
char *getOperandString(unsigned int operand, unsigned int *currentAddress);

#endif 