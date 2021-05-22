#include "../Libraries/files.h"

const int SIZE = 128;
char buf[SIZE] = "";

//-------------------------------------------------------------------------------

// should be part of onegin library
void split (char** str, char* symbols)
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

void scan (FILE* fin_)
{
	for(int i = 0; i < SIZE; i++)
		buf[i] = '\0';

	int descr = fileno(fin_);

 	if(descr == 0)
	{
		fgets(buf, SIZE, fin_);
		buf[strlen (buf) - 1] = '\0';
	}
	else
	{
		recv (descr, buf, SIZE, 0);
		buf[strlen (buf) - 2] = '\0';
	}
}

//-------------------------------------------------------------------------------

int read (char** symbols, const char* filename, FILE* f_out)
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
