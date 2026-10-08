Script started on Wednesday 07 October 2026 05:45:15 PM IST
[24bcs059@mepcolinux ex5]$cat cal.l
%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"
%}

%option noyywrap

%%

[ \t]+              ;

[0-9]+              { yylval.ival = atoi(yytext); return INT; }
[0-9]+\.[0-9]*      { yylval.dval = atof(yytext); return DOUBLE; }
\.[0-9]+            { yylval.dval = atof(yytext); return DOUBLE; }

"quit"|"exit"       { return QUIT; }
"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return MUL; }
"/"                 { return DIV; }
"^"                 { return POW; }
"("                 { return LPAREN; }
")"                 { return RPAREN; }
"\n"                { return NEWLINE; }

.                   { return yytext[0]; }

%%

[24bcs059@mepcolinux ex5]$cat cal.y
%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int yylex(void);
void yyerror(const char *s);

%}

%union {
    int    ival;
    double dval;
}

%token <ival> INT
%token <dval> DOUBLE
%token PLUS MINUS MUL DIV POW
%token LPAREN RPAREN
%token NEWLINE QUIT

%type <dval> EXP

%left  PLUS MINUS
%left  MUL DIV
%right POW
%right UMINUS

%%

S:
      /* empty */
    | S line
    ;

line:
      NEWLINE
        { printf("> "); }
    | EXP NEWLINE
        {
            printf("Result: %.6f\n", $1);
            printf("> ");
        }
    | QUIT NEWLINE
        { printf("Exiting...\n"); exit(0); }
    | error NEWLINE
        { yyerrok; printf("> "); }
    ;

EXP:
      INT
        { $$ = (double)$1; }

    | DOUBLE
        { $$ = $1; }

    | EXP PLUS EXP
        { $$ = $1 + $3; }

    | EXP MINUS EXP
        { $$ = $1 - $3; }

    | EXP MUL EXP
        { $$ = $1 * $3; }

    | EXP DIV EXP
        {
            if ($3 == 0.0) {
                yyerror("Division by zero");
                $$ = 0.0;
            } else {
                $$ = $1 / $3;
            }
        }

    | EXP POW EXP
        { $$ = pow($1, $3); }

    | MINUS EXP %prec UMINUS
        { $$ = -$2; }

    | LPAREN EXP RPAREN
        { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(void) {

    printf("Type 'quit' to exit\nEnter any Expression:");
    yyparse();
    return 0;
}


[24bcs059@mepcolinux ex5]$./cal
Type 'quit' to exit
Enter any Expression:10+5
Result: 15.000000
> quit
Exiting...
[24bcs059@mepcolinux ex5]$./cal
Type 'quit' to exit
Enter any Expression:10-5
Result: 5.000000
> quit
Exiting...
[24bcs059@mepcolinux ex5]$./cal
Type 'quit' to exit
Enter any Expression:10*5
Result: 50.000000
> quit
Exiting...
[24bcs059@mepcolinux ex5]$./cal
Type 'quit' to exit
Enter any Expression:10/5
Result: 2.000000
> quit
Exiting...
[24bcs059@mepcolinux ex5]$./cal
Type 'quit' to exit
Enter any Expression:10^2*4
Result: 400.000000
> quit
Exiting...
[24bcs059@mepcolinux ex5]$exit
exit

Script done on Wednesday 07 October 2026 05:46:43 PM IST
[24bcs059@mepcolinux ex5]$
