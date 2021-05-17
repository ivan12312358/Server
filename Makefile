CXX = g++
CXXFLAGS = -Wall -std=c++17 -g
SRC = ./Akinator/akinator.cpp ./Stack/stack.cpp ./Tree/files.cpp ./Tree/tree.cpp ./Tree/node.cpp ./Server/server.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = akinator.out

all: $(OBJ)
	$(CXX) $^ -o $(TARGET)

.PHONY: clean
clean:
	-rm -rf $(OBJ) *.dot *.png *.log akinator.out
