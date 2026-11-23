# Árvore Binária em C

Trabalho Prático 05 de Estrutura de Dados

Implementação de uma TAD Árvore Binária (`t_binary_tree`) criada a partir de uma representação textual em parênteses, com percursos, altura de nó e impressão em modo texto.

## Integrantes

- Eduardo Paino
- Bernardo Kapp

## Estrutura do projeto

```
.
├── main.c            # Interpretador de comandos
├── t_binary_tree.c   # Implementação da TAD (criação/validação, percursos, altura, impressão)
├── t_binary_tree.h   # Declaração da TAD Árvore Binária (t_binary_tree)
├── tests/            # Entradas (.in) e saídas esperadas (.out)
├── makefile
└── README.md
```

## Compilação e execução

```bash
make            # compila e gera o executável ./arvore
make run        # compila e executa (digitar os comandos no terminal)
make test       # roda todos os casos da pasta tests/ e compara com a saída esperada
make valgrind   # executa com o valgrind para verificar vazamentos de memória
make clean      # remove os arquivos objeto (.o)
make fclean     # remove os .o e o executável
make re         # limpa tudo e recompila
```

## Comandos

| Comando | Descrição |
|---|---|
| `create <expressão>` | cria a árvore a partir da expressão em parênteses |
| `pre` | percurso em pré-ordem |
| `in` | percurso em in-ordem |
| `post` | percurso em pós-ordem |
| `height <n>` | altura do nó `n` |
| `print` | imprime a árvore em modo texto |
| `exit` | sai do programa |

### Exemplo

```
create (A,(B,(),()),(C,(F,(),()),()))
print
    C
        F
A
    B
pre
A B C F
in
B A F C
post
B F C A
height C
2
exit
```

```
create (A,(B,()),(C,(F,(),()),()))
invalid
```

## Formato da expressão

```
arvore  ->  ()                        árvore vazia
        |   (rotulo,arvore,arvore)    raiz, subárvore esquerda, subárvore direita
```

- Espaços entre os símbolos são ignorados.
- O rótulo pode ter mais de um caractere (ex.: `(10,(5,(),()),())`), desde que não tenha espaço, vírgula ou parênteses.

### Verificação de expressões mal formadas

A validação é feita em duas etapas:

1. **Aninhamento dos parênteses** (`has_balanced_parentheses`): percorre a expressão contando os parênteses abertos; o contador nunca pode ficar negativo e precisa terminar em zero.
2. **Estrutura** (`parse_subtree`): um analisador recursivo confere se cada nó tem exatamente rótulo + duas subárvores separados por vírgula. É essa etapa que detecta, por exemplo, `(B,())` (falta uma subárvore) no exemplo 3 do enunciado.

Se a expressão for inválida é impresso `invalid`, tudo que já tinha sido alocado é liberado e a árvore anterior (se existir) é mantida.

## Decisões de implementação

- **Altura**: seguindo a definição dos slides da disciplina, a raiz está no nível 1 e a altura de um nó é a altura da subárvore que começa nele. Logo, uma **folha tem altura 1**. Se o nó não existir é impresso `node not found`.
- **print**: a árvore é impressa "deitada", percorrendo direita → raiz → esquerda, com 4 espaços de recuo por nível. A raiz fica na margem esquerda, a subárvore direita em cima e a esquerda embaixo.
- A linha de entrada é lida com alocação dinâmica, então não há limite de tamanho para a expressão do `create`.
