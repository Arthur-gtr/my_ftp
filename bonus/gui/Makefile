##
## EPITECH PROJECT, 2024
## Makefile
##

TARGET = ftpGui

CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++17 -g
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lm 

INC = -I include

SRC = main.cpp

OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LIBS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(TARGET)
