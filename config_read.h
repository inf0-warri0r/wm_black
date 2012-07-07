/*
*Author :Tharindra Galahena
*Project:wm_black - a simple window manager in linux 
*Date   :07/07/2012
*/

#ifndef __CONFIG__
#define __CONFIG__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct app{
	char name[40];
	char command[40];
	struct app *next;
} app;			

extern app *list;

char *get_command(int index);
int arg_count(char *s);
int splitter(char *com, char **args);
int read_file(char *file);
void run(char *com);

#endif
