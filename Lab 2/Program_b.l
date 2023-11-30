%option noyywrap
%{
    int c1 = 0, c2 = 0, c3 = 0, c4 = 0,c5=0;
%}
%%
auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while {printf("The length of keyword %s: %d \n", yytext, yyleng); c1++;}
[a-zA-Z]([a-zA-Z_]|[0-9])* {printf("The length of identifier %s is: %d \n", yytext, yyleng); c2++;}
[0-9]+ {printf("The length of digit %s is: %d\n", yytext, yyleng); c3++;}
[+*/]+ {printf("The length of operator %s is: %d\n", yytext, yyleng); c5++;}
. {printf("The length of Other %s is: %d\n", yytext, yyleng); c4++;}
%%

int main() {
    yylex();
    printf("Total number of tokens: %d \nkeywords: %d, identifiers: %d, digits: %d ,others: %d\n", c1+c2+c3+c4, c1, c2, c3, c4);
    return 0;
}
