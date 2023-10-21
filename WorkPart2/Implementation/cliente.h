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
       
    int verificador = 0;

    for (int i = posicao; i < numCliente; i++, verificador++){
        
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

    printf("------------------------------------------------------------------------------------------------------------\n");
    printf("HASH\t|CODIGO\t\t\t|NOME\t\t\t|PROXIMO\t\t|FLAG\n");
    printf("------------------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < numClientes; i++) {
        printf("%d\t" ,i);
        printf("| Codigo Cliente: %d\t", clientes[i].codCliente);
        printf("| Nome do cliente: %s\t", clientes[i].nome);
        // printf ("\t\tProximo cliente: %d\t", clientes[i].posicao);

        if(i <= numClientes){
            printf ("| Proximo cliente: %d\t", clientes[i].posicao);
        }else
        {
            printf ("| Proximo cliente: %d\t", clientes[i].posicao = i);
        }
        
        printf ("| Flag cliente: %d\n", clientes[i].Flag);
    }
    printf("------------------------------------------------------------------------------------------------------------\n");
    free(clientes);
    fclose(arquivoCliente);
}

//esta com erro, indo diretamente para o ultimo return
//a comparação com a hash tem que ser revista
int VerificarPosicaoCliente(int codCliente, Cliente cliente[], int posicao, int numCliente){

    if (codCliente == -1) return posicao;

    

    for (int i = posicao + 1; i < TAM ; i++){
        int hash = codCliente % 7;
        int temp = cliente[i].codCliente % 7;

        if (hash == temp){
            return i ;
        }
    }

    return posicao; 
}

//Escreve os clientes no arquivo .dat atráves da estrutura do tipo Cliente
void escreverClientes(Cliente *clientes, int numClientes, Lista t[]) {
    FILE *arquivoCli = arquivoCliente();

    Cliente *cliente = (Cliente *)malloc(sizeof(Cliente) * TAM);

    // Inicializa o array clientes
    for (int i = 0; i < TAM; i++) {
        cliente[i].codCliente = -1;
        strcpy(cliente[i].nome, "");
        cliente[i].posicao = -1;
        cliente[i].Flag = 0;
    }

    // Atualiza o array dos clientes
    for (int i = 0; i < numClientes; i++) {
        if (clientes[i].codCliente == -1){
            continue;
        }
        int hash = funcaoHash(clientes[i].codCliente);
        if (cliente[hash].codCliente == -1) {
            // Caso aposição estiver vazia coloque nela
            cliente[hash] = clientes[i];
        } else {
            // Caso a posição estiver ocupada, procure a proxima 
            int ver = 0;
            for (int j = hash + 1; j < TAM; j++, ver++) {
                if (cliente[j].codCliente == -1) {
                    cliente[j] = clientes[i];
                    break;
                }
                if (j == TAM -1 && ver < TAM -1){
                    for (int k = 0; k < hash; k++){
                        if (cliente[k].codCliente == -1) {
                            cliente[k] = clientes[i];
                            break;
                        }
                    }
                }
            }
        }
    }

    // atualiza a posição e a flag
    for (int i = 0; i < TAM; i++) {
        cliente[i].posicao = VerificarPosicaoCliente(cliente[i].codCliente, cliente, i, numClientes);
        cliente[i].Flag = funcaoFlag(cliente[i].codCliente, cliente, TAM, i, t);
    }

    fwrite(cliente, sizeof(Cliente), TAM, arquivoCli);

    fclose(arquivoCli);
    free(cliente);
}

//se retornar 0, o cliente nao foi encontrado
int removeCliente (int cod, Cliente clientes[]){
    FILE *arquivoCliente = abreCliente();
    Cliente *cliente = (Cliente *)malloc(sizeof(Cliente));

    fseek (arquivoCliente, 0, SEEK_SET);
    int contador = 0;

    while (fread (cliente, sizeof(Cliente), 1, arquivoCliente)){        

        if (cliente->codCliente == cod){
            fseek (arquivoCliente, 0, SEEK_SET);

            Cliente *aux = (Cliente *)malloc(sizeof(Cliente));

            //verificando se alguem aponta para o cliente removido
            for (int i = 0; i <TAM; i++){
                fread (aux, sizeof(Cliente), 1, arquivoCliente);

                if (i != contador && aux->posicao == contador){
                    //verificando se o cliente removido aponta para alguem
                    if (cliente->posicao != contador){
                        aux->posicao = cliente->posicao;
                        fseek (arquivoCliente, sizeof(Cliente) * i, SEEK_SET);
                        fwrite (aux, 1, sizeof(Cliente), arquivoCliente);
                        break;
                    }
                    else{
                        aux->posicao = i;
                        fseek (arquivoCliente, sizeof(Cliente) * i, SEEK_SET);
                        fwrite (aux, 1, sizeof(Cliente), arquivoCliente);
                        break;
                    }
                }
            }
            cliente->codCliente = -1;
            memset(cliente->nome, 0, 100);
            cliente->Flag = 0;
            cliente->posicao = contador;


            fseek (arquivoCliente, sizeof(Cliente) * contador, SEEK_SET);
            fwrite (cliente, 1, sizeof(Cliente), arquivoCliente);

            for (int i = 0; i < TAM; i++){
                if (clientes[i].codCliente == cod){
                    clientes[i].codCliente = -1;
                    memset(clientes[i].nome, 0, 100);//limpando o array de char
                    clientes[i].posicao = i;
                    clientes[i].Flag = 0;
                }
            }
            
            fclose (arquivoCliente);
            free (aux);
            free (cliente);

            return 1;
        }
        contador++;
    }
    fclose (arquivoCliente);
    free (cliente);
    printf ("cliente nao encontrado\n");

    return 0;

}

