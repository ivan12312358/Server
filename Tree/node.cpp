#include "../Libraries/files.h"
#include "../Libraries/node.h"
#include "../Libraries/stack.h"

extern const int SIZE;
extern char buf[];
int SLSH_CNT = 0;

Node::Node (char* data) : 
	data(data) 
{
	left = nullptr;
    right = nullptr;
}

//-------------------------------------------------------------------------------

void Node::walk_(char** str, int* cnt)
{
	this->data = (char*) calloc (strlen (str[*cnt]) + 1, sizeof(char));
	sprintf (this->data, "%s", str[(*cnt)++]);

	if(str[(*cnt)][0] == '{')
	{
		(*cnt)++;

		if(str[(*cnt)][0] != '}')
		{
			left = (Node*) calloc (1, sizeof (Node));
			left->walk_(str, cnt);
			(*cnt)++;
		}
		else return;

		right = (Node*) calloc (1, sizeof (Node));
		right->walk_(str, cnt);
		(*cnt)++;
	}
 }

//-------------------------------------------------------------------------------

void Node::new_obj_ (FILE* f_in, FILE* f_out)
{
    left  = new Node (data);

	//system ("echo \"What is your object?\" | festival --tts");
    print (f_out, "What is your object?\n");
    scan  (f_in);

    right =  new Node (strdup (buf));

    fprintf (f_out, "How does a {%s} differ from a {%s}?\n", right->data, left->data);
    print (f_out, "");
    scan  (f_in);

    data = strdup(buf);
}

//-------------------------------------------------------------------------------

void Node::free_ ()
{
	if(left)  left ->free_ ();
	if(right) right->free_ ();
	if(data)  free (data);
			  free (this);
}

//-------------------------------------------------------------------------------

void Node::fprint_ (FILE* f_dump)
{
	assert(f_dump);
	assert(this->data);

	fprintf(f_dump, "%*s%s\n", SLSH_CNT*4, "", this->data);

	if(left == nullptr && right == nullptr)
	{
		fprintf(f_dump, "%*s{\n", SLSH_CNT*4, "");
		fprintf(f_dump, "%*s}\n", SLSH_CNT*4, "");
	}

	if(left != nullptr)
	{
		fprintf(f_dump, "%*s{\n", SLSH_CNT*4, "");
		SLSH_CNT++;
		left->fprint_(f_dump);

		if(right != nullptr)
			right->fprint_(f_dump);

		SLSH_CNT--;
		fprintf(f_dump, "%*s}\n", SLSH_CNT*4, "");
	}
}

//-------------------------------------------------------------------------------

void Node::search_ (char* str, Stack* stk, int& mode)
{
	if(!strcmp(str, data))
	{
		mode = 1;
		return;
	}

	if(left && !mode)
	{
		stk->push(-1);
		left->search_(str, stk, mode);
	
		if(!mode)
			stk->pop();
	}

	if(right && !mode)
	{
		stk->push(1);
		right->search_(str, stk, mode);
	
		if(!mode)
			stk->pop();
	}
}

//-------------------------------------------------------------------------------

Node* Node::compare_(Stack& stk, int* diff, FILE* f_out)
{
	if(stk[*diff] == -1) print (f_out, "Not ");

	fprintf (f_out, "{%s}  ", data);
	print   (f_out, "");

	if(stk[*diff] == -1) return left;
	else 				 return right;
}

//-------------------------------------------------------------------------------

void Node::graph_ (FILE* f_graph)
{
	if(left == nullptr || right == nullptr)
		return;

	fprintf  (f_graph, "\tsubgraph cluster\n\t{\n");
	g_print_ (f_graph);

	 left->graph_ (f_graph);
	right->graph_ (f_graph);
}

//-------------------------------------------------------------------------------

void Node::g_print_ (FILE* f_graph)
{
	fprintf (f_graph, "\t\t%s ->\n\t\t{\n"
	"\t\t\t%s [shape = \"box3d\", fillcolor = red, style = \"filled\"];\n"
	"\t\t\t%s [shape = \"box3d\", fillcolor = green, style = \"filled\"];\n\t\t}\n\t}\n",
																			   data, 
																		 left->data,
																		right->data);
}