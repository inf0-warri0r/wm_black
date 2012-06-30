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
#include "about.h"

#define INDEX(a, b, c) ((a - b / c))

char * home;

void draw_text(Display *dpy, GC gc, Window win, int width, int height, 				XFontStruct *font){		
	int xx = width/8;
	int i;
	int text_width;
	if(read_file("apps_head")){
		XTextItem item;
		item.delta = 10;
		item.font = font -> fid; 
		app *p = list;
		for(i = 0; i < 4 && p != NULL; i++){
			item.chars = p -> name;
			item.nchars = strlen(p -> name);
			text_width = XTextWidth(font, p -> name, strlen(p -> name));
			XDrawText(dpy, win, gc, (2*i + 1) * xx - text_width/2,  90, &item, 1);
			p = p -> next;
		}
		item.chars = "Log out";
		item.nchars = strlen("Log out");
		text_width = XTextWidth(font, "Log out", strlen("Log out"));
		XDrawText(dpy, win, gc,  width/2 - text_width/2,  
					height - 50, &item, 1);

		char *x[2] = { "__", "||" };
		item.chars = x[0];
		item.nchars = strlen(x[0]);
		text_width = XTextWidth(font, x[0], strlen(x[0]));
		XDrawText(dpy, win, gc,  15.5*width/16 - text_width/2,  
					height - 37, &item, 1);
		item.chars = x[1];
		item.nchars = strlen(x[1]);
		text_width = XTextWidth(font, x[1], strlen(x[1]));
		XDrawText(dpy, win, gc,  15.5*width/16 - text_width/2,  
					height - 26, &item, 1);
	}
}
void draw_lines(Display *dpy, GC gc, Window win, int width, int height){
	XDrawLine(dpy, win, gc, 0, 100, width, 100);	
	int i;
	for(i = 1; i < 4; i++){
		XDrawLine(dpy, win, gc, i * width/4, 0, i * width/4, 100);
	}

	XDrawLine(dpy, win, gc, 5*width/16, height - 80, 11*width/16, height - 80);
	XDrawLine(dpy, win, gc, 5*width/16, height - 30, 11*width/16, height - 30);
	XDrawLine(dpy, win, gc,  5*width/16, height - 80,  5*width/16, height - 30);
	XDrawLine(dpy, win, gc, 11*width/16, height - 80, 11*width/16, height - 30);

	XDrawLine(dpy, win, gc, 15*width/16, height - 40, 15*width/16, height - 20);
	XDrawLine(dpy, win, gc, 16*width/16, height - 40, 16*width/16, height - 20);
	XDrawLine(dpy, win, gc, 15*width/16, height - 40, 16*width/16, height - 40);
	XDrawLine(dpy, win, gc, 15*width/16, height - 20, 16*width/16, height - 20);
}
void draw_icons(Display *dpy, GC gc, Window win, int width, pixmap_ *pm){
	int h = pm -> height;
	int w = pm -> width;
	Pixmap px = pm -> px;
	int xx = width/8;
	if(pm != NULL){
		int i;
		for(i = 0; i < 4; i++)
			XCopyPlane(dpy, px, win, gc, 0, 0, w, h,
					(2 * i + 1) * xx - w/2, 50 - h/2, 1);
	}
}
int main(int argc, char *argv[])
{
	XSetErrorHandler(handler);
	XWindowAttributes attr;
	
	int pid = atoi(argv[1]);
	Display *dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		fputs("Cannot open display", stderr);
		exit(1);
	}
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
 
	XSelectInput(dpy, win, ExposureMask | KeyPressMask | ButtonPressMask | SubstructureNotifyMask);

	GC gc = XDefaultGC(dpy, scr);
	Atom WM_DELETE_WINDOW = XInternAtom(dpy, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(dpy, win, &WM_DELETE_WINDOW, 1);
 
	col = color(dpy, "green");
	XSetForeground(dpy, gc, col.pixel);

	XColor col2 = color(dpy, "black");
	XSetBackground(dpy, gc, col2.pixel);
	int co;
	char **fn = XListFonts(dpy, 
		"-misc-fixed-bold-r-normal--14-130-75-75-c-70-iso106*", 1, &co);
	
	XFontStruct *font = XLoadQueryFont(dpy, fn[0]);

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
						load_bitmap(dpy, "flagdown.bmp"));
				pixmap_ *cen = load_bitmap(dpy, "a.bmp");
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
						kill(pid, SIGKILL);
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
