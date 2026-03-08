%{
    #include <string.h>
    #include <stdio.h>
	#include "symbolTable.h"
	#include "codeGeneration.h"

    void yyerror(char*);
    int yylex();

    extern SymTable table;
	extern SymFunc* Main;
	extern SymFunc* Root;
	extern int flagLocal;
	extern SymTable tableStr;
    char s_decs[256];

%}

%union {
	struct code_t
	{
		char str[2044]; // string para o codigo asm
		int op; // opcoes (por exemplo nos jumps)
	} c;
}


%type <c> programa declaracoes declaracao bloco
%type <c> declaracao_inteiro declaracao_float declaracao_string
%type <c> comandos comando comando_escrita comando_leitura comando_atribuicao
%type <c> expressao_numerica termo fator
%type <c> expressao_booleana operador_relacional
%type <c> comando_se comando_se_senao comando_enquanto


%token <c> VAR ID NUM LITERAL_STR INT FLOAT WRITE READ WRITELN IF THEN ELSE WHILE DO STRING
%token <c> LE GE EQ NE

%left '+' '-'
%left '*' '/'


%%


programa: declaracoes bloco {
		dumpCodeDeclarationEnd();
		fprintf(out_file, "%s", $2.str);
		
		MakeInitImport(out_file);

		makeCodeDeclarationVar(out_file);

		//fprintf(out_file, "%s", $1.str);
	}
;

// programa: protos declaracoes bloco funcoes {
// 		dumpCodeDeclarationEnd();
// 		fprintf(out_file, "%s", $2.str);
// 		makeCodeDeclarationVar(out_file);
	
// 		//fprintf(out_file, "%s", $1.str);
// 	}
// ;

// protos: proto protos {
// }
// 	| %empty { $$.str[0] = '\0'; }
// ;

// proto: ID '(' ')' ';' {
// 	Main = FuncAdd(Main);
// 	Main->label = ($1.str) ;
// }

// funcoes: funcao funcoes {
// 	//funcao
// }
// 	| %empty { $$.str[0] = '\0'; }
// ;

// funcao: 'void' ID '(' ')' bloco {
// 	Main = Root;
// 	while (Main->label!= $2.str || Main != NULL){
// 		Main = Main->prox;
// 	} if(Main == NULL){
// 		printf("função nao existente");
// 	}
// }



declaracoes: declaracao declaracoes  {
		if(flagLocal == 0){
			Main = FuncAdd(Main);
		}
		flagLocal = 1;

		strcpy($$.str, $1.str);
		
		sprintf($$.str + strlen($$.str), "%s", $2.str);
	}

	| %empty { $$.str[0] = '\0'; }
;


declaracao: declaracao_inteiro { 
	
	strcpy($$.str, $1.str); }
	| declaracao_float { strcpy($$.str, $1.str); }
	| declaracao_string {strcpy($$.str, $1.str); }
;


declaracao_inteiro: INT ID '=' NUM ';'  {
		strcpy($$.str, "\0");
		if(flagLocal == 1){
			if(addSymTable(&Main->tabela, $2.str, INTEGER, $4.str) == 0){
				YYABORT;
			}
		}else{
			if(addSymTable(&Root->tabela, $2.str, INTEGER, $4.str) == 0){
				YYABORT;
			}
		}
		
	}
	| INT ID ';'  {
		
		strcpy($$.str, "\0");
		
		if(flagLocal == 1){
			if(addSymTable(&Main->tabela, $2.str, INTEGER, NULL) == 0){
				YYABORT;
			}
		}else{
			if(addSymTable(&Root->tabela, $2.str, INTEGER, NULL) == 0){
				YYABORT;
			}
		}
	}
;


declaracao_float: FLOAT ID '=' NUM ';'  {
		strcpy($$.str, "\0");
		
		//makeCodeDeclaration($$.str, $2.str, REAL, $4.str);

		if(flagLocal == 1){
			if(addSymTable(&Main->tabela, $2.str, REAL, NULL)== 0){
				YYABORT;
			}
		}else{
			if(addSymTable(&Root->tabela, $2.str, REAL, NULL)== 0){
				YYABORT;
			}
		}
	}
	| FLOAT ID ';'  {
		strcpy($$.str, "\0");
		
		//makeCodeDeclaration($$.str, $2.str, REAL, NULL);

		if(flagLocal == 1){
			if(addSymTable(&Main->tabela, $2.str, REAL, NULL)== 0){
				YYABORT;
			}
		}else{
			if(addSymTable(&Root->tabela, $2.str, REAL, NULL)== 0){
				YYABORT;
			}
		}
	}
;

