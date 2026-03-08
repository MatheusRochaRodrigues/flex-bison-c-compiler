

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"
#include "codeGeneration.h"


void MakeInitImport(FILE* out_file){
    fprintf(out_file, "\nmov rax,0\n");  
    fprintf(out_file, "ret\n\n");

    
    fprintf(out_file, "section .data\n");

    // this are strings without a line break character
    fprintf(out_file, "fmt_d: db \"%%ld\", 0\n");
    fprintf(out_file, "fmt_f: db \"%%f\", 0\n");
    fprintf(out_file, "fmt_s: db \"%%s\", 0\n");
    

    // this are strings that contain a line break character
    fprintf(out_file, "fmt_dln: db \"%%ld\", 10, 0\n");
    fprintf(out_file, "fmt_fln: db \"%%f\", 10, 0\n");
    fprintf(out_file, "fmt_sln: db \"%%s\", 10, 0\n");
    fprintf(out_file, "\n");
}

// Labels para os jumps
void makeLabel(char* out_label)
{
    static int label_count = 0;
    strcpy(out_label, "label");
    
    char s[10];
    sprintf(s, "%d", label_count);
    strcat(out_label, s);

    label_count++;
}


void makeLabelFunc(char* out_label, char*name)
{
    strcpy(out_label, "label");
    

    strcat(out_label, name);
}

void makeCodeDeclarationVar(FILE* out_file)
{
    char* buff = (char*)malloc(1024 * sizeof(char));
    char* auxB = (char*)malloc(1024 * sizeof(char));

    int i = 0;
	int cont = 0;
	//printf("\n\tdado   %s\n", file);
    while (i < Main->tabela.max_size) {
		// Se nao tiver nada simplesmente imprime vazio
        SymTableNode *aux = &Main->tabela.array[i];
        
        if (Main->tabela.array[i].data.identifier[0] != '\0'){
            while (aux != NULL) {
                
                //printf("\n\tdsfdsfsd 1VEZ   %s\n", aux->data.identifier);
                makeCodeDeclaration(auxB, aux->data.identifier, aux->data.type, aux->data.value);
                strcat(buff, auxB);
                aux = aux->next;
                
            }
        }
		i++;
	}

    i = 0;
	cont = 0;
    while (i < Root->tabela.max_size) {
		// Se nao tiver nada simplesmente imprime vazio
        SymTableNode *aux = &Root->tabela.array[i];
        
        if (Root->tabela.array[i].data.identifier[0] != '\0'){
            while (aux != NULL) {
                
                //printf("\n\tdsfdsfsd 1VEZ   %s\n", aux->data.identifier);
                makeCodeDeclaration(auxB, aux->data.identifier, aux->data.type, aux->data.value);
                strcat(buff, auxB);
                aux = aux->next;
                
            }
        }
		i++;
	}


    //STR CONSTANT
    strcat(buff, "\n");
    i = 0;
	cont = 0;
    while (i < tableStr.max_size) {
		// Se nao tiver nada simplesmente imprime vazio
        SymTableNode *aux = &tableStr.array[i];
        
        if (tableStr.array[i].data.identifier[0] != '\0'){
            while (aux != NULL) {
                
                //printf("\n\tdsfdsfsd 1VEZ   %s\n", aux->data.identifier);
                makeCodeDeclaration(auxB, aux->data.identifier, aux->data.type, aux->data.value);
                strcat(buff, auxB);
                aux = aux->next;
                
            }
        }
		i++;
	}



    fprintf(out_file, "\n%s", buff);
 
}

// Codigo para declaracao de variaveis
void makeCodeDeclaration(char* dest, char* identifier, Type type, char* value)
{
    if (type == INTEGER)
    {
        if (value == NULL)
        	sprintf(dest, "%s: dq 0\n", identifier);

        else
        {
        	int x = atoi(value);
            sprintf(dest, "%s: dq %d\n", identifier, x);
        }
    }

    else if (type == REAL)
    {
    	if (value == NULL)
            sprintf(dest, "%s: dq 0\n", identifier);

        else
        {
          	double x = atof(value);
            sprintf(dest, "%s: dq %f\n", identifier, x);
        }
    }

    else if (type == STR)
    {
    	if (value == NULL)
            sprintf(dest, "%s: times %d db 0 \n", identifier, STRING_SIZE);

        else
        {
            sprintf(dest, "%s: db %s, 0\n", identifier, value);
        }
    }
}


