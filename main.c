#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#define FILE_PATH "./data/assembly.asm"

int getLinesFromFile(FILE *currentFile, char **lines);

int main()
{
    int numLines = 0;
    int conversionStatus;
    char *lines[1024];
    FILE *currentFile = fopen(FILE_PATH, "r");
    numLines = getLinesFromFile(currentFile, lines);
    if (numLines > 0)
    {
        char **assembledLines = malloc(numLines * sizeof(char*));
        conversionStatus = assembleToC(assembledLines, lines, numLines);
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