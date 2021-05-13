#include "../Libraries/stack.h"
#include "../Libraries/node.h"
#include "../Libraries/akinator.h"
#include <sys/socket.h>


//-------------------------------------------------------------------------------

void Menu (char* filename)
{
	print ("\t\tHello! I am Akinator!\n");
	//system ("echo \"Hello! I am Akinator! Enter base and game mode\" | festival --tts");

	if(!filename)
	{
		print ("Enter name of file with base or type \"New\" to use empty base and choose game mode\n\n");
		scan  ();
	}
	else
		sprintf (buf, "%s", filename);

	Tree tree (buf);

	char mode[8] = "";

	while(true)
	{
		print  ("\t\t\tMENU\n"
				// "0 - Clear terminal\n"
				"1 - Guessing\n"
				"2 - Definition\n"
				"3 - Compare\n"
				"4 - Graph\n"
				"5 - Make a save\n"
				"6 - Exit\n");

		for(int i = 0; i < 8; i++)
			mode[i] = '\0';

		recv (sock, mode, 8, 0);
		mode[strlen(mode) - 2] = '\0';


		switch(mode[0])
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
				close (sock);
				return;
			default:
				print ("WRONG command. Try again\n");
		}
	}
}

//-------------------------------------------------------------------------------

Tree::Tree (const char* filename)
{
	if(!strcmp(filename, "New"))
	{
		head_ = (Node*) calloc (1, sizeof (Node));

		//system ("echo \"Please, enter first object\" | festival --tts");
		print ("Please, enter first object:\n");
		scan ();

		head_->data_ = (char*) calloc (strlen (buf) + 1, sizeof (char));

		sprintf (head_->data_, "%s", buf);
		return;
	}

	char *symbols = nullptr;
	int size = Read (&symbols, filename);

	if(size == 0)
	{
		//system ("echo \"Empty base!\" | festival --tts");
		print ("Your base is empty! I will use default base.\n");
		size = Read (&symbols, "default.base");
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
	print ("Do you want to save base? Type [Y/N]\n");
	//system ("echo \"Do you want to save base?\" | festival --tts ");
	scan ();

	if(!strcmp (buf, "Y"))
		Save ();

	assert (head_);

	head_->free_ ();

	print ("Bye!\n");
	//system ("echo \"Bye\" | festival --tts");
}

//-------------------------------------------------------------------------------

void Tree::Akinator () 
{
	Node* curr = head_;

	//system("clear");

	while(true)
	{
		sprintf (string, "Your object is {%s}? [Y\\N]\n", curr->data_);
		print (string);
		scan  ();

		if (!strcmp (buf, "N"))
		{
			if(curr->left_ == nullptr)
			{
				curr->new_obj_ ();
				curr = head_;
			}
			else
				curr = curr->left_;
		}
		else if(!strcmp (buf, "Y"))
		{	
			if(curr->right_ == nullptr)
			{
				//system ("echo \"Wohoo!!! We found it\" | festival --tts");
				print ("Wohoo!!! We found it!\n");
				curr = head_;
			} 
			else 
				curr = curr->right_;
		}
		else if(!strcmp (buf, "Quit"))
			break;
		else
		{
			//system ("echo \"WRONG COMMAND. Type 'Y' or 'N'\" | festival --tts");			
			print ("WRONG COMMAND. Type [Y\\N] or 'Quit':\n");
		}
	}
}

//-------------------------------------------------------------------------------

void Tree::Save ()
{	
	print ("Saving...\n");
	//system ("echo \"Where to save your database?\" | festival --tts");
	print ("Where to save your database?\n");

	scan ();

	if(!strcmp(buf, "server"))
		strcat(buf, "(copy)");

	FILE*   f_dump = fopen (buf, "w");
	assert (f_dump);

	head_->fprint_ (f_dump);

	fclose (f_dump);

	sprintf (string, "Saved in %s\n", buf);
	print (string);
}

//-------------------------------------------------------------------------------

void Tree::Find ()
{
	//system("clear");
	//system ("echo \"Enter object that you want to find\" | festival --tts");	
	print ("Enter object that you want to find:\n");

	Stack stk;

	Search (&stk);

	int size = stk.get_size ();

	Node* curr = head_;

	for(int i = 0; i < size; i++)
	{
		if(stk[i] == -1)
		{
			sprintf (string, "Not {%s}\n", curr->data_);
			print   (string);
			curr = curr->left_;
		}
		else if(stk[i] == 1)
		{
			sprintf (string, "{%s}\n", curr->data_);
			print   (string);
			curr = curr->right_;
		}
	}
	sprintf (string, "{%s}\n", curr->data_);
	print   (string);
}


//-------------------------------------------------------------------------------

void Tree::Compare ()
{
	//system("clear");
	//system("echo \"What to compare\" | festival --tts");
	print ("Enter objects that you want to compare:\n");

	Stack    stk1;
	Search (&stk1);
	int size1 = stk1.get_size ();

	Stack    stk2;
	Search (&stk2);
	int size2 = stk2.get_size ();

	print ("Objects are same in:\n");

	Node* curr = head_;

	int diff = 0;

	for( ; diff < size1 && diff < size2; diff++)
	{
		if(stk1[diff] == stk2[diff])
			curr = curr->compare_(stk1, &diff);
		else break;
	}

	Node* tmp = curr;

	print ("\nObjects are different in:\n");

	for(int m = diff; m < size1; m++)
		curr = curr->compare_(stk1, &m);

	print ("\n");

	curr = tmp;

	for(int n = diff; n < size2; n++)
		curr = curr->compare_(stk2, &n);

	print ("\n");
}

//-------------------------------------------------------------------------------

void  Tree::Search (Stack* stk)
{
	scan ();

	int mode = 0;

	head_->search_ (buf, stk, mode);
}

//-------------------------------------------------------------------------------

void Tree::Graph ()
{
	if(head_->data_ == nullptr)
	{
		print ("Tree is empty\n");
		return;
	}

	//system("clear");
	print ("Making graph...\n");
	print ("Where to save your graph? Type name of file\n");
	scan  ();

	char filename[SIZE + 4] = "";

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
			buf, head_->data_);

	if(head_->right_ && head_->left_)
	{
		head_->g_print_		  (f_graph);
		head_->left_ ->graph_ (f_graph);
		head_->right_->graph_ (f_graph);
	}
	else
		fprintf (f_graph, "\t}\n");

	fprintf (f_graph, "\tlabel = \"This is Akinator base\";\n}\n");

	fclose  (f_graph);

	sprintf (string, "dot -Tpng %s.dot -o %s.png", buf, buf);
	system  (string);
}

//-------------------------------------------------------------------------------

int Read (char** symbols, const char* filename)
{
	FILE* base = fopen (filename, "r");

	if(base == nullptr)
	{
		print ("Couldn't find file\n");
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


void print (const char* buffer)
{
	send(sock, buffer, strlen(buffer), 0);
}

void scan ()
{
	for(int i = 0; i < SIZE; i++)
		buf[i] = '\0';

	recv (sock, buf, SIZE, 0);
	buf[strlen(buf) - 2] = '\0';
}
