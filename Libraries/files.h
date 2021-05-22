//do not forget about pragmas
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/socket.h>

/*!
 \brief Separate input text(base) into lines
 \param [in] symbols - input text(base)
 \param [in] str - text(base) separated into lines
*/
void split (char** str, char* symbols);

/*!
 \brief Print message in console
 \param [in] f_out - output stream
 \param [in] buffer - message
*/
void print (FILE* f_out, const char* buffer);

/*!
 \brief Write input stream in "buf"
 \param [in] f_in - input stream
*/
void scan  (FILE* fin_);

/*!
 \brief Write text(base) in "symbols"
 \param [in] symbols - input text(base)
 \param [in] filename - name of file 
 \param [in] f_out - output stream
*/
int  read  (char** symbols, const char* filename, FILE* f_out);