// Termino da secao de dados e comeco da secao de codigo
void dumpCodeDeclarationEnd()
{
    fprintf(out_file, "\nsection .text\n");
    fprintf(out_file, "global main\n");
    fprintf(out_file, "\nmain:\n");
}


// Codigo para leitura (scanf)
int makeCodeRead(char* dest, char *id)
{
    SymTableEntry* ret = findSymTable(&Main->tabela,id);
    
    dest[0] = '\0';

    if (ret == NULL)
    {
        if((ret = findSymTable(&Root->tabela,id)) == NULL)
        {
            fprintf(stderr, "Error: %s not recognized at line %d\n", id, cont_lines);
            return 0;
        }
    }

    if (ret->type == INTEGER)
    {
        sprintf(dest + strlen(dest), "mov rdi,fmt_d\n");
        sprintf(dest + strlen(dest), "mov rsi,%s\n", ret->identifier);
    }

    else if (ret->type == REAL)
    {
        sprintf(dest + strlen(dest), "mov rdi,fmt_f\n");
        sprintf(dest + strlen(dest), "mov rsi,%s\n", ret->identifier);
    }

    else if (ret->type == STR)
    {
        sprintf(dest + strlen(dest), "mov rdi,fmt_s\n");
        sprintf(dest + strlen(dest), "mov rsi,%s\n", ret->identifier);
    }

    // else
    // {
    //     sprintf(dest + strlen(dest), "mov rdi,fmt_s\n");
    //     sprintf(dest + strlen(dest), "mov rsi,%s\n", ret->identifier);
    // }

   
    sprintf(dest + strlen(dest), "mov rax,0\n");
    sprintf(dest + strlen(dest), "call scanf\n");

    return 1;

}


// Codigo para escrita (printf)
int makeCodeWrite(char* dest, char *id, int ln)
{
    SymTableEntry* ret = findSymTable(&Main->tabela,id);
    
    dest[0] = '\0';

    if (ret == NULL)
    {
        if((ret = findSymTable(&Root->tabela,id)) == NULL)
        {
            fprintf(stderr, "Error: %s not recognized at line %d\n", id, cont_lines);
            return 0;
        }
    }

    if (ret->type == INTEGER)
    {
        if (ln) sprintf(dest + strlen(dest), "mov rdi,fmt_dln\n");
        else sprintf(dest + strlen(dest), "mov rdi,fmt_d\n");
        sprintf(dest + strlen(dest), "mov rsi,[%s]\n", ret->identifier);
    }

    else if (ret->type == REAL)
    {
        if (ln) sprintf(dest + strlen(dest), "mov rdi,fmt_fln\n");
        else sprintf(dest + strlen(dest), "mov rdi,fmt_f\n");
        sprintf(dest + strlen(dest), "mov rsi,[%s]\n", ret->identifier);
    }
    else if (ret->type == STR)
    {
        if (ln) sprintf(dest + strlen(dest), "mov rdi,fmt_fln\n");
        else sprintf(dest + strlen(dest), "mov rdi,fmt_f\n");
        sprintf(dest + strlen(dest), "mov rsi,[%s]\n", ret->identifier);
    }

    else
    {
        if (ln) sprintf(dest + strlen(dest), "mov rdi,fmt_sln\n");
        else sprintf(dest + strlen(dest), "mov rdi,fmt_s\n");
        sprintf(dest + strlen(dest), "mov rsi,%s\n", ret->identifier);
    }

    sprintf(dest + strlen(dest), "mov rax,0\n");
    sprintf(dest + strlen(dest), "call printf\n");

    return 1;
}

