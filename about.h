#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void drew_grid(Display *dpy, Window win, GC gc){
	int i, j;

	char *p[14] = {
				"------------------------------------------------------------------",
				"",
				"wm_black - window manager for linux",
				"------------------------------------",
				"",
				"wm_black is a very basic window manager for linux. it is created",
				"using c and X11 libraries.",
				"",
				"",
				"m0re inf0:",
				"         http://www.inf0warri0r.blogspot.com",
				"         tcg.galahena@gmail.com",
				"",
				"-------------------------------------------------------------------"
			};
	for(i = 0; i < 14; i++)
		XDrawString(dpy, win, gc, 40,  100 + 20*i, p[i], strlen(p[i]));

	XFlush(dpy);
}
int about()
{

	XSetErrorHandler(handler);
	XWindowAttributes attr;

	Display *dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		fputs("Cannot open display", stderr);
		exit(1);
	}
	int scr = XDefaultScreen(dpy);
	GC gc = XDefaultGC(dpy, scr);
	XColor col  = color(dpy,"green");

	Window win = XCreateSimpleWindow(dpy,
		XRootWindow(dpy, scr),
		DisplayWidth(dpy, scr)/2 - 300, 200, 500, 500, 3,
		col.pixel, XBlackPixel(dpy, scr));

	XStoreName(dpy, win, "xx");
	XMapWindow(dpy, win);
	XFlush(dpy);
	
	XSetForeground(dpy, gc, col.pixel);
	
	int co;
	char **fn = XListFonts(dpy, 
		"-misc-fixed-bold-r-normal--14-130-75-75-c-70-iso106*",
		1, &co);

	XFontStruct *font = XLoadQueryFont(dpy, fn[0]);

    	XSelectInput(dpy,
		win, 
		ExposureMask);

	XGetWindowAttributes(dpy, win, &attr);
	drew_grid(dpy, win, gc);
	XFlush(dpy);
	int loop = 1;
	XEvent ev;
	while (loop) {
		XNextEvent(dpy, &ev);
		
		switch (ev.type)
		{
			case Expose:
			{
				XGetWindowAttributes(dpy, win, &attr);
				drew_grid(dpy, win, gc);
				XFlush(dpy);
			}
			break;
		}
	}
	return 0;
}


