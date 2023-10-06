// Isso serve para criar a biblioteca !! 
//#include "compartimento_hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// chave e seu valor
typedef struct {
    int codCliente;
    char nome[101];    
    bool status; // TRUE == LIBERADO ou FALSE == OCUPADO
    struct Cliente *prox;
}Cliente;

// Cria o arquivo binário 
void *arquivoCliente(){
    FILE *arquivo = fopen("cliente.dat", "w+b");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    //printf ("Cliente criado com sucesso\n");
    fclose (arquivo);
}

//abre o arquivo
FILE* abreCliente (){
    FILE *arquivo = fopen("cliente.dat", "r+b");

    if (arquivo == NULL){
        printf("erro ao abrir o arquivo cliente\n");
        exit (1);
    }
    return arquivo;
}

Cliente *CriaCliente (int codCliente, char *nome){
    Cliente *cliente = (Cliente *)malloc(sizeof(Cliente));

    //dá entrada nas informações do cliente
    cliente->codCliente = codCliente;
    strcpy(cliente->nome, nome);
    cliente->status = false;
    cliente->prox = NULL;

    return cliente;
}


void lerClientes(int numClientes) {
    FILE *arquivoCliente = abreCliente();
    Cliente *clientes = (Cliente *)malloc(sizeof(Cliente));

    fseek (arquivoCliente, 0, SEEK_SET);

    if (clientes == NULL) {
        printf("Erro ao alocar memória para clientes.\n");
        exit(1);
    }

    //utilizando feof repete a ultima escrita 2 vezes
    //desta forma, caso o fread não consiga ler, ele retorna -1
    while(fread(clientes, sizeof(Cliente), 1, arquivoCliente)) {
        
        printf("Codigo Cliente: %d    ", clientes->codCliente);
        printf("Nome do cliente: %s    ", clientes->nome);
        if (clientes->status == true){
            printf ("status: TRUE    ");
        }
        else{
            printf("status: FALSE    ");
        }

        printf ("proximo: %p\n\n", clientes->prox);
    }    

    free(clientes);
    fclose (arquivoCliente);
}


// Escreve os clientes no arquivo .dat atráves da estrutura do tipo Cliente
// int escreverClientes(Cliente *clientes, int numClientes) {
//     FILE *arquivoCliente = abreCliente ();
//     Cliente *C = (Cliente *)malloc(sizeof(Cliente));//cliente contido no arquivo que estou lendo

//     clientes->status = false;

//     fseek (arquivoCliente, 0, SEEK_SET);

//     //procurando parte a parte
//     for (int i = 0; i < numClientes; i++){
//         size_t tamanho = sizeof(Cliente);
//         fread(C, tamanho, 1, arquivoCliente);

//         if(clientes->codCliente == C->codCliente){
//             if(C->status == false){
//             printf ("Codigo ja inserido\n");
//             break;
//             }
//             else{
//                 fseek(arquivoCliente, -(sizeof(Cliente)), SEEK_CUR);
//                 size_t tamanho = sizeof(Cliente);
//                 fwrite(clientes, tamanho, 1, arquivoCliente);
//                 fclose(arquivoCliente);
//                 numClientes++;
//                 break;
//             }
//         }
            
            
//         else if (feof(arquivoCliente)){
//             size_t tamanho = sizeof(Cliente);
//             fwrite(clientes, tamanho, 1, arquivoCliente);
//             fclose(arquivoCliente);
//             numClientes++;
//             break;
//         }
//     }

//     fclose (arquivoCliente);
//     free (C);
//     return numClientes;

// }

// Escreve os clientes no arquivo .dat atráves da estrutura do tipo Cliente
void escreverClientes(Cliente *clientes, int numClientes) {
    FILE *arquivoCliente = abreCliente ();
    Cliente *cliente = (Cliente *)malloc(sizeof(Cliente));//cliente contido no arquivo que estou lendo

    cliente->status = false;
    
    if (fwrite(cliente, sizeof(Cliente), numClientes, arquivoCliente) != numClientes) {
        printf("Erro ao escrever clientes no arquivo.\n");
        exit(1);
    }

    fclose(arquivoCliente);
    free(cliente);
}

//se retornar 0, o cliente nao foi encontrado
int removeCliente (int cod){
    FILE *arquivoCliente = abreCliente();
    Cliente *cliente = (Cliente *)malloc(sizeof(Cliente));

    fseek (arquivoCliente, 0, SEEK_SET);

    while (fread (cliente, sizeof(Cliente), 1, arquivoCliente)){        

        if (cliente->codCliente == cod){
            cliente->status = true;

            fseek (arquivoCliente, -sizeof(Cliente), SEEK_CUR);

            fwrite (cliente, 1, sizeof(Cliente), arquivoCliente);
            fclose (arquivoCliente);
            free (cliente);

            return 1;
        }
    }
    fclose (arquivoCliente);
    free (cliente);
    return 0;

}