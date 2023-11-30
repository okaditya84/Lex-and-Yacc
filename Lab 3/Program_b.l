%option noyywrap
%{
#include<stdio.h>
int charCount = 0;
int wordCount = 0;
int lineCount = 0;
int inWord = 0;
%}

%%
\n {
    charCount++;
    if (inWord) {
        wordCount++;
        inWord = 0;
    }
    lineCount++;
}

[ \t]+ {
    if (inWord)
    {
        wordCount++;
        inWord = 0;
    }
}

[a-zA-Z]+ {
    charCount += yyleng;
    inWord = 1;
}

. {
    charCount++;
}
%%

int main()
{
    FILE* input = fopen("HarryPotter.txt","r");
    if (!input) {
        fprintf(stderr, "Error opening input file.\n");
    }

    yyin = input;
    yylex();

    fclose(input);

    printf("Character count: %d\n", charCount);
    printf("Word count: %d\n", wordCount);
    printf("Line count: %d\n", lineCount+1);
}
