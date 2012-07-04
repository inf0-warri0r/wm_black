
#include "grapics.h"

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

	char *path = full_path(name); 
                                              
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
XFontStruct *get_font(Display *dpy){
	int co;
	char **fn = XListFonts(dpy, 
		"-misc-fixed-bold-r-normal--14-130-75-75-c-70-iso106*", 
		1, &co);
	if(co == 0) fn = XListFonts(dpy, "*", 1, &co);
	XFontStruct *font = XLoadQueryFont(dpy, fn[0]);
	return font;
}
