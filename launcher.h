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