int makeCodeWriteLiteral(char* dest, char *id)
{
    dest[0] = '\0';
    sprintf(dest + strlen(dest), "mov rax, %s\n", id);
    sprintf(dest + strlen(dest), "call printf\n");

    return 1;
}


char* makeLabelStr(char* out_label)
{
    char* aux = (char*)malloc(sizeof(char)* 1024);
    strcpy(aux, "fmt_s");
    static int label_count = 0;
    
    char s[10];
    sprintf(s, "%d", label_count);
    strcat(aux, s);

    addSymTable(&tableStr, aux, STR, out_label);

    label_count++;
    return aux;
}



int makeCodeAssignment(char* dest, char* id, char* expr)
{   
    SymTableEntry* ret = findSymTable(&Main->tabela, id);
    dest[0] = '\0';

    if (ret == NULL)
    {
        if((ret = findSymTable(&Root->tabela,id)) == NULL)
        {
            fprintf(stderr, "Error: %s not recognized at line %d\n", id, cont_lines);
            return 0;
        }
    }

 
    if (ret->type == INTEGER)
    {
        sprintf(dest + strlen(dest), "%s", expr);
        sprintf(dest + strlen(dest), "pop rbx\n");
        sprintf(dest + strlen(dest), "mov [%s],rbx\n", ret->identifier);
    }

    else if (ret->type == REAL)
    {
        sprintf(dest + strlen(dest), "%s", expr);
        sprintf(dest + strlen(dest), "pop rbx\n");
        sprintf(dest + strlen(dest), "mov [%s],rbx\n", ret->identifier);
    }
    else if (ret->type == STR)
    {
        sprintf(dest + strlen(dest), "%s", expr);
        sprintf(dest + strlen(dest), "pop rbx\n");
        sprintf(dest + strlen(dest), "mov [%s],rbx\n", ret->identifier);
    }
    else
    {
        fprintf(stderr, "Unsuported operation envolving string or float at line %d\n",
            cont_lines);
        return 0;
    }

    return 1;
}


int makeCodeAssignmentSTR(char* dest, char* id)
{   
    dest[0] = '\0';
    sprintf(dest + strlen(dest), "pop rbx\n");
    sprintf(dest + strlen(dest), "mov [%s],rbx\n", id);

    return 1;
}



int makeCodeLoad(char* dest, char* id, int ref)
{
    dest[0] = '\0';

    if (ref == 0)
    {
        sprintf(dest + strlen(dest), "mov rbx,%s\n", id);
        sprintf(dest + strlen(dest), "push rbx\n");
        return 1;
    }

    SymTableEntry* ret = findSymTable(&Main->tabela, id);

    if (ret == NULL)
    {
        if((ret = findSymTable(&Root->tabela,id)) == NULL)
        {
            fprintf(stderr, "Error: %s not recognized at line %d\n", id, cont_lines);
            return 0;
        }
    }

    if (ret->type == STR)
        sprintf(dest + strlen(dest), "mov rbx,%s\n", ret->identifier);
    else
        sprintf(dest + strlen(dest), "mov rbx,[%s]\n", ret->identifier);
    
    sprintf(dest + strlen(dest), "push rbx\n");
    return 1;
}


void makeCodeAdd(char* dest, char* value)
{
    sprintf(dest + strlen(dest), "%s", value);
    sprintf(dest + strlen(dest), "pop rcx\n");
    sprintf(dest + strlen(dest), "pop rbx\n");
    sprintf(dest + strlen(dest), "add rbx,rcx\n");
    sprintf(dest + strlen(dest), "push rbx\n");
}


void makeCodeSub(char* dest, char* value)
{   
    sprintf(dest + strlen(dest), "%s", value);
    sprintf(dest + strlen(dest), "pop rcx\n");
    sprintf(dest + strlen(dest), "pop rbx\n");
    sprintf(dest + strlen(dest), "sub rbx,rcx\n");
    sprintf(dest + strlen(dest), "push rbx\n");

}

void makeCodeMul(char* dest, char* value2)
{
    sprintf(dest + strlen(dest), "%s", value2);
    sprintf(dest + strlen(dest), "pop rcx\npop rbx\nimul rbx,rcx\npush rbx\n");
}


