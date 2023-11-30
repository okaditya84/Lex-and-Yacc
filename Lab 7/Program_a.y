%{
#include <stdio.h>
#include <math.h>
int yylex(void);
int yyerror(char *s);
%}

%token NAME num
%left '+' '-'
%left '*' '/'
%right '^'
%%

S: E      { printf("Result: %d\n", $1); }
  | S E    { printf("Result: %d\n", $2); }
  ;

E: E '+' E   { $$ = $1 + $3; } | E '-' E   { $$ = $1 - $3; } | E '*' E   { $$ = $1 * $3; }
| E '/' E   { 
                if ($3 == 0) {
                    yyerror("Division by zero");
                    $$ = 0; // Assign a default value or handle as needed
                } else {
                    $$ = $1 / $3;
                }
              }
  | E '^' E   { $$ = pow($1, $3); }
  | '(' E ')' { $$ = $2; }
  | num       { $$ = $1; }
  ;

%%

int main(){
    yyparse();
}

int yyerror(char *s){
    fprintf(stderr,"%s\n",s);
    // You might want to perform error handling or recovery here
    return 0;
}
