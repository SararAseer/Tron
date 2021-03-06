#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename
          struct matrix * transform,
          struct matrix * pm,
          screen s
Returns:

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix -
               takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
         ident: set the transform matrix to the identity matrix -
         scale: create a scale matrix,
                then multiply the transform matrix by the scale matrix -
                takes 3 arguments (sx, sy, sz)
         translate: create a translation matrix,
                    then multiply the transform matrix by the translation matrix -
                    takes 3 arguments (tx, ty, tz)
         rotate: create a rotation matrix,
                 then multiply the transform matrix by the rotation matrix -
                 takes 2 arguments (axis, theta) axis should be x y or z
         apply: apply the current transformation matrix to the edge matrix
         display: clear the screen, then
                  draw the lines of the edge matrix to the screen
                  display the screen
         save: clear the screen, then
               draw the lines of the edge matrix to the screen
               save the screen to a file -
               takes 1 argument (file name)
         quit: end parsing

See the file script for an example of the file format

IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/


char ** parse_args( char * line , char* p)
{
  char ** args = calloc(sizeof(char*)+1, 50);
  char * d = strdup(line);
  int i = 0;
 
  while(args[i] = strsep(&d, p))
    {
      i++;
    }
  args[i]=NULL;
  return args;
}


void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * edges,
                  screen s) {
  FILE *f;
  clear_screen(s);
  color c;
  c.red=0;
  c.blue=255;
  c.green=0;
  if (strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);  
  char *line = malloc(fsize);
  fread(line, 1, fsize, f);
  fclose(f);
  char ** args =parse_args(line, "\n");
  int i=0;
  while(args[i] !=NULL){   
    if(strcmp(args[i],"line")==0){
      i++;
      char **args2= parse_args(args[i], " ");
      double x,y,z,x1,y1,z1;
      x=atof(args2[0]);
      y=atof(args2[1]);
      z=atof(args2[2]);
      x1=atof(args2[3]);
      y1=atof(args2[4]);
      z1=atof(args2[5]);
      add_edge(edges,x,y,z,x1,y1,z1);
    }
    else if(strcmp(args[i],"ident")==0){
      ident(transform);
    }
    else if(strcmp(args[i],"scale")==0){
      i++;
      char **args2= parse_args(args[i], " ");
      matrix_mult(make_scale(atof(args2[0]), atof(args2[1]),atof(args2[2])),transform);
    }
    else if(strcmp(args[i],"move")==0){
      i++;
      char **args2= parse_args(args[i], " ");
      matrix_mult(make_translate(atof(args2[0]), atof(args2[1]),atof(args2[2])),transform);

    }
    else if(strcmp(args[i],"rotate")==0){
      i++;
      char **args2= parse_args(args[i], " ");
      if(strcmp(args2[0],"x")==0){
	matrix_mult(make_rotX(atof(args2[1])),transform);
      }
      else if(strcmp(args2[0],"y")==0){
	matrix_mult(make_rotY(atof(args2[1])),transform);
      }
      else  if(strcmp(args2[0],"z")==0){
	matrix_mult(make_rotZ(atof(args2[1])),transform);
      }
    }
    else if(strcmp(args[i],"apply")==0){
      matrix_mult(transform,edges);
    }
    else if(strcmp(args[i],"display")==0){
      display(s);
     
    }
    else if(strcmp(args[i],"save")==0){
      clear_screen(s);
      draw_lines(edges, s, c);
      save_extension(s, "new.png");
    }
    else if(strcmp(args[i],"quit")==0){
      
      break;
    }
    i++;
  }
  print_matrix(edges);
}
  
