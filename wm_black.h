/*
*Author :Tharindra Galahena
*Project:wm_black - a simple window manager in linux 
*Date   :07/07/2012
*/

#ifndef __BLACK__
#define __BLACK__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void run_apps();
void set_borders(Display *dpy, Window skip, char *back, char *border);
void send_message(Display *dpy, Window w_hide, Window w_des);

#endif
