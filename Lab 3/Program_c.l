%option noyywrap
%{
#include <stdio.h>
%}

%%
[0-9]+ {
    int num = atoi(yytext); // Convert matched text to an integer
    if (num % 7 == 0) {
        num += 3;
    }
    printf("%d sh", num);
}

.|\n {
    printf("%s", yytext); // Print non-matching characters as they are
}
%%

int main() {
    yylex();
    return 0;
}
