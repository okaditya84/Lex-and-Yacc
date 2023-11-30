%{
#include <stdio.h>
#include <math.h>
int yylex(void);
int yyerror(char *s);
%}

%token NUMBER
%right '^'

%%
statement: E { printf("Result: %d\n", $1); }
| statement E { printf("Result: %d\n", $2); }
;
E : E '+' T { $$ = $1 + $3; }
| E '-' T { $$ = $1 - $3; }
| T { $$ = $1; }
;
T : T '*' F { $$ = $1 * $3; }
| T '/' F { if ($3 == 0) yyerror("division by zero"); else $$ = $1 / $3; }
| F { $$ = $1; }
;
F : P '^' F { $$ = pow($1, $3); }
| P { $$ = $1; }
;
P : '(' E ')' { $$ = $2; }
| NUMBER { $$ = $1; }
;
%%

int main() {
    yyparse();
}

int yyerror(char *s){
    fprintf(stderr,"%s\n",s);
    return 0;
}