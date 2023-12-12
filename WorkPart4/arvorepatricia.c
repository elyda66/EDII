#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Estrutura de um nó da árvore Patricia
struct Node {
    int isLeaf; // Indica se é uma folha
    char* key;  // Parte da chave armazenada no nó
    struct Node* children[ALPHABET_SIZE]; // Filhos do nó
};

// Função auxiliar para criar um novo nó
struct Node* createNode(char* key) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->isLeaf = 0;
    newNode->key = strdup(key);
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Função auxiliar para verificar se um nó é folha
int isLeafNode(struct Node* node) {
    return node->isLeaf;
}

// Função auxiliar para verificar se um nó tem filhos não nulos
int hasChildren(struct Node* node) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != NULL) {
            return 1;
        }
    }
    return 0;
}

// Função auxiliar para verificar se um nó tem apenas um filho não nulo
int hasSingleChild(struct Node* node) {
    int count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != NULL) {
            count++;
        }
    }
    return (count == 1);
}

// Função auxiliar para encontrar o índice do primeiro filho não nulo
int findFirstChild(struct Node* node) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != NULL) {
            return i;
        }
    }
    return -1; // Não deveria acontecer se o nó tiver pelo menos um filho
}


// Função para remover uma palavra da árvore Patricia
struct Node* removeWord(struct Node* root, char* key) {
    // Caso base: árvore vazia
    if (root == NULL) {
        return NULL;
    }

    // Caso recursivo: encontrar o nó correspondente à chave
    int index = key[0] - 'a';
    if (root->key[0] == key[0]) {
        // Caso: encontramos o nó correspondente à chave
        root->children[index] = removeWord(root->children[index], key + 1);

        // Verificar casos especiais de compactação
        if (!hasChildren(root) && !isLeafNode(root)) {
            // Caso: nó não tem filhos e não é folha, podemos remover o nó
            free(root->key);
            free(root);
            return NULL;
        } else if (hasSingleChild(root)) {
            // Caso: nó tem apenas um filho, podemos compactar os nós
            int childIndex = findFirstChild(root);
            struct Node* child = root->children[childIndex];

            char* combinedKey = (char*)malloc(strlen(root->key) + strlen(child->key) + 1);
            strcpy(combinedKey, root->key);
            strcat(combinedKey, child->key);

            free(root->key);
            free(child->key);

            root->key = combinedKey;
            root->isLeaf = isLeafNode(child);

            for (int i = 0; i < ALPHABET_SIZE; i++) {
                root->children[i] = child->children[i];
            }

            free(child);
        }
    } else {
        // Caso: não encontramos o nó correspondente à chave, então a chave não está na árvore
        return root;
    }

    return root;
}


// Função para inserir uma palavra na árvore Patricia
struct Node* insertWord(struct Node* root, char* key) {
    // Caso base: árvore vazia
    if (root == NULL) {
        return createNode(key);
    }

    // Caso recursivo: encontrar o nó correspondente à chave
    int index = key[0] - 'a';
    if (root->key[0] == key[0]) {
        // Caso: encontramos o nó correspondente à chave
        root->children[index] = insertWord(root->children[index], key + 1);
    } else {
        // Caso: não encontramos o nó correspondente à chave, então a chave não está na árvore
        struct Node* newNode = createNode(key);
        newNode->children[root->key[0] - 'a'] = root;
        return newNode;
    }

    return root;
}

// Função para imprimir a árvore Patricia
void printTree(struct Node* root, int level) {
    if (root != NULL) {
        printf("%*s", level * 2, ""); // Espaçamento para visualização
        printf("[%s]\n", root->key);
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            printTree(root->children[i], level + 1);
        }
    }
}

// Função para liberar a memória da árvore Patricia
void freeTree(struct Node* root) {
    if (root != NULL) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            freeTree(root->children[i]);
        }
        free(root->key);
        free(root);
    }
}




int main() {
    struct Node* root = NULL; // Inicializando uma árvore vazia

    // Inserindo algumas palavras na árvore
    root = insertWord(root, "batata");
    root = insertWord(root, "bola");
    root = insertWord(root, "batom");
    root = insertWord(root, "bolaco");
    root = insertWord(root, "bolacha");

    // Imprimindo a árvore inicial
    printf("Arvore Patricia inicial:\n");
    printTree(root, 0);
    printf("\n");

    // Removendo algumas palavras da árvore
    root = removeWord(root, "batom");
    // root = removeWord(root, "bolaco");
    // root = removeWord(root, "bolacha");

    // Imprimindo a árvore após remoções
    printf("Arvore Patricia apos remocoes:\n");
    printTree(root, 0);

    // Liberando a memória alocada pela árvore
    freeTree(root);

    return 0;
}
