##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## .
##

NAME_C = client

NAME_S = server


CLIENT = client.c

SERV = server.c

OB_C = $(CLIENT:.c=.o)

OB_S = $(SERV:.c=.o)

CFLAGS = -Wall -Wextra -Iinclude

LDFLAG = -g

all: $(NAME_C)	$(NAME_S)

$(NAME_C): $(OB_C)
		gcc -g $(OB_C) $(CFLAGS) -o $(NAME_C)  $(LDFLAG) 
$(NAME_S): $(OB_S)
		gcc -g $(OB_S) $(CFLAGS) -o $(NAME_S)  $(LDFLAG)

clean:
		rm -rf $(OB_C) && rm -rf $(OB_S)

fclean: clean
		rm -rf $(NAME_C) && rm -rf $(NAME_S)


re: fclean all
