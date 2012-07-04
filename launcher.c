
#include "config_read.h"
#include "launcher.h"

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
