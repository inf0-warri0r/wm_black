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
#include "about.h"
#include "base_win.h"

int main(int argc, char *argv[])
{
	XSetErrorHandler(handler);
	XWindowAttributes attr;

	Display *dpy = XOpenDisplay(NULL);
	if(!(dpy = XOpenDisplay(0x0))) return 1;

	int scr = XDefaultScreen(dpy);
	int display_width = DisplayWidth(dpy, scr);
	int display_height = DisplayHeight(dpy, scr);

	XColor col = color(dpy, "green");
	Window win = XCreateSimpleWindow(dpy,
		XRootWindow(dpy, scr),
		0, 0, display_width + 10, display_height + 10, 2,
		XBlackPixel(dpy, scr), XBlackPixel(dpy, scr));

	XGetWindowAttributes(dpy, win, &attr);
	XStoreName(dpy, win, "xx");
 
	XSelectInput(dpy, win, 
		ExposureMask | KeyPressMask | ButtonPressMask | SubstructureNotifyMask);

	GC gc = XDefaultGC(dpy, scr);
 
	col = color(dpy, "green");
	XSetForeground(dpy, gc, col.pixel);

	col = color(dpy, "black");
	XSetBackground(dpy, gc, col.pixel);

	XFontStruct *font = get_font(dpy);

	XTextItem item;
	item.delta = 10;
	item.font = font -> fid; 
	XMapWindow(dpy, win);
	XEvent ev;
	int pid_about = 0;
	int loop = 1;
	while (loop) {
		XNextEvent(dpy, &ev);
		switch (ev.type)
		{
			case Expose:
			{
				XGetWindowAttributes(dpy, win, &attr);
				draw_text(dpy, gc, win, attr.width, attr.height, font);
				draw_lines(dpy, gc, win, attr.width, attr.height);
				draw_icons(dpy, gc, win, attr.width, 
						load_bitmap(dpy, "icon.bmp"));
				pixmap_ *cen = load_bitmap(dpy, "background.bmp");
				int h = cen -> height;
				int w = cen -> width;
				XCopyPlane(dpy, cen -> px, win, gc, 0, 0, w, h,
								attr.width/2 - w/2, attr.height/2 - h/2, 1);
			}
			break;
 
			case ButtonPress:
			{
				
				if(ev.xbutton.y_root - attr.y > attr.height - 80 &&
				   ev.xbutton.y_root - attr.y < attr.height - 30){
					if(ev.xbutton.x_root - attr.x >  5 * attr.width / 16 &&
				 	   ev.xbutton.x_root - attr.x < 11 * attr.width / 16){
						kill(0, SIGHUP);
						loop = 0;
					}
				}else if(ev.xbutton.y_root - attr.y > attr.height - 40 &&
				   	    ev.xbutton.y_root - attr.y < attr.height - 20){
					if(ev.xbutton.x_root - attr.x > 15 * attr.width / 16 &&
				 	   ev.xbutton.x_root - attr.x < 16 * attr.width / 16){
						if(pid_about == 0){
							pid_about = fork();
							if(!pid_about){
								about();
								exit(0);						
							}
						}else{
							kill(pid_about, SIGKILL);
							pid_about = 0;
						}
					}
				}else if(ev.xbutton.y_root - attr.y < 100){
					XGetWindowAttributes(dpy, win, &attr);
					int index = (ev.xbutton.x_root-attr.x)/
								(attr.width/4);		
					run(get_command(index));		
				}
			}
			break;
		}
	}
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
	return 1;
}
