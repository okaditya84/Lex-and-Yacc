#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function to check if a token is a C keyword
int isKeyword(char *token)
{
    // List of C keywords
    char keywords[][10] = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};

    // Loop through keywords to check if the token is a keyword
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
    {
        if (strcmp(token, keywords[i]) == 0)
        {
            return 1; // It's a keyword
        }
    }
    return 0;
}

int main()
{
    // Declare variables
    FILE *file;
    char filename[50];
    char token[50];

    printf("Enter the name of the input file: ");
    scanf("%s", filename);

    file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("File not found or could not be opened.\n");
        return 1;
    }

    printf("Identifying tokens:\n");

    // Loop through the file, reading tokens
    while (fscanf(file, "%s", token) != EOF)
    {

        if (isKeyword(token))
        {
            printf("Keyword: %s\n", token);
        }
        else
        {
            int i = 0;
            int isIdentifier = 1;
            int isNumber = 1;

            // Check if the token is an identifier or a number
            while (token[i])
            {
                if (!isalpha(token[i]))
                {
                    isIdentifier = 0;
                }
                if (!isdigit(token[i]) && token[i] != '.')
                {
                    isNumber = 0;
                }
                i++;
            }

            // Print the appropriate type of token
            if (isIdentifier)
            {
                printf("Identifier: %s\n", token);
            }
            else if (isNumber)
            {
                printf("Number: %s\n", token);
            }
            else
            {
                printf("Operator or Other: %s\n", token);
            }
        }
    }
    fclose(file);
    return 0;
}
