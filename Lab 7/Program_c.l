%{
#include "Program_c.tab.h"

%}
%%
[0-9]+ { yylval = atoi(yytext); return NUMBER; }
[ \t] ; /* Ignore whitespace */
\n return 0; /* Logical EOF */
. return yytext[0];
%%
int yywrap() {
return 1;
}