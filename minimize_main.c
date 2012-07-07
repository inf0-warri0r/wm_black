/*
*Author :Tharindra Galahena
*Project:wm_black - a simple window manager in linux 
*Date   :07/07/2012
*/

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>

#include "share.h"
#include "grapics.h"
#include "minimize.h"

int main(){

	Display *dpy = XOpenDisplay(NULL);
	if(!(dpy = XOpenDisplay(0x0))) return 1;

	XEvent ev;
	int scr = XDefaultScreen(dpy);
	int display_width = DisplayWidth(dpy, scr);
	int display_height = DisplayHeight(dpy, scr);
	XColor col = color(dpy, "green");

	Window win = XCreateSimpleWindow(dpy,
					XRootWindow(dpy, scr),
					display_width / 2  - 100 , 
					display_height - 155, 
					200, 50, 
					3,
					col.pixel, XBlackPixel(dpy, scr));

	XStoreName(dpy, win, "mini");
	XSelectInput(dpy, win, 
			ExposureMask | KeyPressMask | ButtonPressMask | SubstructureNotifyMask);
	

	Window win_xx = find(dpy, "xx");
	Window win_clock = find(dpy, "clock");
	Window win_mini = find(dpy, "mini");

	Atom hide = XInternAtom(dpy, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(dpy, win, &hide, 1);
	XMapWindow(dpy, win);
	GC gc = XDefaultGC(dpy, scr);
	XSetForeground(dpy, gc, col.pixel);
	
	XFontStruct *font = get_font(dpy);
	
	char *msg1 = "";

	XTextItem item;
	item.delta = 10;
	item.font = font -> fid; 
	item.chars = msg1;
	item.nchars = strlen(msg1);

	XDrawText(dpy, win, gc, 20,  20, &item, 1);
	XSetErrorHandler(handler);
	while (1){
		XNextEvent(dpy, &ev);
		switch (ev.type)
		{
			case Expose:
			{
				if(count > 0) repaint(dpy, win, font, gc);
			}
			break;
			case ClientMessage:
			if (ev.xclient.data.l[0] == hide){
				if(ev.xclient.data.l[2] != 0){
					Window n = (Window)ev.xclient.data.l[2];
					if(add(n)){
						repaint(dpy, win, font, gc);
						XMoveWindow(dpy, n, 1500, 100);
						XRaiseWindow(dpy, n);
					}	
				}	
			}
			break;
			case ButtonPress:
			{
				if(count > 0){
					XWindowAttributes attr;
					XGetWindowAttributes(dpy, win, &attr);
					int x = ev.xbutton.x_root - attr.x;
					int index =  x / 200;
					if(index < count && index > -1){
						Window w = get_window(index);
						if(w != 0){
							XMapWindow(dpy, w);
							XMoveWindow(dpy, w, 100, 100);
							XRaiseWindow(dpy, w);							
							count--;
							repaint(dpy, win, font, gc);
						}
					}
				}
			}
			break;
		}
	}
}
