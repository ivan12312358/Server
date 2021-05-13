#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

extern const int SIZE;
extern int sock;
extern char buf[];
extern char string[];
extern void print (const char* buffer);
extern void scan  ();

class Tree;
struct Stack;

struct Node
{
	char* data_  = nullptr;
	Node* left_  = nullptr;
	Node* right_ = nullptr;

	public:
		void new_obj_ ();
		void free_	  ();
		void fprint_  (FILE* f_dump);
		void search_  (char* str, Stack* stk, int& mode);
		void walk_	  (char** str, int* cnt);
		void graph_	  (FILE* f_graph);
		void g_print_ (FILE* f_graph);
		Node* compare_(Stack& stk, int* diff);
};
