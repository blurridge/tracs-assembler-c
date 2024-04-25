#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#define FILE_PATH "./data/assembly.asm"
#define OUTPUT_PATH "./output/assembled.txt"

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
        char **assembledLines = malloc(numLines * sizeof(char *));
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