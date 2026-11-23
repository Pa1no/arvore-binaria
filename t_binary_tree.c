#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "t_binary_tree.h"

#define INDENT_SIZE 4

typedef struct {

    char rotulo[LABEL_SIZE];
    struct t_node *esquerda;
    struct t_node *direita;
} t_node;

typedef struct {
    t_node *raiz;
} t_binary_tree;

t_node *create_node(const char *rotulo, t_node *esquerda, t_node *direita) {

    t_node *no = malloc(sizeof(t_node));

    if(no == NULL) {

        return NULL;
    }

    strcpy(no->rotulo, rotulo);
    no->esquerda = esquerda;
    no->direita = direita;

    return no;
}

void destroy_nodes(t_node *no) {

    if(no == NULL) {

        return;
    }

    destroy_nodes(no->esquerda);
    destroy_nodes(no->direita);
    free(no);
}

typedef struct {

    const char *texto;
    int posicao;
} t_parser;

int has_balanced_parentheses(const char *expressao) {

    int abertos = 0;

    for(int i = 0; expressao[i] != '\0'; i++) {

        if(expressao[i] == '(') {

            abertos++;
        }
        else if(expressao[i] == ')') {

            abertos--;

            if(abertos < 0) {

                return 0;
            }
        }
    }

    return abertos == 0;
}

void skip_spaces(t_parser *leitor) {

    while(isspace((unsigned char) leitor->texto[leitor->posicao])) {

        leitor->posicao++;
    }
}

int consume(t_parser *leitor, char esperado) {

    skip_spaces(leitor);

    if(leitor->texto[leitor->posicao] != esperado) {

        return 0;
    }

    leitor->posicao++;

    return 1;
}

int is_label_char(char caractere) {

    return caractere != '\0' && caractere != '(' && caractere != ')' && caractere != ','
           && !isspace((unsigned char) caractere);
}

int read_label(t_parser *leitor, char *rotulo) {

    int tamanho = 0;

    skip_spaces(leitor);

    while(is_label_char(leitor->texto[leitor->posicao])) {

        if(tamanho == LABEL_SIZE - 1) {

            return 0;
        }

        rotulo[tamanho] = leitor->texto[leitor->posicao];
        tamanho++;
        leitor->posicao++;
    }

    rotulo[tamanho] = '\0';

    return tamanho > 0;
}

int parse_subtree(t_parser *leitor, t_node **no);

int parse_children(t_parser *leitor, t_node **esquerda, t_node **direita) {

    *esquerda = NULL;
    *direita = NULL;

    if(!consume(leitor, ',') || !parse_subtree(leitor, esquerda)) {

        return 0;
    }

    if(consume(leitor, ',') && parse_subtree(leitor, direita) && consume(leitor, ')')) {

        return 1;
    }

    destroy_nodes(*esquerda);
    destroy_nodes(*direita);
    *esquerda = NULL;
    *direita = NULL;

    return 0;
}

int parse_subtree(t_parser *leitor, t_node **no) {

    char rotulo[LABEL_SIZE];
    t_node *esquerda;
    t_node *direita;

    *no = NULL;

    if(!consume(leitor, '(')) {

        return 0;
    }

    if(consume(leitor, ')')) {

        return 1;
    }

    if(!read_label(leitor, rotulo) || !parse_children(leitor, &esquerda, &direita)) {

        return 0;
    }

    *no = create_node(rotulo, esquerda, direita);

    if(*no == NULL) {

        destroy_nodes(esquerda);
        destroy_nodes(direita);
        return 0;
    }

    return 1;
}

int reached_end(t_parser *leitor) {

    skip_spaces(leitor);

    return leitor->texto[leitor->posicao] == '\0';
}

t_binary_tree *create_tree(const char *expressao) {

    if(!has_balanced_parentheses(expressao)) {

        return NULL;
    }

    t_parser leitor = { expressao, 0 };
    t_node *raiz;

    if(!parse_subtree(&leitor, &raiz)) {

        return NULL;
    }

    if(!reached_end(&leitor)) {

        destroy_nodes(raiz);
        return NULL;
    }

    t_binary_tree *arvore = malloc(sizeof(t_binary_tree));

    if(arvore == NULL) {

        destroy_nodes(raiz);
        return NULL;
    }

    arvore->raiz = raiz;

    return arvore;
}

void destroy_tree(t_binary_tree *arvore) {

    if(arvore == NULL) {

        return;
    }

    destroy_nodes(arvore->raiz);
    free(arvore);
}

int tree_is_empty(const t_binary_tree *arvore) {

    return arvore == NULL || arvore->raiz == NULL;
}

void print_label(const t_node *no, int *primeiro) {

    if(!*primeiro) {

        printf(" ");
    }

    printf("%s", no->rotulo);
    *primeiro = 0;
}

void preorder(const t_node *no, int *primeiro) {

    if(no == NULL) {

        return;
    }

    print_label(no, primeiro);
    preorder(no->esquerda, primeiro);
    preorder(no->direita, primeiro);
}

void inorder(const t_node *no, int *primeiro) {

    if(no == NULL) {

        return;
    }

    inorder(no->esquerda, primeiro);
    print_label(no, primeiro);
    inorder(no->direita, primeiro);
}

void postorder(const t_node *no, int *primeiro) {

    if(no == NULL) {

        return;
    }

    postorder(no->esquerda, primeiro);
    postorder(no->direita, primeiro);
    print_label(no, primeiro);
}

const t_node *root_of(const t_binary_tree *arvore) {

    return tree_is_empty(arvore) ? NULL : arvore->raiz;
}

void tree_print_preorder(const t_binary_tree *arvore) {

    int primeiro = 1;

    preorder(root_of(arvore), &primeiro);
    printf("\n");
}

void tree_print_inorder(const t_binary_tree *arvore) {

    int primeiro = 1;

    inorder(root_of(arvore), &primeiro);
    printf("\n");
}

void tree_print_postorder(const t_binary_tree *arvore) {

    int primeiro = 1;

    postorder(root_of(arvore), &primeiro);
    printf("\n");
}

void print_sideways(const t_node *no, int nivel) {

    if(no == NULL) {

        return;
    }

    print_sideways(no->direita, nivel + 1);
    printf("%*s%s\n", nivel * INDENT_SIZE, "", no->rotulo);
    print_sideways(no->esquerda, nivel + 1);
}

void tree_print(const t_binary_tree *arvore) {

    print_sideways(root_of(arvore), 0);
}

t_node *find_node(const t_node *no, const char *rotulo) {

    if(no == NULL || strcmp(no->rotulo, rotulo) == 0) {

        return no;
    }

    const t_node *encontrado = find_node(no->esquerda, rotulo);

    if(encontrado != NULL) {

        return encontrado;
    }

    return find_node(no->direita, rotulo);
}

int subtree_height(const t_node *no) {

    if(no == NULL) {

        return 0;
    }

    int altura_esquerda = subtree_height(no->esquerda);
    int altura_direita = subtree_height(no->direita);

    return 1 + (altura_esquerda > altura_direita ? altura_esquerda : altura_direita);
}

int tree_node_height(const t_binary_tree *arvore, const char *rotulo) {

    return subtree_height(find_node(root_of(arvore), rotulo));
}
