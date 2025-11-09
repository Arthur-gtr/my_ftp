##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## .
##

NAME_C = client

NAME_S = server


CLIENT = client.c

SERV = my_server/server.c

#INIT
SERV += my_server/init/init_ftp.c
SERV += my_server/init/init_server.c
SERV += my_server/init/init_polling.c
SERV += my_server/init/init_client.c 

#UTILS
SERV += my_server/utils/reterr.c
SERV += my_server/utils/my_str_to_word_array.c

#FTP
SERV += my_server/ftp/run_ftp.c

#COMMAND
SERV += my_server/ftp/command/command_tab.c

#FREE
SERV += my_server/free/destroy_ftp.c

OB_C = $(CLIENT:.c=.o)

OB_S = $(SERV:.c=.o)

CFLAGS = -Wall -Wextra -Iinclude -g

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
