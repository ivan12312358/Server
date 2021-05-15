#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

extern const int SIZE;
extern char buf[];
extern void print (FILE* f_out, const char* buffer);
extern void scan  (FILE* f_in);

class Tree;
struct Stack;

struct Node
{
	char* data  = nullptr;
	Node* left  = nullptr;
	Node* right = nullptr;
	Node (char* data);

	public:
		void new_obj_ (FILE* f_in, FILE* f_out);
		void free_	  ();
		void fprint_  (FILE* f_dump);
		void search_  (char* str, Stack* stk, int& mode);
		void walk_	  (char** str, int* cnt);
		void graph_	  (FILE* f_graph);
		void g_print_ (FILE* f_graph);
		Node* compare_(Stack& stk, int* diff, FILE* f_out);
};
