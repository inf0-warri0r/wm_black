/*
*Author :Tharindra Galahena
*Project:wm_black - a simple window manager in linux 
*Date   :07/07/2012
*/

#ifndef __LAUNCHER__
#define __LAUNCHER__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void drew_grid(Display *, Window, GC, int, int, int, int);
int get_index(int, int, int, int, int);

#endif
