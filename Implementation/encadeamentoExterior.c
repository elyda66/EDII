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
    char arquivo[] = {"compartimento_hash.dat"};
    Lista tabela[TAM];
    int numClientes;
    arquivoCliente();


    printf("\n\nQuantos clientes deseja adicionar: ");
    scanf("%d", &numClientes);
    printf("\n");

    // Declara um array de clientes com o tamanho informado pelo usuário
     Cliente clientes[numClientes];

    // Preenche o array com os dados do usuário
    for (int i = 0; i < numClientes; i++) {
        printf("Informe o ID do Cliente %d: ", i + 1);
        scanf("%d", &clientes[i].codCliente);

        printf("Informe o nome do Cliente %d: ", i + 1);
        scanf("%s", clientes[i].nome);
        printf("\n");
    }

     numClientes = sizeof(clientes) / sizeof(clientes[0]);

    FILE *arqCliente = abreCliente();
    if (arqCliente != NULL) {
            
        escreverClientes(clientes, numClientes);
        fclose(arqCliente);
        
        arqCliente = abreCliente();
        
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
    
    int remove;

    printf ("Insira o codigo que quer remover\n");

    scanf ("%d", &remove);

    removeCliente (remove);

    lerClientes (numClientes);
    return 0;
}