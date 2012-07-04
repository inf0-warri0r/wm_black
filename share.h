#ifndef __SHA__
#define __SHA__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int handler(Display *d, XErrorEvent *ev);
char *full_path(char *file);
Window* get_all_windows(Display *dpy);
Window find(Display *dpy, char *name);
int get_simb_num();

#endif
