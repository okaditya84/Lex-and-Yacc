%option noyywrap
%{
    #include "7B.tab.h"
    extern int yyval;
    extern void yyerror(char *s);
%}

%%

[0-9]+ { yylval = atoi(yytext); return num; }
[ \t] ; /* Ignore whitespace */
\n return 0; /* Logical EOF */
. return yytext[0];

%%
