#ifndef __MINIMIZE__
#define __MINIMIZE__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct window{
	Window w;
	struct window *next;
} window;

extern count;

int check(Window w);
int add(Window w);
Window get_window(int n);
void repaint(Display *dpy, Window win, XFontStruct *font, GC gc);

#endif
