#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_SIZE_HASH_ARRAY 20
#define MAX_SIZE_SYMBOL 32

typedef enum {INTEGER, REAL, STR} Type;

extern int cont_lines;

//Declaracao de tipo de dados
struct symTableEntry {
    char identifier[MAX_SIZE_SYMBOL];
    Type type;
    char* value;
    int len_value;
};
typedef struct symTableEntry SymTableEntry;


//Tipo de dado para definicao do no
struct symTableNode {
	SymTableEntry data;
	struct symTableNode *next;
};
typedef struct symTableNode SymTableNode;


struct symTable {
	SymTableNode* array;
	int max_size;
	int size;

	// SymTable* prox;
};
typedef struct symTable SymTable;



//			FUNCOES
struct symFunc {
	SymTable tabela;
	struct symFunc* ant;
	struct symFunc* prox;
	char* label[30];
	int created;
};
typedef struct symFunc SymFunc;

SymFunc* FuncAdd(SymFunc* funcao);
void initFunc(SymFunc* funcao);


extern SymTable table;
extern SymFunc* Main;
extern SymFunc* Root;
extern SymTable tableStr;

//Prototipos das funcoes
int initSymTable(SymTable* table);
void printSymTable(SymTable* table);
int addSymTable(SymTable* table, char* identifier, Type type, char* value);
SymTableEntry* findSymTable(SymTable* table, char* identifier);
void freeSymTable(SymTable* table);

// int CreateSymTable(SymTable* table, SymTable* tableOld);

// int retiraValor( SymTableNode tabHash[],int valor,int qualHash);
// void liberaTabela(SymTableNode* tab);

#endif