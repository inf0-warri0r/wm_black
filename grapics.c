#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>


typedef struct pixmap_{
	int width;
	int height;
	Pixmap px;
} pixmap_;

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
pixmap_ *load_bitmap(Display *dpy, char *name){
	Pixmap bitmap;
	unsigned int bitmap_width, bitmap_height;
	int hotspot_x, hotspot_y;
	Window root_win = DefaultRootWindow(dpy); 

	char *home = getenv("HOME");
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
