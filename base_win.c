#include "config_read.h"
#include "grapics.h"
#include "base_win.h"

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
