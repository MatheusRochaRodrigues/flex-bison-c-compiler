
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"
#include "sintatico.tab.h"


FILE* out_file = NULL;

SymFunc* Main;
SymFunc* Root;

SymTable tableStr;
SymTable table;
int flagLocal = 0;

// SymTable* tableCurrent = &table;

char jumps[10][4] = {"jnl", "jng", "jne", "jnz", "", "jz", "je", "jg", "jl", "jmp"};

int cont_lines = 1;


int main(int argc, char const *argv[])
{
    initSymTable(&tableStr);
    Main = (SymFunc*)malloc(sizeof(SymFunc));
    
    initFunc(Main);
    //FuncAdd(Main);
    
    //printf("\newedsadasds MAIN %d\n", Main->prox);
    Root = Main;
    
    if (argc < 2)
    {
        printf("comp: nenhum arquivo de entrada\n");
        return 1;
    }

    int n = strlen(argv[1]);
    char s[n + 10];

    int i;
    for (i = 0; i < (n + 10); i++)
        s[i] = '\0';
    
    if (argc >= 4 && strcmp(argv[2], "-o") == 0)
        strcpy(s, argv[3]);
    
    else
    {
        for (i = n - 1; i >= 0 && argv[1][i] != '.'; i--){}

        if (i == -1)
        {
            fprintf(stderr, "[ERRO]: Extensao do arquivo de entrada incorreta\n");
            return 0;
        }

        strncpy(s, argv[1], i);
        strcat(s, ".asm");
    }
    
    out_file = fopen(s, "w");
    fprintf(out_file, "extern printf\n");
	fprintf(out_file, "extern scanf\n");
    
    
    
    //initSymTable(&table);


    FILE* teclado = stdin;
    stdin = fopen(argv[1], "r");
    
    int ret = yyparse(); // chamada do analisador sintatico
    if (ret == 1)
    {
        fclose(out_file);
        remove(s);
        return 1;
    }  

    

    int t = 2;
    //SymFunc* aux = Main;
    while (Main != NULL)
    {   
        printf("\n\nDebug Funcs #%d\n", t);
        /* code */
        printSymTable(&Main->tabela);
        Main = Main->ant;
        t--;
    }
    
    

    fclose(out_file);
    fclose(stdin);

    return 0;
    
}

