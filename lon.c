#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "sha.c"
#include "grapics.c"
#include "config_read.c"
	
void drew_grid(Display *dpy, Window win, GC gc, int x, int y, int width, int N){
	int i, j;
	for(i = 0; i < y; i++){
		XDrawLine(dpy, win, gc, 0, 70*(i + 1) , width, 70*(i + 1));
	}
	int n = 0;
	app *p = list;
	for(j = 0; j < y; j++){
		for(i = 0; i < x; i++){
			if(n >= N) break;
			XDrawString(dpy, win, gc,
				120 * i + 10,  70 * j + 35,
				p -> name, strlen(p -> name));
			XFlush(dpy);
			XDrawLine(dpy, win, gc, 
					120 * (i + 1), 70 * j,
					120 * (i + 1), 70 * (j + 1));	
			XFlush(dpy);
			n++;
			p = p -> next;
		}
		if(n >= N) break;
	}
	XFlush(dpy);
}
int get_index(int x1, int y1, int x2, int y2, int width){
	int x =  (x1 - x2) / 120;
	int y =  (y1 - y2) /  70;
	return (y * width + x);
}
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
	if (dpy == NULL) {
		fputs("Cannot open display", stderr);
		exit(1);
	}
	int scr = XDefaultScreen(dpy);
	GC gc = XDefaultGC(dpy, scr);
	int width  = 120*x;
	int height = 70*y;
	XColor col  = color(dpy,"green");
	XColor col2 = color(dpy, "black");

	Window win = XCreateSimpleWindow(dpy,
		XRootWindow(dpy, scr),
		DisplayWidth(dpy, scr)/2 -60*x, 200, 120*x, 70*y, 3,
		col.pixel, XBlackPixel(dpy, scr));
	XStoreName(dpy, win, "lon");	
	
	XMapWindow(dpy, win);
	
	Atom WM_DELETE_WINDOW = XInternAtom(dpy, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(dpy, win, &WM_DELETE_WINDOW, 1);
 
	XSetForeground(dpy, gc, col.pixel);
	
	int co;
	char **fn = XListFonts(dpy, 
		"-misc-fixed-bold-r-normal--14-130-75-75-c-70-iso106*",
		1, &co);

	XFontStruct *font = XLoadQueryFont(dpy, fn[0]);
    	XSelectInput(dpy,
		win, 
		ExposureMask | KeyPressMask | ButtonPressMask 
		|SubstructureNotifyMask);

	
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


