#include "../Libraries/stack.h"
#include "../Libraries/node.h"
#include "../Libraries/akinator.h"
#include <sys/socket.h>

extern void server (char* port_c, char* filename);

//-------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	if(!strcmp(argv[1], "-i"))
	{
		Menu(argv[2], stdin, stdout);
	}
	else if(!strcmp(argv[1], "-s"))
	{
		if(argv[1] == nullptr)
			printf("Type port to connect, for example \"7777\"\n");
		else server (argv[2], argv[3]);
	}
	else printf("Type game mode \"-i\" for interactive or \"-s\" for server mode\n");

	return 0;
}

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
	int size = Read (&symbols, filename, f_out);

	if(size == 0)
	{
		//system ("echo \"Empty base!\" | festival --tts");
		print (fout_, "Your base is empty! I will use default base.\n");
		
		size = Read (&symbols, "default.base", f_out);
	}

	char* tmp = symbols;

	char** str = (char**) calloc (size, sizeof (char*));
	Split (str, symbols);

	head_ = (Node*) calloc (1, sizeof (Node));

	int start = 0;

	head_->walk_ (str, &start);

	free (str);
	free (tmp);
}

//-------------------------------------------------------------------------------

Tree::~Tree ()
{
	print (fout_,"Do you want to save base? Type [Y/N]\n");
	//system ("echo \"Do you want to save base?\" | festival --tts ");
	scan  (fin_);

	if(!strcmp (buf, "Y"))
		Save ();

	assert (head_);

	head_->free_ ();

	print (fout_,"Bye!\n");
	//system ("echo \"Bye\" | festival --tts");
}

//-------------------------------------------------------------------------------

void Menu (char* filename, FILE* f_in, FILE* f_out)
{
	print (f_out, "\t\tHello! I am Akinator!\n");
	//system ("echo \"Hello! I am Akinator! Enter base and game mode\" | festival --tts");

	if(!filename)
	{
		print (f_out, "Enter name of file with base or type \"New\" to use empty base and choose game mode\n\n");
		scan  (f_in);
	}
	else
		sprintf (buf, "%s", filename);

	Tree tree (buf, f_in, f_out);

	while(true)
	{
		print  (f_out, "\t\t\tMENU\n"
				// "0 - Clear terminal\n"
				"1 - Guessing\n"
				"2 - Definition\n"
				"3 - Compare\n"
				"4 - Graph\n"
				"5 - Make a save\n"
				"6 - Exit\n");

		scan (f_in);

		switch(buf[0])
		{
			case(Clear):
				//system("clear");
				break;
			case(Guessing):
				tree.Akinator ();
				break;
			case(Definition):
				tree.Find ();
				break;
			case(Compare):
				tree.Compare ();
				break;
			case(Graph):
				tree.Graph ();
				break;
			case(Save):
				tree.Save ();
				break;
			case(Exit):
				return;
			default:
				print (f_out, "WRONG command. Try again\n");
		}
	}
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

	char filename[SIZE*3] = "";

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

int Read (char** symbols, const char* filename, FILE* f_out)
{
	FILE* base = fopen (filename, "r");

	if(base == nullptr)
	{
		print (f_out,"Couldn't find file\n");
		return 0;
	}

	fseek (base, 0, SEEK_END);
	int size = ftell (base);
	rewind (base);

	if(size > 0)
	{
		*symbols = (char*) calloc (size, sizeof (char));
		fread (*symbols, sizeof (char), size, base);
	}
	fclose (base);

	return size;
}

//-------------------------------------------------------------------------------

void Split (char** str, char* symbols)
{
	symbols = strtok (symbols, " \n");

	int cnt = 0;

	while(symbols != NULL)
	{
		str[cnt++] = symbols;
		symbols    = strtok (nullptr, " \n");
	}
}

//-------------------------------------------------------------------------------

void print (FILE* f_out, const char* buffer)
{
	fprintf (f_out, "%s", buffer);
	fflush  (f_out);
}

//-------------------------------------------------------------------------------

void scan  (FILE* fin_)
{
	for(int i = 0; i < SIZE; i++)
		buf[i] = '\0';

	int descr = fileno(fin_);

 	if(descr == 0)
	{
		fgets(buf, SIZE, stdin);
		buf[strlen (buf) - 1] = '\0';
	}
	else
	{
		recv (descr, buf, SIZE, 0);
		buf[strlen (buf) - 2] = '\0';
	}

}
