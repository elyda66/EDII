#include <stdio.h>
#include <stdlib.h>
#include "cliente.h"


int main(){
    Cliente* tabela = (Cliente*)malloc(sizeof(Cliente) * TAM);
    criaArquivo();

    for(int i = 0; i < TAM; i++){
        tabela[i].flag = 0;
    }

    char metodo;
    int continuar = 1;
do
{
    printf("Insira um caractere para o metodo de hashing:\nl — Linear\nq — Quadratica\nd — Dispersao Dupla\n");
    scanf("%c", &metodo);
    getchar();
    tabela = inserirCliente(tabela, metodo);
    printf("deseja continuar?\n");
    scanf("%d", &continuar);
    getchar();
} while (continuar);

    


    free(tabela);

    return 0;
}