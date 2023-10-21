// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
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
    int numClientes;
    Lista tabela[TAM];
    inicializarTabela(tabela);    


    arquivoCliente();


    //printf("\n\nQuantos clientes deseja adicionar: ");
    //scanf("%d", &numClientes);
    //printf("\n");

    // Declara um array de clientes com o tamanho informado pelo usuário
    Cliente clientes[TAM];
    
    for(int i = 0; i < TAM; i++){
        
        clientes[i].codCliente = -1;
        clientes[i].nome[0] = '\0';//limpando o array de char
        clientes[i].posicao = i;
        clientes[i].Flag = 0;

    }

    int fazer, cod;
    
    Cliente temp;

    
    do{
        printf("Qual funcao quer usar?\n");
        printf("1: inserir cliente\n");
        printf("2: ler todos os clientes\n");
        printf("3: remover cliente\n");
        printf("0: encerrar o programa\n");

        scanf("%d", &fazer);
        getchar();

        switch (fazer){
            case 1:
                printf ("insira o codigo do cliente:\n");
                scanf ("%d", &temp.codCliente);
                getchar();
                printf ("insira o nome do cliente:\n");
                scanf ("%99[^\n]", temp.nome);   
                getchar(); 


                for (int i = 0; i < TAM; i++){
                    if (clientes[i].codCliente == -1){
                        clientes[i].codCliente = temp.codCliente;
                        clientes[i].nome[99] = temp.nome[99];
                        clientes[i].Flag = 1;
                        
                        escreverClientes(clientes, TAM, tabela);
                        break;
                    }
                    if (i == TAM - 1){
                        printf("Houve um overflow\n");
                        printf("Remova um cliente caso queira inserir um novo\n\n");
                    }
                }
                printf("\n\n");
                for (int i = 0; i < TAM; i++){
                    printf("%d\n", clientes[i].codCliente);
                }
                printf("\n\n");

                break;

            case 2:
                lerClientes (TAM);
                break;               

            case 3:
                printf ("escreva o codigo do cliente que quer remover\n");
                scanf ("%d", &cod);

                removeCliente (cod, clientes);
                break;            

            case 0:        
                
                exit(1);
        
            default:
                printf ("opcao invalida\n");
                break;
            }

    
    } while (true);

    return 0;
}