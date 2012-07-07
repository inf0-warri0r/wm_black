/*
*Author :Tharindra Galahena
*Project:wm_black - a simple window manager in linux 
*Date   :07/07/2012
*/

#ifndef __GRAPICS__
#define __GRAPICS__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct pixmap_{
	int width;
	int height;
	Pixmap px;
} pixmap_;

XColor color(Display *, char *);
pixmap_ *load_bitmap(Display *, char *);
XFontStruct *get_font(Display *);

#endif
