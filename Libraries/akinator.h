#pragma once
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>


const int SIZE = 128;
char buf[SIZE] = "";
char string[SIZE*3] = "";

enum game_modes
{
	Clear = '0',
	Guessing, 
	Definition,
	Compare,
	Graph,
	Save,
	Exit
};


struct Node;

class Tree
{
	Node* head_ = nullptr;

	public:
		Tree (const char* filename);
	   ~Tree ();
		void Akinator ();
		void Find	  ();
		void Compare  ();
		void Save     ();
		void Graph	  ();
		void Search	  (Stack* stk);
		Tree(const Tree& tree) = delete;
		Tree& operator= (Tree& tree);
};

void Menu  (char* filename);
void Split (char** str, char* symbols);
int  Read  (char** symbols, const char* filename);
void print (const char* buffer);
void scan  ();