// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include "compartimento_hash.h"
#include "cliente.h"
#include <string.h>

/*

Implementar o Encadeamento Exterior
•Tamanho da tabela: m = 7
•Função de hash: h(x) = x mod 7  <---- ok
•Registros a inserir: Clientes (codCliente (inteiro) e nome (Stringde 100 caracteres))

*/


int main() {
    int opcao;
    int i = 0;
    char arquivo[] = {"compartimento_hash.dat"};
    Lista tabela[TAM];
     int numClientes;

    printf("Quantos clientes deseja adicionar? ");
    scanf("%d", &numClientes);

    // Declara um array de clientes com o tamanho informado pelo usuário
     Cliente clientes[numClientes];

    // Preenche o array com os dados do usuário
    for (int i = 0; i < numClientes; i++) {
        printf("Informe o ID do Cliente %d: ", i + 1);
        scanf("%d", &clientes[i].codCliente);

        printf("Informe o nome do Cliente %d: ", i + 1);
        scanf("%s", clientes[i].nome);
    }

     numClientes = sizeof(clientes) / sizeof(clientes[0]);

    FILE *arqCliente = arquivoCliente("cliente.dat");
    if (arqCliente != NULL) {
        

    
    escreverClientes(clientes, numClientes);
    fclose(arqCliente);
    
    arqCliente = fopen("cliente.dat", "r+b");
    
    //Coloca o ponteiro para o inicio do arquivo
    fseek(arqCliente, 0, SEEK_SET);
    lerClientes(numClientes);

    fclose(arqCliente);
    } else {
        printf("Erro ao abrir o arquivo para escrita.\n");
    }
    
    inicializarTabela(tabela);

    //int valor[TAM] = clientes.codCliente;

    for (int i = 0; i < numClientes; i++){

        inserir(tabela, clientes[i].codCliente);
    }
    escreverHash(tabela,arquivo); 
    
    
    lerHash(tabela,arquivo);
    return 0;
}