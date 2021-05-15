
class Stack;
class Node;

class Tree
{
	FILE* fin_  = nullptr;
	FILE* fout_ = nullptr;
	Node* head_ = nullptr;

	public:
		Tree (const char* filename, FILE* f_in, FILE* f_out);
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
