#ifndef ASSEMBLER_H_   
#define ASSEMBLER_H_

int assembleToC(char **assembledLines, char **extractedLines, int numLines);
int checkIfEOPExists(char **extractedLines, int numLines);
unsigned int getStartingAddress(char **extractedLines, int numLines);
int getBranches(char **extractedLines, int numLines, char **branchKeys, unsigned int *branchValues, unsigned int startingAddress);

#endif 