void makeCodeDiv(char* dest, char* value2)
{
    sprintf(dest + strlen(dest), "%s", value2);
    sprintf(dest + strlen(dest), "pop r8\n");
    sprintf(dest + strlen(dest), "pop rax\n");
    sprintf(dest + strlen(dest), "xor rdx,rdx\n");
    sprintf(dest + strlen(dest), "idiv r8\n");
    sprintf(dest + strlen(dest), "push rax\n");
}


void makeCodeMod(char* dest, char* value2)
{
    sprintf(dest + strlen(dest), "%s", value2);
    sprintf(dest + strlen(dest), "pop r8\n");
    sprintf(dest + strlen(dest), "pop rax\n");
    sprintf(dest + strlen(dest), "xor rdx,rdx\n");
    sprintf(dest + strlen(dest), "idiv r8\n");
    sprintf(dest + strlen(dest), "push rdx\n");
}


int makeCodeComp(char* dest, char* id, char* expr)
{
    SymTableEntry* ret = findSymTable(&Main->tabela, id);
    dest[0] = '\0';

    if (ret == NULL)
    {
        if((ret = findSymTable(&Root->tabela,id)) == NULL)
        {
            fprintf(stderr, "Error: %s not recognized at line %d\n", id, cont_lines);
            return 0;
        }
    }
    // int a = ret->type;
    // printf("\n%d %d type\n ", ret->type, REAL);

    if (ret->type != INTEGER && ret->type != REAL)
    {
        fprintf(stderr, "Unsuported operation envolving string or float at line %d\n",
            cont_lines);
        return 0;
    }

    sprintf(dest + strlen(dest), "%s", expr);
    sprintf(dest + strlen(dest), "pop rcx\n");
    sprintf(dest + strlen(dest), "mov rbx, [%s]\n", ret->identifier);
    sprintf(dest + strlen(dest), "cmp rbx, rcx\n");

    return 1;
}


void makeCodeIf(char* dest, char* expr_code, int expr_jump, char* block_code)
{
    char label[16];
    makeLabel(label);

    dest[0] = '\0';

    sprintf(dest + strlen(dest), "%s", expr_code);
    sprintf(dest + strlen(dest), "%s %s\n", jumps[expr_jump + JUMPS_ARRAY_OFFSET],
        label);
    sprintf(dest + strlen(dest), "%s", block_code);
    sprintf(dest + strlen(dest), "%s:\n", label);
}


void makeCodeIfElse(char* dest, char* expr_code, int expr_jump,
    char* block_code_if, char* block_code_else)
{
    char else_label[16];
    char final_label[16];

    makeLabel(else_label);
    makeLabel(final_label);

    dest[0] = '\0';

    
    sprintf(dest + strlen(dest), "%s", expr_code);
    sprintf(dest + strlen(dest), "%s %s\n", jumps[expr_jump + JUMPS_ARRAY_OFFSET],
        else_label);
    sprintf(dest + strlen(dest), "%s", block_code_if);
    sprintf(dest + strlen(dest), "jmp %s\n", final_label);
    sprintf(dest + strlen(dest), "%s:\n", else_label);
    sprintf(dest + strlen(dest), "%s", block_code_else);
    sprintf(dest + strlen(dest), "%s:\n", final_label);
}


void makeCodeWhile(char* dest, char* expr_code, int expr_jump, char* block_code)
{
    char loop_label[16];
    char final_label[16];

    makeLabel(loop_label);
    makeLabel(final_label);

    dest[0] = '\0';

    
    sprintf(dest + strlen(dest), "%s:\n", loop_label);
    sprintf(dest + strlen(dest), "%s", expr_code);
    sprintf(dest + strlen(dest), "%s %s\n", jumps[expr_jump + JUMPS_ARRAY_OFFSET],
        final_label);
    sprintf(dest + strlen(dest), "%s", block_code);
    sprintf(dest + strlen(dest), "jmp %s\n", loop_label);
    sprintf(dest + strlen(dest), "%s:\n", final_label);
}

