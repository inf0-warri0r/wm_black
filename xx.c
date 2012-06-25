#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "config_read.c"

#define INDEX(a, b, c) ((a - b / c))

//Display *dpy;
//Window win;
//GC gc;
//int scr;
//Atom WM_DELETE_WINDOW;
//int loop;

char * home;

typedef struct pixmap_{
	int width;
	int height;
	Pixmap px;
} pixmap_;

int handler(Display *d, XErrorEvent *ev){
	printf("error\n");
	int i = 5;
	return i;
}
pixmap_ *load_bitmap(Display *dpy, char *name){
	Pixmap bitmap;
	unsigned int bitmap_width, bitmap_height;
	int hotspot_x, hotspot_y;
	Window root_win = DefaultRootWindow(dpy); 
	char *path = (char*)malloc(strlen(home) + strlen(name) + 2);   
	strcpy(path, home);
	strcat(path, "/");
	strcat(path, name);                                               
	int rc = XReadBitmapFile(dpy, root_win, path, &bitmap_width, &bitmap_height,
		                 &bitmap,
		                 &hotspot_x, &hotspot_y);
	if(rc == BitmapSuccess){
		pixmap_ *pm = (pixmap_ *)malloc(sizeof(pixmap_));
		pm -> width = bitmap_width;
		pm -> height = bitmap_height;
		pm -> px = bitmap;	
		return pm;
	}
	return NULL;
}
XColor color(Display *dpy, char *color){
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

	return system_color;
}
void draw_text(Display *dpy, GC gc, Window win, int width, int height, 				XFontStruct *font){		
	int xx = width/8;
	int i;
	if(read_file("apps_head")){
		XTextItem item;
		item.delta = 10;
		item.font = font -> fid; 
		app *p = list;
		for(i = 0; i < 4 && p != NULL; i++){
			item.chars = p -> name;
			item.nchars = strlen(p -> name);
			XDrawText(dpy, win, gc, (2*i + 1) * xx - 25,  90, &item, 1);
			p = p -> next;
		}
		item.chars = "Log out";
		item.nchars = strlen("Log out");
		XDrawText(dpy, win, gc, 2 * width/4 + 20,  
					height - 50, &item, 1);
	}
}
void draw_lines(Display *dpy, GC gc, Window win, int width, int height){
	XDrawLine(dpy, win, gc, 0, 100, width, 100);
	XDrawLine(dpy, win, gc, 0, height - 100, width,  height - 100);
	int i;
	for(i = 1; i < 4; i++){
		XDrawLine(dpy, win, gc, i * width/4, 0, i * width/4, 100);
	}
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
	
	home = getenv("HOME");
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
			}
			break;
 
			case ButtonPress:
			{
				
				if(ev.xbutton.y_root - attr.y > attr.height - 100){
					kill(pid, SIGKILL);
					loop = 0;
				}else if(ev.xbutton.y_root - attr.y < 100){
					XGetWindowAttributes(dpy, win, &attr);
					int index = (ev.xbutton.x_root-attr.x)/
								(attr.width/4);		
					//int index = INDEX(ev.xbutton.x_root, attr.x,attr.width / 4);
					//printf("%d \n", index);
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
