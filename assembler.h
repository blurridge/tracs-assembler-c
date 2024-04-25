#ifndef ASSEMBLER_H_   
#define ASSEMBLER_H_

int assembleToC(char **assembledLines, char **extractedLines, int numLines);
int checkIfEOPExists(char **extractedLines, int numLines);
unsigned int getStartingAddress(char **extractedLines, int numLines);
int getBranches(char **extractedLines, int numLines, char **branchKeys, unsigned int *branchValues, unsigned int startingAddress);
int convertAsmToC(char **assembledLines, char **extractedLines, int numLines, int numBranches, char **branchKeys, unsigned int *branchValues, unsigned int startingAddress);
int checkValidAddress(char *instruction, unsigned int operand);
char *getInstructionString(unsigned int instruction, unsigned int *currentAddress);
char *getOperandString(unsigned int operand, unsigned int *currentAddress);

#endif 