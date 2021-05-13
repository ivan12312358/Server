#include "../Libraries/node.h"
#include "../Libraries/stack.h"

int SLSH_CNT = 0;

void Node::walk_(char** str, int* cnt)
{
	this->data_ = (char*) calloc (strlen (str[*cnt]) + 1, sizeof(char));
	sprintf (this->data_, "%s", str[(*cnt)++]);

	if(str[(*cnt)][0] == '{')
	{
		(*cnt)++;

		if(str[(*cnt)][0] != '}')
		{
			left_ = (Node*) calloc (1, sizeof (Node));
			left_->walk_(str, cnt);
			(*cnt)++;
		}
		else return;

		right_ = (Node*) calloc (1, sizeof (Node));
		right_->walk_(str, cnt);
		(*cnt)++;
	}
 }

//-------------------------------------------------------------------------------

void Node::new_obj_ ()
{
	left_ 		  = (Node*) calloc(1, sizeof (Node));
	left_->data_  = (char*) calloc(strlen (data_) + 1, sizeof (char));

	sprintf (left_->data_, "%s", data_);
	free	(data_);

	//system ("echo \"What is your object?\" | festival --tts");
	print ("What is your object?\n");
	scan  ();

	right_ 		  = (Node*) calloc (1, sizeof (Node));
	right_->data_ = (char*) calloc (strlen (buf) + 1, sizeof (char));

	sprintf (right_->data_, "%s", buf);

	sprintf (string, "How does a {%s} differ from a {%s}?\n", right_->data_, left_->data_);
	print (string);
	scan  ();

	data_ = (char*) calloc (strlen (buf) + 1, sizeof (char));
	sprintf (data_, "%s", buf);
}

//-------------------------------------------------------------------------------

void Node::free_ ()
{
	if(left_)  left_ ->free_ ();
	if(right_) right_->free_ ();
	if(data_)  free (data_);
			   free (this);
}

//-------------------------------------------------------------------------------

void Node::fprint_ (FILE* f_dump)
{
	assert(f_dump);
	assert(this->data_);

	fprintf(f_dump, "%*s%s\n", SLSH_CNT*4, "", this->data_);

	if(left_ == nullptr && right_ == nullptr)
	{
		fprintf(f_dump, "%*s{\n", SLSH_CNT*4, "");
		fprintf(f_dump, "%*s}\n", SLSH_CNT*4, "");
	}

	if(left_ != nullptr)
	{
		fprintf(f_dump, "%*s{\n", SLSH_CNT*4, "");
		SLSH_CNT++;
		left_->fprint_(f_dump);

		if(right_ != nullptr)
			right_->fprint_(f_dump);

		SLSH_CNT--;
		fprintf(f_dump, "%*s}\n", SLSH_CNT*4, "");
	}
}

//-------------------------------------------------------------------------------

void Node::search_ (char* str, Stack* stk, int& mode)
{
	if(!strcmp(str, data_))
	{
		mode = 1;
		return;
	}

	if(left_ && !mode)
	{
		stk->push(-1);
		left_->search_(str, stk, mode);
	
		if(!mode)
			stk->pop();
	}

	if(right_ && !mode)
	{
		stk->push(1);
		right_->search_(str, stk, mode);
	
		if(!mode)
			stk->pop();
	}
}

//-------------------------------------------------------------------------------

Node* Node::compare_(Stack& stk, int* diff)
{
	if(stk[*diff] == -1) print ("Not ");

	sprintf (string, "{%s}  ", data_);
	print   (string);

	if(stk[*diff] == -1) return left_;
	else 				 return right_;
}

//-------------------------------------------------------------------------------

void Node::graph_ (FILE* f_graph)
{
	if(left_ == nullptr || right_ == nullptr)
		return;

	fprintf  (f_graph, "\tsubgraph cluster\n\t{\n");
	g_print_ (f_graph);

	 left_->graph_ (f_graph);
	right_->graph_ (f_graph);
}

//-------------------------------------------------------------------------------

void Node::g_print_ (FILE* f_graph)
{
	fprintf (f_graph, "\t\t%s ->\n\t\t{\n"
	"\t\t\t%s [shape = \"box3d\", fillcolor = red, style = \"filled\"];\n" 
	"\t\t\t%s [shape = \"box3d\", fillcolor = green, style = \"filled\"];\n\t\t}\n\t}\n",
																			    data_, 
																		 left_->data_,
																		right_->data_);
}