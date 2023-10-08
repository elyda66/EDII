#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define TAM 7

typedef struct no{
    int chave;
    struct no *proximo;
}Lista;

int funcaoHash(int chave){
    return chave % (int)TAM;
}

void inicializarTabela(Lista t[]) {
    for (int i = 0; i < TAM; i++) {
        t[i].chave = -1; //  chaves com um valor inválido
        t[i].proximo = NULL;  
    }
}



void *inserir(Lista t[], int valor) {
    int id = funcaoHash(valor);

    Lista *novoNo = (Lista *)malloc(sizeof(Lista));
    if (novoNo == NULL) {
        printf("Erro ao alocar memória para novo nó.\n");
        exit(1);
    }

    novoNo->chave = id;
    novoNo->proximo = t[id].proximo;
    t[id].proximo = novoNo;

    return novoNo->proximo;
}

void escreverHash(Lista t[], char arq[]) {
    FILE *filename = fopen(arq, "w+b");
    if (filename == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        exit(1);
    }

    for (int i = 0; i < TAM; i++) {
        Lista *aux = t[i].proximo;
        while (aux != NULL) {
            fwrite(&aux->chave, sizeof(int), 1, filename);
            aux = aux->proximo;
        }
    }

    fclose(filename);
}

void lerHash(Lista t[], char arq[]) {
    int count=1;
    int temp;
    FILE *filename = fopen(arq, "r+b");
    if (filename == NULL) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        exit(1);
    }
    printf("\n\n----------------");
    printf("TABELA HASH");
    printf("----------------\n");
    for (int i = 0; i < TAM; i++) {
        Lista *aux = t[i].proximo;
        if (aux != NULL){
            while (aux != NULL) {
            fread(&aux->chave, sizeof(int), 1, filename);
            temp = aux->chave;
            aux = aux->proximo;
            if(aux == NULL){
                 printf("Chave[%d]: %d\n", count, temp);
                }
                count++;
            }
        }
        else{
            printf("Chave[0]: \n");
        }
            
 
        count=1;

    }
    printf("\n");

    fclose(filename);
}

