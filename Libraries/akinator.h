#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

enum game_modes
{
	Clear = '0',
	Guessing, 
	Definition,
	Compare,
	Graph,
	Save,
	Exit
};


extern char buf[];
extern const int SIZE;
extern void server(char* port_c, char* filename);

/*!
 \brief Allow to choose game mode
 \param [in] filename - name of file
 \param [in] f_in - input stream
 \param [in] f_out - output stream
*/
void Menu (char* filename, FILE* f_in, FILE* f_out);
