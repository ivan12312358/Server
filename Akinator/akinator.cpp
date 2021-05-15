#include "../Libraries/akinator.h"
#include "../Libraries/files.h"
#include "../Libraries/tree.h"

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
