NAME = arvore

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -g

SRCS = main.c t_binary_tree.c
OBJS = $(SRCS:.c=.o)
HEADERS = t_binary_tree.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME) $(NAME).exe

re: fclean all

run: all
	./$(NAME)

test: all
	@for entrada in tests/*.in; do \
		esperado=$${entrada%.in}.out; \
		if ./$(NAME) < $$entrada | diff --strip-trailing-cr -q - $$esperado > /dev/null; then \
			echo "OK     $$entrada"; \
		else \
			echo "FALHOU $$entrada"; \
		fi; \
	done

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) < tests/01_enunciado.in

.PHONY: all clean fclean re run test valgrind
