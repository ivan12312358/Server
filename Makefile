CC = g++
CFLAGS = -c -Wall

all: ./Akinator/akinator.cpp ./Stack/stack.cpp ./Akinator/node.cpp ./Server/server.cpp
	$(CC) $(CFLAGS) ./Akinator/akinator.cpp -o akinator.o 
	$(CC) $(CFLAGS) ./Akinator/node.cpp -o node.o 
	$(CC) $(CFLAGS)	./Stack/stack.cpp -o stack.o
	$(CC) $(CFLAGS) ./Server/server.cpp -o server.o
	$(CC) akinator.o stack.o node.o server.o -o akinator
	rm -rf *.o

clean:
	rm -rf *.o akinator STACK_DUMP.log base.log graph.dot graph.png saved.base