##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## .
##

NAME_S = my_ftp

SERV = my_server/server.c

#INIT

SERV += my_server/init/init_ftp.c
SERV += my_server/init/init_server.c
SERV += my_server/init/init_polling.c
SERV += my_server/init/init_client.c 

#UTILS
SERV += my_server/utils/reterr.c
SERV += my_server/utils/my_str_to_word_array.c
SERV += my_server/utils/is_file.c

#FTP
SERV += my_server/ftp/run_ftp.c
SERV += my_server/ftp/add_user.c

#COMMAND UTILS
SERV += my_server/ftp/command/cmd_tab.c
SERV += my_server/ftp/command/command_tab.c
SERV += my_server/ftp/command/parsing_command.c
SERV += my_server/ftp/command/handle_co.c

#COMMAND
SERV += my_server/ftp/command/NOOP.c
SERV += my_server/ftp/command/USER.c
SERV += my_server/ftp/command/PWD.c
SERV += my_server/ftp/command/PASV.c
SERV += my_server/ftp/command/LIST.c
SERV += my_server/ftp/command/CWD.c
SERV += my_server/ftp/command/CDUP.c
SERV += my_server/ftp/command/RETR.c
SERV += my_server/ftp/command/PASS.c
SERV += my_server/ftp/command/HELP.c
SERV += my_server/ftp/command/DELE.c

#FREE
SERV += my_server/free/destroy_ftp.c

OB_S = $(SERV:.c=.o)

CFLAGS = -Wall -Wextra -Iinclude -g

LDFLAG = -g

all: $(NAME_S)
$(NAME_S): $(OB_S)
		gcc -g $(OB_S) $(CFLAGS) -o $(NAME_S)  $(LDFLAG)

clean:
		rm -rf $(OB_S)

fclean: clean
		rm -rf $(NAME_S)


re: fclean all
