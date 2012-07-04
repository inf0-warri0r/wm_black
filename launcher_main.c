#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "share.h"
#include "grapics.h"
#include "config_read.h"
#include "launcher.h"
	
int main(int argc, char *argv[])
{
	XWindowAttributes attr;
	
	int count = read_file("apps");
	if(count == 0) return 0;
	int x = 5;
	if(x > count) x = count;
	int y = count / x;
	if(count > y * x) y++;
	
	Display *dpy = XOpenDisplay(NULL);
	if(!(dpy = XOpenDisplay(0x0))) return 1;

	int scr = XDefaultScreen(dpy);
	GC gc = XDefaultGC(dpy, scr);
	int width  = 120*x;
	int height = 70*y;
	XColor col  = color(dpy,"green");
	XColor col2 = color(dpy, "black");

	Window win = XCreateSimpleWindow(dpy,
				XRootWindow(dpy, scr),
				DisplayWidth(dpy, scr)/2 -60*x, 200, 
				120*x, 70*y, 
				3,
				col.pixel, XBlackPixel(dpy, scr));

	XStoreName(dpy, win, "lon");	
	
	XMapWindow(dpy, win);
	
	Atom WM_DELETE_WINDOW = XInternAtom(dpy, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(dpy, win, &WM_DELETE_WINDOW, 1);
 
	XSetForeground(dpy, gc, col.pixel);
	
	XFontStruct *font = get_font(dpy);

    	XSelectInput(dpy,
		win, 
		ExposureMask | KeyPressMask | ButtonPressMask |SubstructureNotifyMask);

	XSetErrorHandler(handler);
	XGetWindowAttributes(dpy, win, &attr);
	drew_grid(dpy, win, gc, x, y, attr.width, count);
	int loop = 1;
	XEvent ev;
	while (loop) {
		XNextEvent(dpy, &ev);
		
		switch (ev.type)
		{
			case Expose:
			{
				XGetWindowAttributes(dpy, win, &attr);
				drew_grid(dpy, win, gc, x, y, attr.width, count);
			}
			break;
			case ButtonPress:
			{
				XGetWindowAttributes(dpy, win, &attr);
				int index = get_index(ev.xbutton.x_root,
							ev.xbutton.y_root,
							attr.x,
							attr.y, x);
				if(index < count && index > -1){
					
					run(get_command(index));
				}
			}
			break;
			case ClientMessage:
				if (ev.xclient.data.l[0] == WM_DELETE_WINDOW)
					loop = 0;
			break;
		}
	}
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
	return 0;
}


