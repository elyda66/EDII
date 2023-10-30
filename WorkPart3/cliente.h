#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define TAM 7

typedef struct cliente{
    int codCliente;
    char nome[101];
    int flag;
}Cliente;

int hashBase(int x, int tam){
    return x%tam;
}

typedef struct no{
    int chave;
    struct no *proximo;
}Lista;

void inicializarTabela(Lista t[]) {
    for (int i = 0; i < TAM; i++) {
        t[i].chave = 0; //  chaves com um valor inválido
        t[i].proximo = NULL;
    }
}

int funcaoHash(int chave){
    return chave % (int)TAM;
}

int  funcaoFlag(int codCliente, Cliente cliente[], int numCliente, int posicao, Lista t[]){

    int hash = funcaoHash(codCliente);
    if(codCliente == -1){
       return 0;
    }

    for (int i = posicao; i < numCliente; i++){
        if (t[hash].chave == 0){
            t[hash].chave = 1;
            return 1;
        }else{
            for(int j = hash; j < numCliente; j++){
                if(t[j].chave != 0){
                    continue;
                }else{
                     t[j].chave = 1;
                     return 1;
                }
            }
        }
    }
}

//zera os valores
Cliente* iniciaTabela (Cliente tabela[]){
    for (int i = 0; i < TAM; i++){
        tabela[i].codCliente = 0;
        tabela[i].nome[0] = '\0';
        tabela[i].flag = 0;
    }
    return tabela;
}

// Cria o arquivo binário
void *criaArquivo(){
    FILE *arquivo = fopen("tabHash.dat", "w+b");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    fclose (arquivo);
}
//abre o arquivo
FILE *abreArquivo (){
    FILE *arquivo = fopen("tabHash.dat", "r+b");

    if (arquivo == NULL){
        printf("erro ao abrir o arquivo cliente\n");
        exit (1);
    }
    return arquivo;
}

void escreverArquivo (Cliente tabelaLinear[], Cliente tabelaQuadratica[], Cliente tabelaDupla[]){
    FILE* arquivoHash = abreArquivo();
    fseek (arquivoHash, 0, SEEK_SET);

    //Cliente *cliente = (Cliente *) malloc(sizeof(Cliente));
    //avançando o ponteiro do arquivo até chegar ao seu fim
    //Não vejo motivo nisso. ~Walter
    //while (fread(cliente, sizeof(Cliente), 1, arquivoHash)) continue;

    //sem isto o codigo escreve lixo do sistema na flag
    //tabela->flag = 0;

    //fwrite (metodo, sizeof(char), 1, arquivoHash);

    fwrite (tabelaLinear, sizeof(Cliente), TAM, arquivoHash);
    fwrite (tabelaQuadratica, sizeof(Cliente), TAM, arquivoHash);
    fwrite (tabelaDupla, sizeof(Cliente), TAM, arquivoHash);


    fclose (arquivoHash);
    //free (cliente);
}

void lerArquivo (){
    FILE* arquivoHash = abreArquivo();


    fseek (arquivoHash, 0, SEEK_SET);

    Cliente *cliente = (Cliente*) malloc(sizeof(Cliente));

    for (int i = 0; i < 3; i++){

        switch (i)
        {
        case 0:
            printf ("\n\n");
            printf ("==========================================================================\n");
            printf ("Linear\n");
            break;
        case 1:
            printf ("Quadratica\n");
            break;
        case 2:
            printf("Dupla\n");
            break;
        }


        for (int j = 0; j < TAM; j++){
            fread(cliente, sizeof(Cliente), 1, arquivoHash);
            printf("Codigo do cliente: %d\t", cliente->codCliente);
            printf("Nome do cliente: %s\t", cliente->nome);
            printf("flag: %d\n", cliente->flag);
        }

        printf ("==========================================================================\n\n");

    }
}

int hashBase2(int x, int tam){
    //método da multiplicação

    x = (int) pow(x, 2);
    int wordBits = ceil(log(x)/log(2));
    int nBits = ceil(log(tam)/log(2));
    int cutOff = wordBits - nBits;

    if(cutOff > 0){
        for(int i = 0; i < cutOff; i++){
            if(!(i%2))
                x = floor(x/2);
            else
                x = x % (int) pow(2, wordBits - (i + 1));
        }
    }

    return hashBase(x, tam);

}

int* hashLinear(int x, int tam){
    int* conjuntoHash = (int*)malloc(sizeof(int) * tam);

    printf("Chaves da Hash Linear\n");

    for(int k = 0; k < tam; k++){
        conjuntoHash[k] = (hashBase(x, tam) + k) % tam;
        printf("%d\n", conjuntoHash[k]);
    }

    return conjuntoHash;
}

int* hashQuadratica(int x, int tam, int c1, int c2){
    int* conjuntoHash = (int*)malloc(sizeof(int) * tam);

    printf("Chaves da Hash Quadratica\n");

    if(c2 == 0)
        return hashLinear(x, tam);

    for(int k = 0; k < tam; k++){
        conjuntoHash[k] = (int) (hashBase(x, tam) + c1 * k + c2 * (int)pow(k, 2))%tam;
        printf("%d\n", conjuntoHash[k]);
    }

    return conjuntoHash;
}

int* hashDispersaoDupla(int x, int tam){
    int* conjuntoHash = (int*)malloc(sizeof(int) * tam);

    printf("Chaves da Hash de Dispersao Dupla\n");

    for(int k = 0; k < tam; k++){
        conjuntoHash[k] = (hashBase(x, tam) + k * hashBase2(x, tam)) % tam;
        printf("%d\n", conjuntoHash[k]);
    }

    return conjuntoHash;
}


Cliente* inserirCliente(Cliente* tabela, char metodoHash, Cliente cli){
    //Removi a inserção manual de dentro da função. ~Walter
    int* conjuntoHash;
    int i;

    switch(metodoHash){
        case 'l':
            conjuntoHash = hashLinear(cli.codCliente,TAM);
            break;

        case 'q':
            conjuntoHash = hashQuadratica(cli.codCliente, TAM, 0, 1);
            break;

        case 'd':
            conjuntoHash = hashDispersaoDupla(cli.codCliente, TAM);
            break;

        default:
            printf("Algo esta dando errado na alocacao.\n");
            exit(1);
    }

    for(i = 0; i < TAM; i++){
        if(tabela[conjuntoHash[i]].flag == 0){
            tabela[conjuntoHash[i]] = cli;
            break;
        }
    }

    if(i == TAM)
        printf("Erro! Nao ha espaco na tabela. Desculpe-nos pelo inconveniente.\n");

    free(conjuntoHash);
    return tabela;
}
