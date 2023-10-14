#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAM 7
// No início do arquivo cliente.h
FILE* arquivoClientes();


// chave e seu valor
typedef struct {
    int codCliente;
    char nome[100];    
    bool status; // true == LIBERADO ou false == OCUPADO
    int posicao;
    int Flag;
    struct Cliente *prox;
}Cliente;

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
        
      //  int hashNew = funcaoHash(cliente[i].codCliente);
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
            //return 0;
        }    
    }
}


// Cria o arquivo binário 
FILE *arquivoCliente(){
    FILE *arquivo = fopen("tabHash.dat", "w+b");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    //printf ("Cliente criado com sucesso\n");
    return arquivo;
}
//abre o arquivo
FILE *abreCliente (){
    FILE *arquivo = fopen("tabHash.dat", "r+b");

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
    if (arquivoCliente == NULL) {
        printf("Erro ao abrir arquivo de clientes.\n");
        exit(1);
    }

    Cliente *clientes = (Cliente *)malloc(sizeof(Cliente) * numClientes);

    if (clientes == NULL) {
        printf("Erro ao alocar memória para clientes.\n");
        exit(1);
    }

    fseek (arquivoCliente, 0, SEEK_SET);

    if (fread(clientes, sizeof(Cliente), numClientes, arquivoCliente) != numClientes) {
        printf("Erro ao ler clientes do arquivo.\n");
        exit(1);
    }

    for (int i = 0; i < numClientes; i++) {
        printf("Codigo Cliente: %d\t", clientes[i].codCliente);
        printf("Nome do cliente: %s\t", clientes[i].nome);
      
/*
  if (clientes[i].status == false){
            printf ("\tstatus: false ");
        }else{
            printf ("\tstatus: true ");
        }
*/
        //printf ("\tproximo: %d\n\n", clientes[i].prox);
        printf ("\t\tProximo cliente: %d", clientes[i].posicao);
        printf ("\t\t Flag cliente: %d\n", clientes[i].Flag);
    }

    free(clientes);
}


int VerificarPosicaoCliente(int codCliente, Cliente cliente[], int numCliente, int posicao){

    int hash = codCliente % 7;

    for (int i = posicao + 1; i < numCliente; i++){
        if (cliente[i].status == true){
            continue;
        }
        int temp = cliente[i].codCliente % 7;
        if (hash == temp){
            return i + 1;
        }
    }

    return -1; 
}

//Escreve os clientes no arquivo .dat atráves da estrutura do tipo Cliente
void escreverClientes(Cliente *clientes, int numClientes, Lista t[]) {
    FILE *arquivoCli = arquivoCliente();
    if (arquivoCli == NULL) {
        printf("Erro ao abrir arquivo de clientes.\n");
        exit(1);
    }

    Cliente *cliente = (Cliente *)malloc(sizeof(Cliente));
    cliente = clientes;
    
    for (int i = 0; i < numClientes; i++){

        cliente[i].status = false;
    }
    for (int i = 0; i < numClientes; i++){

        cliente[i].posicao = VerificarPosicaoCliente(cliente[i].codCliente, cliente, numClientes, i);
    }
     for (int i = 0; i < TAM; i++){

        cliente[i].Flag = funcaoFlag(cliente[i].codCliente, cliente, TAM, i,t);
    }
    

    fwrite(cliente, sizeof(Cliente), numClientes, arquivoCli);
    
    fclose(arquivoCli);
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

