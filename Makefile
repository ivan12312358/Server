CXX = g++
CXXFLAGS = -c -Wall

all: ./Akinator/akinator.cpp ./Stack/stack.cpp ./Tree/files.cpp ./Tree/tree.cpp ./Tree/node.cpp
	$(CXX) $(CXXFLAGS) ./Akinator/akinator.cpp -o akinator.o 
	$(CXX) $(CXXFLAGS) ./Stack/stack.cpp -o stack.o
	$(CXX) $(CXXFLAGS) ./Tree/files.cpp -o files.o
	$(CXX) $(CXXFLAGS) ./Tree/tree.cpp -o tree.o
	$(CXX) $(CXXFLAGS) ./Tree/node.cpp -o node.o
	$(CXX) $(CXXFLAGS) ./Server/server.cpp -o server.o
	$(CXX) files.o tree.o akinator.o node.o stack.o server.o -o akinator
	rm -rf *.o

clean:
	rm -rf *.o STACK_DUMP.log base.log graph.dot graph.png saved.base