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
    int opcao, numClientes, cod;
    arquivoCliente();
    char arquivo[] = {"compartimento_hash.dat"};
    Lista tabela[TAM];
    Cliente *clientes = (Cliente *)malloc(sizeof(Cliente));
    

    do{
        printf("\n\nQual funcao quer usar?\n");
        printf("1: inserir cliente\n");
        printf("2: ler todos os clientes\n");
        printf("3: remover cliente\n");
        printf("0: encerrar o programa\n");

        printf("\nQual opcao voce escolher: ");
        scanf("%d", &opcao);
        printf("\n");
        switch (opcao)
        {
        case 1:
            
            printf("\n---- INSERINDO CLIENTES ----\n\n");
            printf("Quantos clientes deseja adicionar: ");
            scanf("%d", &numClientes);
            printf("\n\n");
            // Declara um array de clientes com o tamanho informado pelo usuário
            clientes[numClientes];

            // Preenche o array com os dados do usuário
            for (int i = 0; i < numClientes; i++) {
                printf("Informe o ID do Cliente %d: ", i + 1);
                scanf("%d", &clientes[i].codCliente);

                printf("Informe o nome do Cliente %d: ", i + 1);
                scanf("%s", clientes[i].nome);
                printf("\n");
            }
    
            printf("---- Cliente(s) Inseridos ----\n\n");
            escreverClientes(clientes, numClientes);

            inicializarTabela(tabela);
            for (int i = 0; i < numClientes; i++){  
                 inserir(tabela, clientes[i].codCliente);
            }
            escreverHash(tabela,arquivo); 

            break;
         case 2:
                lerClientes(numClientes);
                lerHash(tabela,arquivo);
            break;
         case 3:
             printf ("escreva o codigo do cliente que quer remover\n");
                scanf ("%d", &cod);

                if ((removeCliente (cod)) == 0){
                    printf ("cliente nao encontrado\n");
                }
                else{
                    numClientes--;
                }
            break;
         case 0:
            free(clientes);
            exit(1);
        default:
        printf("OPCAO INVALIDA!\n");
            break;
        }

    } while (true);
    
    return 0;
}