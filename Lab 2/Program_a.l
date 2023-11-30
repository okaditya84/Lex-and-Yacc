%option noyywrap
%{
    int count = 0;
%}
%%
[^\n \t]+    {printf("%s is Token having length = %d\n",yytext,yyleng);count++;}  
\n  {printf("No. of tokens generated are: %d\n",count);}
.   ; 
%%
int main() 
{
    yylex();
}
