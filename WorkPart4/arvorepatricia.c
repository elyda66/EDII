#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define MAX_WORD_LENGTH 5

typedef struct Node {
    struct Node* children[ALPHABET_SIZE];
    int isEndOfWord;
} Node;

Node* createNode() {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node) {
        node->isEndOfWord = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}

void insertWord(Node* root, const char* word) {
    Node* currentNode = root;

    for (int level = 0; level < strlen(word); level++) {
        int index = word[level] - 'a';

        if (!currentNode->children[index]) {
            currentNode->children[index] = createNode();
        }

        currentNode = currentNode->children[index];
    }

    currentNode->isEndOfWord = 1;
}

int searchWord(Node* root, const char* word) {
    Node* currentNode = root;

    for (int level = 0; level < strlen(word); level++) {
        int index = word[level] - 'a';

        if (!currentNode->children[index]) {
            return 0; // Palavra não encontrada
        }

        currentNode = currentNode->children[index];
    }

    return currentNode != NULL && currentNode->isEndOfWord;
}

int main() {
    Node* root = createNode();

    insertWord(root, "cachorro");
    insertWord(root, "casa");
    insertWord(root, "carro");

    if (searchWord(root, "cachorro")) {
        printf("Palavra encontrada: cachorro\n");
    } else {
        printf("Palavra nao encontrada\n");
    }

    if (searchWord(root, "carro")) {
        printf("Palavra encontrada: carro\n");
    } else {
        printf("Palavra nao encontrada: carros\n");
    }

    return 0;
}

