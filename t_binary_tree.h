#ifndef T_BINARY_TREE_H
#define T_BINARY_TREE_H

#define LABEL_SIZE 32

typedef struct t_binary_tree t_binary_tree;

t_binary_tree *create_tree(const char *expressao);
void destroy_tree(t_binary_tree *arvore);

int tree_is_empty(const t_binary_tree *arvore);
void tree_print_preorder(const t_binary_tree *arvore);
void tree_print_inorder(const t_binary_tree *arvore);
void tree_print_postorder(const t_binary_tree *arvore);
void tree_print(const t_binary_tree *arvore);
int tree_node_height(const t_binary_tree *arvore, const char *rotulo);

#endif