declaracao_string: STRING ID '=' LITERAL_STR ';'  {
        strcpy($$.str, "\0");
		if(flagLocal == 1){
            if(addSymTable(&Main->tabela, $2.str, STR, $4.str)== 0){
				YYABORT;
			}
        } else {
            if(addSymTable(&Root->tabela, $2.str, STR, $4.str)== 0){
				YYABORT;
			}
        }
    }
    | STRING ID ';'  {
		strcpy($$.str, "\0");
        if(flagLocal == 1){    
            if(addSymTable(&Main->tabela, $2.str, STR, NULL)== 0){
				YYABORT;
			}
        } else {
            if(addSymTable(&Root->tabela, $2.str, STR, NULL)== 0){
				YYABORT;
			}
        }

    }
;


bloco : '{' comandos '}'  {
		strcpy($$.str, $2.str);
	}
;



comandos : comando comandos  {

       	strcpy($$.str, $1.str);
		sprintf($$.str + strlen($$.str), "%s", $2.str);
	}

	| %empty { $$.str[0] = '\0'; }
;


comando: comando_escrita      { strcpy($$.str, $1.str); }
	| comando_leitura         { strcpy($$.str, $1.str); }
	| comando_atribuicao      { strcpy($$.str, $1.str); }
	| comando_se              { strcpy($$.str, $1.str); }
	| comando_se_senao        { strcpy($$.str, $1.str); }
	| comando_enquanto        { strcpy($$.str, $1.str); }
	| declaracao			  { strcpy($$.str, $1.str); }
//	| funcaoDec				  { strcpy($$.str, $1.str); }
;

// funcaoDec: ID '(' ')' ';' {
// 	makeLabel();
// }


comando_leitura: READ '(' ID ')' ';'  {
		
		if (!makeCodeRead($$.str, $3.str))
			YYABORT;
	}
;


comando_escrita: WRITE '(' ID ')' ';'  {
		//strcpy($$.str, "\0");
		if (!makeCodeWrite($$.str, $3.str, 0))
			YYABORT;
	}

	| WRITE '(' LITERAL_STR ')'  ';'  {
		
		if (!makeCodeWriteLiteral($$.str, makeLabelStr($3.str)))
			YYABORT;
	}
;


comando_atribuicao: ID '=' expressao_numerica ';'  {
		
		if (!makeCodeAssignment($$.str, $1.str, $3.str))
			YYABORT;
	}| ID '=' LITERAL_STR ';' {
		if (!makeCodeAssignmentSTR($$.str, makeLabelStr($3.str)))
			YYABORT;
	} 
;


expressao_numerica: termo  {

		strcpy($$.str, $1.str);
	}

	| expressao_numerica '+' expressao_numerica  {

		makeCodeAdd($$.str, $3.str);
	}

	| expressao_numerica '-' expressao_numerica  {
		
		makeCodeSub($$.str, $3.str);
	}

	| termo '*' fator  {
		makeCodeMul($1.str, $3.str);
		strcpy($$.str, $1.str);

	}

	| termo '/' fator  {

		makeCodeDiv($$.str, $3.str);
	}

	| termo '%' fator  {

		makeCodeMod($$.str, $3.str);
	}


;



termo: NUM  {

		makeCodeLoad($$.str, $1.str, 0);
	}

	| ID  {

		if (!makeCodeLoad($$.str, $1.str, 1))
			YYABORT;

	}
;


fator: NUM  {
		
		makeCodeLoad($$.str, $1.str, 0);
	}

	| ID  {

		makeCodeLoad($$.str, $1.str, 1);
	}
	
	| '(' expressao_numerica ')'  {
		
		strcpy($$.str, $2.str);
	}
;




comando_se: IF '(' expressao_booleana ')' bloco  {
		makeCodeIf($$.str, $3.str, $3.op, $5.str);
	}
;


comando_se_senao: IF '(' expressao_booleana ')' bloco ELSE bloco  {

		makeCodeIfElse($$.str, $3.str, $3.op, $5.str, $7.str);
	}
;


comando_enquanto: WHILE '(' expressao_booleana ')' bloco  {

		makeCodeWhile($$.str, $3.str, $3.op, $5.str);
	}
;


expressao_booleana: ID operador_relacional expressao_numerica  {
		
		$$.op = $2.op;
		if (!makeCodeComp($$.str, $1.str, $3.str))
			YYABORT;
	}
;

operador_relacional: '<'   { $$.op = -4; }
	| '>'                  { $$.op = -3; }
	| LE                   { $$.op = 3; }
	| GE                   { $$.op = 4; }
	| EQ                   { $$.op = -2; }
	| NE                   { $$.op = 2; }
;



%%

void yyerror(char *s) {
   fprintf(stderr, "Error Sintatico: %s at line %d", s, cont_lines);
   fprintf(stderr, "\n");
}
