##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## .
##

NAME_S = myftp

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
SERV += my_server/ftp/command/noop.c
SERV += my_server/ftp/command/user.c
SERV += my_server/ftp/command/pwd.c
SERV += my_server/ftp/command/pasv.c
SERV += my_server/ftp/command/list.c
SERV += my_server/ftp/command/cwd.c
SERV += my_server/ftp/command/cdup.c
SERV += my_server/ftp/command/retr.c
SERV += my_server/ftp/command/pass.c
SERV += my_server/ftp/command/help.c
SERV += my_server/ftp/command/dele.c
SERV += my_server/ftp/command/stor.c
SERV += my_server/ftp/command/port.c

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
