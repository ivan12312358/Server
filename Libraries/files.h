//do not forget about pragmas
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/socket.h>


void split (char** str, char* symbols);
void print (FILE* f_out, const char* buffer);
void scan  (FILE* fin_);
int  read  (char** symbols, const char* filename, FILE* f_out);
