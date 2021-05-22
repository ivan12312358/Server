#include "../Libraries/files.h"
#include "../Libraries/tree.h"
#include "../Libraries/node.h"
#include "../Libraries/stack.h"

extern const int SIZE;
extern char buf[];

//-------------------------------------------------------------------------------

Tree::Tree (const char* filename, FILE* f_in, FILE* f_out) :
	fin_  (f_in),
	fout_ (f_out)
{
	if(!strcmp(filename, "New"))
	{
		head_ = (Node*) calloc (1, sizeof (Node));

		//system ("echo \"Please, enter first object\" | festival --tts");
		print (f_out, "Please, enter first object:\n"); 
		scan  (f_in);

		head_->data = (char*) calloc (strlen (buf) + 1, sizeof (char));

		sprintf (head_->data, "%s", buf);
		return;
	}

	char *symbols = nullptr;
	int size = read (&symbols, filename, f_out);

	if(size == 0)
	{
		//system ("echo \"Empty base!\" | festival --tts");
		print (fout_, "Your base is empty! I will use default base.\n");
		
		size = read (&symbols, "default.base", f_out);
	}

	char* tmp = symbols;

	char** str = (char**) calloc (size, sizeof (char*));
	split (str, symbols);

	head_ = (Node*) calloc (1, sizeof (Node));

	int start = 0;

	head_->walk_ (str, &start);

	free (str);
	free (tmp);
}

//-------------------------------------------------------------------------------

Tree::~Tree ()
{
	assert (head_);

	head_->free_ ();

	print (fout_,"Bye!\n");
	//system ("echo \"Bye\" | festival --tts");
}

//-------------------------------------------------------------------------------

void Tree::Akinator () 
{
	Node* curr = head_;

	//system("clear");

	while(true)
	{
		fprintf (fout_, "Your object is {%s}? [Y\\N]\n", curr->data);

		print (fout_, "");		
		scan  (fin_);

		if (!strcmp (buf, "N"))
		{
			if(curr->left == nullptr)
			{
				curr->new_obj_ (fin_, fout_);
				curr = head_;
			}
			else
				curr = curr->left;
		}
		else if(!strcmp (buf, "Y"))
		{	
			if(curr->right == nullptr)
			{
				//system ("echo \"Wohoo!!! We found it\" | festival --tts");
				print (fout_, "Wohoo!!! We found it!\n");
				curr = head_;
			} 
			else 
				curr = curr->right;
		}
		else if(!strcmp (buf, "Quit"))
			break;
		else
		{
			//system ("echo \"WRONG COMMAND. Type 'Y' or 'N'\" | festival --tts");			
			print (fout_, "WRONG COMMAND. Type [Y\\N] or 'Quit':\n");
		}
	}
}

//-------------------------------------------------------------------------------

void Tree::Save ()
{	
	print (fout_, "Saving...\n");
	//system ("echo \"Where to save your database?\" | festival --tts");
	print (fout_, "Where to save your database?\n");
	scan  (fin_);

	strcat (buf, ".base");

	FILE*   f_dump = fopen (buf, "w");
	assert (f_dump);

	head_->fprint_ (f_dump);

	fclose (f_dump);

	fprintf (fout_, "Saved in %s\n", buf);
	print   (fout_, "");
}

//-------------------------------------------------------------------------------

void Tree::Find ()
{
	//system("clear");
	//system ("echo \"Enter object that you want to find\" | festival --tts");	
	print (fout_,"Enter object that you want to find:\n");

	Stack stk;

	Search (&stk);

	int size = stk.get_size ();

	Node* curr = head_;

	for(int i = 0; i < size; i++)
	{
		if(stk[i] == -1)
		{
			fprintf (fout_, "Not {%s}\n", curr->data);
			print   (fout_, "");
			curr = curr->left;
		}
		else if(stk[i] == 1)
		{
			fprintf (fout_, "{%s}\n", curr->data);
			print   (fout_, "");
			curr = curr->right;
		}
	}
	fprintf (fout_, "{%s}\n", curr->data);
	print   (fout_, "");
}


//-------------------------------------------------------------------------------

void Tree::Compare ()
{
	//system("clear");
	//system("echo \"What to compare\" | festival --tts");
	print (fout_,"Enter objects that you want to compare:\n");

	Stack    stk1;
	Search (&stk1);
	int size1 = stk1.get_size ();

	Stack    stk2;
	Search (&stk2);
	int size2 = stk2.get_size ();

	print (fout_,"Objects are same in:\n");

	Node* curr = head_;

	int diff = 0;

	for( ; diff < size1 && diff < size2; diff++)
	{
		if(stk1[diff] == stk2[diff])
			curr = curr->compare_(stk1, &diff, fout_);
		else break;
	}

	Node* tmp = curr;

	print (fout_,"\nObjects are different in:\n");

	for(int m = diff; m < size1; m++)
		curr = curr->compare_(stk1, &m, fout_);

	print (fout_,"\n");

	curr = tmp;

	for(int n = diff; n < size2; n++)
		curr = curr->compare_(stk2, &n, fout_);

	print (fout_,"\n");
}

//-------------------------------------------------------------------------------

void  Tree::Search (Stack* stk)
{
	scan (fin_);

	int mode = 0;

	head_->search_ (buf, stk, mode);
}

//-------------------------------------------------------------------------------

void Tree::Graph ()
{
	if(head_->data == nullptr)
	{
		print (fout_,"Tree is empty\n");
		return;
	}

	//system("clear");
	print (fout_, "Making graph...\n");
	print (fout_, "Where to save your graph? Type name of file\n");
	scan  (fin_);

	char filename[512] = "";

	sprintf(filename, "%s.dot", buf);

	FILE*   f_graph = fopen (filename, "w");
	assert (f_graph);

	fprintf (f_graph, "digraph Tree {\n"
			"\tnode [shape = circle, width = 1]\n"
			"\tedge [color = \"blue\"];\n"
			"\tnodesep = 1.5\n"
			"\tbase [label = \"%s.dot\", fillcolor = yellow, style = \"rounded,filled\","
			"shape = doubleoctagon, fontsize = 15]\n\n"

			"\tsubgraph cluster\n\t{\n"
			"\t\tlabel = \"head\";\n"
			"\t\t%s [fillcolor = royalblue, style = \"rounded,filled\", shape = diamond]\n",
			buf, head_->data);

	if(head_->right && head_->left)
	{
		head_->g_print_		  (f_graph);
		head_->left ->graph_ (f_graph);
		head_->right->graph_ (f_graph);
	}
	else
		fprintf (f_graph, "\t}\n");

	fprintf (f_graph, "\tlabel = \"This is Akinator base\";\n}\n");

	fclose  (f_graph);

	sprintf (filename, "dot -Tpng %s.dot -o %s.png", buf, buf);
	system  (filename);
}

//-------------------------------------------------------------------------------

