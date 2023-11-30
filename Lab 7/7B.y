%{
#include <stdio.h>
#include <math.h>
void yyerror(char *s);
int yylex();
%}

%token NAME num
%left '+' '-'
%left '*' '/'
%right '^'
%nonassoc UMINUS
%%
s: NAME '=' Ex
    | Ex { printf("= %d\n", $1); }
    ;
Ex: Ex '+' Ex {$$ = $1 + $3;}
| Ex '-' Ex {$$ = $1 - $3;}
| Ex '*' Ex {$$ = $1 * $3;}
| Ex '/' Ex {if($3 == 0){
                yyerror("error");
                return 1;
            }
            else
                $$ = $1 / $3;
            }
| Ex '^' Ex {$$ = pow($1,$3);}
| '-' Ex %prec UMINUS {$$ = -$2;}
| '(' Ex ')' {$$ = $2;}
| num {$$ = $1;}
;
%%
int main() {
    yyparse();
    return 0;
}
void yyerror(char *s) {
    printf("error");
}

