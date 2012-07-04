#ifndef __BASE__
#define __BASE__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void draw_text (Display *, GC, Window, int, int, XFontStruct *);
void draw_lines(Display *, GC, Window, int, int);
void draw_icons(Display *dpy, GC g, Window win, int width, pixmap_ *pm);

#endif
