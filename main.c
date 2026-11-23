/*
    Integrantes do grupo: Eduardo Paino e Bernardo Kapp
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "t_binary_tree.h"

#define INITIAL_LINE_SIZE 128

char *read_line(void) {

    int capacidade = INITIAL_LINE_SIZE;
    int tamanho = 0;
    int caractere;
    char *linha = malloc(capacidade);

    if(linha == NULL) {

        return NULL;
    }

    while((caractere = getchar()) != EOF && caractere != '\n') {

        if(tamanho + 1 == capacidade) {

            char *linha_maior = realloc(linha, capacidade * 2);

            if(linha_maior == NULL) {

                free(linha);
                return NULL;
            }

            linha = linha_maior;
            capacidade *= 2;
        }

        linha[tamanho] = caractere;
        tamanho++;
    }

    if(caractere == EOF && tamanho == 0) {

        free(linha);
        return NULL;
    }

    linha[tamanho] = '\0';

    return linha;
}

char *trim(char *texto) {

    while(isspace((unsigned char) *texto)) {

        texto++;
    }

    int tamanho = strlen(texto);

    while(tamanho > 0 && isspace((unsigned char) texto[tamanho - 1])) {

        texto[tamanho - 1] = '\0';
        tamanho--;
    }

    return texto;
}

char *split_argument(char *linha) {

    char *separador = linha;

    while(*separador != '\0' && !isspace((unsigned char) *separador)) {

        separador++;
    }

    if(*separador == '\0') {

        return separador;
    }

    *separador = '\0';

    return trim(separador + 1);
}

void execute_create(t_binary_tree **arvore, const char *expressao) {

    t_binary_tree *nova_arvore = create_tree(expressao);

    if(nova_arvore == NULL) {

        printf("invalid\n");
        return;
    }

    destroy_tree(*arvore);
    *arvore = nova_arvore;
}

void execute_height(const t_binary_tree *arvore, const char *rotulo) {

    if(*rotulo == '\0') {

        printf("invalid\n");
        return;
    }

    int altura = tree_node_height(arvore, rotulo);

    if(altura == 0) {

        printf("node not found\n");
        return;
    }

    printf("%d\n", altura);
}

int execute_command(t_binary_tree **arvore, char *linha) {

    char *comando = trim(linha);
    char *argumento = split_argument(comando);

    if(strcmp(comando, "create") == 0)      execute_create(arvore, argumento);
    else if(strcmp(comando, "pre") == 0)    tree_print_preorder(*arvore);
    else if(strcmp(comando, "in") == 0)     tree_print_inorder(*arvore);
    else if(strcmp(comando, "post") == 0)   tree_print_postorder(*arvore);
    else if(strcmp(comando, "height") == 0) execute_height(*arvore, argumento);
    else if(strcmp(comando, "print") == 0)  tree_print(*arvore);
    else if(strcmp(comando, "exit") == 0)   return 0;
    else if(*comando != '\0')               printf("unknown command\n");

    return 1;
}

int main() {

    t_binary_tree *arvore = NULL;
    int executando = 1;
    char *linha;

    while(executando && (linha = read_line()) != NULL) {

        executando = execute_command(&arvore, linha);
        free(linha);
    }

    destroy_tree(arvore);

    return 0;
}
