##
## EPITECH PROJECT, 2024
## Makefile
##

CC  = clang
CXX = clang++

NAME_S = myftp

SRC_C =

SRC_CPP = my_server/server.cpp

# INIT
SRC_C += my_server/init/init_ftp.c
SRC_C += my_server/init/init_server.c
SRC_C += my_server/init/init_polling.c
SRC_C += my_server/init/init_client.c

# UTILS
SRC_C += my_server/utils/reterr.c
SRC_C += my_server/utils/my_str_to_word_array.c
SRC_C += my_server/utils/is_file.c
SRC_C += my_server/utils/get_data.c
SRC_C += my_server/utils/check_event.c
SRC_C += my_server/utils/is_valid.c
SRC_C += my_server/utils/fill_current_cmd.c

# FTP
SRC_C += my_server/ftp/run_ftp.c
SRC_C += my_server/ftp/add_user.c

# COMMAND UTILS
SRC_C += my_server/ftp/command/cmd_tab.c
SRC_C += my_server/ftp/command/command_tab.c
SRC_C += my_server/ftp/command/parsing_command.c
SRC_C += my_server/ftp/command/handle_co.c

# COMMAND
SRC_C += my_server/ftp/command/noop.c
SRC_C += my_server/ftp/command/user.c
SRC_C += my_server/ftp/command/pwd.c
SRC_C += my_server/ftp/command/pasv.c
SRC_C += my_server/ftp/command/list.c
SRC_C += my_server/ftp/command/cwd.c
SRC_C += my_server/ftp/command/cdup.c
SRC_C += my_server/ftp/command/retr.c
SRC_C += my_server/ftp/command/pass.c
SRC_C += my_server/ftp/command/help.c
SRC_C += my_server/ftp/command/dele.c
SRC_C += my_server/ftp/command/stor.c
SRC_C += my_server/ftp/command/port.c
SRC_C += my_server/ftp/command/quit.c

# FREE
SRC_C += my_server/free/destroy_ftp.c

OBJ_C   = $(SRC_C:.c=.o)
OBJ_CPP = $(SRC_CPP:.cpp=.o)
OBJ     = $(OBJ_C) $(OBJ_CPP)

CFLAGS   = -Wall -Wextra -Iinclude -g
CXXFLAGS = -Wall -Wextra -Iinclude -g
LDFLAGS  = -g

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(NAME_S)

$(NAME_S): $(OBJ)
	$(CXX) $(OBJ) -o $(NAME_S) $(LDFLAGS)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME_S)

re: fclean all
