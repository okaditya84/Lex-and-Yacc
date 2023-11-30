%option noyywrap
%{
    #include <stdio.h>
    #include "Program_a.tab.h"
    extern int yylval;
%}

%%
[0-9]+      { yylval = atoi(yytext);  return num; }
[ \t] ;           /* ignore whitespace */
\n    return 0;   /* logical EOF */
.     return yytext[0];
%%


