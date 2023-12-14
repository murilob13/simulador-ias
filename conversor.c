#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_LINE_LENGTH 1000

// function to extract the memory address from an operation
void extract_address(char *line, char *address)
{
    while (*line != '(')
    {
        line++;
    }
    line++;

    while (*line != ')' && *line != ',')
    {
        *address = *line;
        address++;
        line++;
    }

    *address = '\0';
}

void remove_memory_address(char *line)
{
    regex_t regex;
    regmatch_t match;

    // Compile the regular expression pattern to match memory addresses (M(X))
    if (regcomp(&regex, "M\\([^)]*\\)", REG_EXTENDED) != 0)
    {
        fprintf(stderr, "Error compiling regular expression\n");
        exit(EXIT_FAILURE);
    }

    // Use regexec to find and replace memory addresses in the line
    if (regexec(&regex, line, 1, &match, 0) == 0)
    {
        char address[10] = {'\0'};
        extract_address(line, address);

        // Use memmove to replace the matched memory address with "M()"
        memmove(&line[match.rm_so], "M()", strlen("M()"));
        memmove(&line[match.rm_so + strlen("M()")], &line[match.rm_eo], strlen(&line[match.rm_eo]) + 1);

        // concatenate the memory address to the end of the line separeted by a semicolon
        if (strlen(address) > 0)
        {
            line[strlen(line) - 1] = '\0';
            strcat(line, ";");
            strcat(line, address);
            strcat(line, "\n");
        }
    }

    // Free the compiled regular expression
    regfree(&regex);
}

int main()
{
    FILE *input_file, *output_file;
    char line[MAX_LINE_LENGTH];

    // Open input file for reading
    if ((input_file = fopen("input.txt", "r")) == NULL)
    {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    // Open output file for writing
    if ((output_file = fopen("output.txt", "w")) == NULL)
    {
        perror("Error opening output file");
        fclose(input_file);
        exit(EXIT_FAILURE);
    }

    // Read lines from the input file, remove memory addresses, and write to the output file
    while (fgets(line, MAX_LINE_LENGTH, input_file) != NULL)
    {
        remove_memory_address(line);
        fprintf(output_file, "%s", line);
    }

    // Close files
    fclose(input_file);
    fclose(output_file);

    return 0;
}
