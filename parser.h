#ifndef PARSER_H
#define PARSER_H

#include "matrix.h"
#include "ml6.h"

void parse_file ( char * filename, 
		  struct matrix * transform, 
		  struct matrix * edges,
		  screen s);
char ** parse_args( char * line , char* p);


#endif
