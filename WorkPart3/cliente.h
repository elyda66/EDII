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

void escreverArquivo (Cliente* tabela){
    FILE* arquivoHash = abreArquivo();
    fseek (arquivoHash, 0, SEEK_SET);

    Cliente *cliente = (Cliente *) malloc(sizeof(Cliente));

    //avançando o ponteiro do arquivo até chegar ao seu fim
    while (fread(cliente, sizeof(Cliente), 1, arquivoHash));

    //sem isto o codigo escreve lixo do sistema na flag
    //tabela->flag = 0;

    //fwrite (metodo, sizeof(char), 1, arquivoHash);
    fwrite (tabela, sizeof(Cliente), 1, arquivoHash);

    fclose (arquivoHash);
    free (cliente);
}

void lerArquivo (){
    FILE* arquivoHash = abreArquivo();

    fseek (arquivoHash, 0, SEEK_SET);

    Cliente *cliente = (Cliente *) malloc(sizeof(Cliente));

    while (fread(cliente, sizeof(Cliente), 1, arquivoHash)){

        printf("Codigo do cliente: %d\t", cliente->codCliente);
        printf("Nome do cliente: %s\t", cliente->nome);
        printf("flag: %d\n", cliente->flag);
    }
}

int hashBase2(int x, int tam){
    //método da multiplicação

    x = (int) pow(x, 2);
    int wordBits = ceil(log(x)/log(2));
    int nBits = ceil(log(tam)/log(2));
    int cutOff = wordBits - nBits;

    for(int i = 0; i < cutOff; i++){
        if(!(i%2))
            x = floor(x/2);
        else
            x = x % (int) pow(2, wordBits - (i + 1));
    }

    return hashBase(x, tam);

}

int* hashLinear(int x, int tam){
    int* conjuntoHash = (int*)malloc(sizeof(int) * tam);
    //O problema está em algum lugar nesta função. Mais provavelmente no loop.
    for(int k = 0; k < tam; k++){
        conjuntoHash[k] = (hashBase(x, tam) + k) % tam;
    }

    return conjuntoHash;
}

int* hashQuadratica(int x, int tam, int c1, int c2){
    int* conjuntoHash = (int*)malloc(sizeof(int) * tam);

    if(c2 == 0)
        return hashLinear(x, tam);

    for(int k = 0; k < tam; k++)
        conjuntoHash[k] = (int) (hashBase(x, tam) + c1 * k + c2 * pow(k, 2))%tam;

    return conjuntoHash;
}

int* hashDispersaoDupla(int x, int tam){
    int* conjuntoHash = (int*)malloc(sizeof(int) * tam);
    for(int k = 0; k < tam; k++)
        conjuntoHash[k] = hashBase(x, tam) + k * hashBase2(x, tam);

    return conjuntoHash;
}


Cliente* inserirCliente(Cliente* tabela, char metodoHash){
    //Eu sei que as inserções são aleatórias, mas desenvolver uma inserção manual faz sentido por enquanto ~ Walter
    Cliente novoCliente;
    int* conjuntoHash;
    int tam = 7;
    int i;

    printf("Inserir chave do cliente:\t");
    scanf("%d", &novoCliente.codCliente);
    getchar();
    printf("Inserir nome do cliente:\t");
    scanf("%s", &novoCliente.nome);
    getchar();
    
    switch(metodoHash){
        case 'l':
            conjuntoHash = hashLinear(novoCliente.codCliente,tam);
            break;

        case 'q':
            conjuntoHash = hashQuadratica(novoCliente.codCliente, tam, 0, 1);
            break;

        case 'd':
            conjuntoHash = hashDispersaoDupla(novoCliente.codCliente, tam);
            break;

        default:
            printf("Algo está dando errado na alocação.\n");
            exit(1);
    }


    for(i = 0; i < tam; i++){
        if(tabela[conjuntoHash[i]].flag == 0){
            tabela[conjuntoHash[i]] = novoCliente;
            //printf("Cliente #%d — %s inserido com sucesso!\n", tabela[conjuntoHash[i]].codCliente, tabela[conjuntoHash[i]].nome);
            
            escreverArquivo(&tabela[conjuntoHash[i]]);

            lerArquivo();
            break;
        }
    }

    if(i == tam)
        printf("Erro! Nao ha espaco na tabela. Desculpe-nos pelo inconveniente.\n");

    free(conjuntoHash);
    return tabela;
}