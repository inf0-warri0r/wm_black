#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
 
XWindowAttributes attr;
Display *dpy;
Window win;
GC gc;
int scr;
Atom WM_DELETE_WINDOW;
XEvent ev;
XEvent ev2;
KeySym keysym;
int loop;

char * home;

XColor color(char *color){
	Visual* default_visual = DefaultVisual(dpy, DefaultScreen(dpy));
	Colormap screen_colormap = XCreateColormap(dpy,
                                       DefaultRootWindow(dpy),
                                       default_visual,
                                       AllocNone);
	XColor system_color;
 
	XColor exact_color;

	Status rc = XAllocNamedColor(dpy,
									screen_colormap,
									color,
									&system_color,
									&exact_color);

	if (rc == 0) {
		fprintf(stderr,
				"XAllocNamedColor - allocation of 'red' color failed.\n");
	}
	return system_color;
}
int main(int argc, char *argv[])
{
	
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		fputs("Cannot open display", stderr);
		exit(1);
	}
	scr = XDefaultScreen(dpy);
	int display_width = DisplayWidth(dpy, scr);
	int display_height = DisplayHeight(dpy, scr);
	XColor col = color("green");
	
	win = XCreateSimpleWindow(dpy,
		XRootWindow(dpy, scr),
		display_width - 320 , 150, 300, 100, 3, /*XWhitePixel(dpy, scr),*/
		col.pixel, XBlackPixel(dpy, scr));
	XFlush(dpy);
	XGetWindowAttributes(dpy, win, &attr);
	printf("%d\n", attr.x);
	XStoreName(dpy, win, "clock");
 
	XMapWindow(dpy, win);
	XFlush(dpy);

	gc = XDefaultGC(dpy, scr);
	WM_DELETE_WINDOW = XInternAtom(dpy, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(dpy, win, &WM_DELETE_WINDOW, 1);
 
	XSetForeground(dpy, gc, col.pixel);

	XColor col2 = color("black");
	XSetBackground(dpy, gc, col2.pixel);
	
	int co;
	char **fn = XListFonts(dpy, "-misc-fixed-bold-r-normal--14-130-75-75-c-70-iso106*", 1, &co);

	XFontStruct *font = XLoadQueryFont(dpy, fn[0]);
    
	if(font == NULL) printf("aaaaaaa\n");
	XTextItem item;
	loop = 1;
	while (loop) {
		XGetWindowAttributes(dpy, win, &attr);
		time_t now;
		time(&now);
		char *msg1 = ctime(&now);
		XClearArea(dpy, win, 0, 0, 300, 100, False);		
		//XDrawString(dpy, win, gc, 90,  50, msg1, strlen(msg1));
		item.chars = msg1;
		item.nchars = strlen(msg1) - 1;
		item.delta = 10;
		item.font = font -> fid; 
		XDrawText(dpy, win, gc, 50, 50, &item, 1);
		XFlush(dpy);
		sleep(1);
	}
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
	return 1;
}

