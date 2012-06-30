#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "grapics.c"

int main(int argc, char *argv[])
{
	Display *dpy;
	if(!(dpy = XOpenDisplay(0x0))) return 1;
	int scr = XDefaultScreen(dpy);
	int display_width = DisplayWidth(dpy, scr);
	GC gc = XDefaultGC(dpy, scr);

	XColor col = color(dpy, "green");
	Window win = XCreateSimpleWindow(dpy,
		XRootWindow(dpy, scr),
		display_width - 320 , 150, 300, 100, 3,
		col.pixel, XBlackPixel(dpy, scr));
	XStoreName(dpy, win, "clock");
	XSetForeground(dpy, gc, col.pixel);
	XMapWindow(dpy, win);
	
	Atom delete_message = XInternAtom(dpy, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(dpy, win, &delete_message, 1);

	int co;
	char **fn = XListFonts(dpy, 
		"-misc-fixed-bold-r-normal--14-130-75-75-c-70-iso106*", 
		1, &co);
	if(co == 0) fn = XListFonts(dpy, "*", 1, &co);

	XFontStruct *font = XLoadQueryFont(dpy, fn[0]);
	XTextItem item;
	item.delta = 10;
	item.font = font -> fid;
	time_t now;
 
	while (1) {
		time(&now);
		char *msg1 = (char *)ctime(&now);
		XClearArea(dpy, win, 0, 0, 300, 100, False);		
		item.chars = msg1;
		item.nchars = strlen(msg1) - 1;
		XDrawText(dpy, win, gc, 50, 50, &item, 1);
		XFlush(dpy);
		sleep(1);
	}
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
	return 1;
}

