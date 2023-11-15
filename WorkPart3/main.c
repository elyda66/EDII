#include <stdio.h>
#include <stdlib.h>
#include "cliente.h"


int main(){
    Cliente* tabelaLinear = (Cliente*)malloc(sizeof(Cliente) * TAM);
    Cliente* tabelaQuadratica = (Cliente*)malloc(sizeof(Cliente) * TAM);
    Cliente* tabelaDupla = (Cliente*)malloc(sizeof(Cliente) * TAM);

    tabelaLinear = iniciaTabela (tabelaLinear);
    tabelaQuadratica = iniciaTabela (tabelaQuadratica);
    tabelaDupla = iniciaTabela (tabelaDupla);

    criaArquivo();

    char metodo;
    int funcao;
do
{
    printf ("\n\n");
    printf ("Qual acao quer fazer?\n");
    printf ("1: inserir na tabela\n");
    printf ("2: ler as tabelas\n");
    printf ("3: remover um termo das tabelas\n");
    printf ("0: encerrar o programa");
    printf("\n\n");
    scanf ("%d", &funcao);
    getchar();

    switch (funcao)
    {
    case 1:
        printf ("\n\n");
        printf("Insira um caractere para o metodo de hashing:\nl  Linear\nq  Quadratica\nd  Dispersao Dupla\n");
        scanf("%c", &metodo);
        getchar();
        if (metodo == 'l'){
            tabelaLinear = inserirCliente(tabelaLinear, metodo);
        }
        else if (metodo == 'q'){
            tabelaQuadratica = inserirCliente(tabelaQuadratica, metodo);
        }
        else{
            tabelaDupla = inserirCliente(tabelaDupla, metodo);
        }
        break;

        case 2:
            escreverArquivo (tabelaLinear, tabelaQuadratica, tabelaDupla);
            lerArquivo();
            break;

        case 3:
            printf("ainda não implementado\n");
            break;

        case 0:

            free(tabelaLinear);
            free(tabelaQuadratica);
            free(tabelaDupla);

            exit(1);
            break;
    
    default:

        printf("opcao invalida\n");
        break;
    }
} while (1);

    return 0;
}