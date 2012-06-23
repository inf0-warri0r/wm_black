
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <pthread.h>

Window win;
XWindowAttributes w;
GC g;
Display * dpy;
Atom hide;
int count = 0;

typedef struct window{
	Window w;
	struct window *next;
} window;
window *list = NULL;
window *st = NULL;

int handler(Display *d, XErrorEvent *ev){
	int i = 5;
	//printf("Error\n");
	return i;
}
int check(Window w){
	window *p = st;
	while(p != NULL){
		if(p -> w == w) return 1;
		p = p -> next;	
	}
	return 0;
}
int add(Window w){
	if(check(w)) return 0;
	count++;
	window *tmp = malloc(sizeof(window));
	tmp -> w = w;
	tmp -> next = NULL;	
	if(st == NULL){
		st = list = tmp;
	}else{
		list -> next = tmp;
		list = list -> next;	
	}
	return 1;	
}
Window get_window(int n){
	window *p = st;
	int i;
	if(count == 0) return NULL;
	if(count == 1){
		Window w = st -> w;
		st = list = NULL;
		return w;	
	}
	for(i = 0; i < n - 1; i++){  
		p = p -> next;	
	}
	Window w = (p -> next) -> w;
	p -> next = p -> next -> next;
	if(p -> next  == NULL) list = p; 
	return w;
}
XColor color(char *color){
	Visual* default_visual = DefaultVisual(dpy, DefaultScreen(dpy));
	Colormap screen_colormap = XCreateColormap(dpy, DefaultRootWindow(dpy),
                                   default_visual,  AllocNone);
	XColor system_color;
	XColor exact_color;
	Status rc = XAllocNamedColor(dpy, screen_colormap, color, &system_color,
							&exact_color);
	if (rc == 0) fprintf(stderr, "XAllocNamedColor - allocation of 'red' color failed.\n");
	return system_color;
}
void repaint(Window win, XFontStruct *font, GC gc){

	if(count <= 1)XClearArea(dpy, win, 0, 0, 200, 50, False);	
	else XClearArea(dpy, win, 0, 0, (count - 1)*200, 50, False);
	window *p = st;
	if(count <= 1) XResizeWindow(dpy, win, 200, 50);
	else XResizeWindow(dpy, win, count*200, 50);

	int j = 0;
	while(p != NULL){
		Window w = p -> w;
		
		char *name = '\0';
		XFetchName(dpy, w, &name); 
		if(name == NULL) name = "untitled";
		if(strlen(name) > 20) name[20] = '\0';
		printf("%s\n", name);
		XDrawLine(dpy, win, gc, 200*j, 0, 200*j, 50);

		XTextItem item;
		item.delta = 10;
		item.font = font -> fid; 
		item.chars = name;
		item.nchars = strlen(name);
		XDrawText(dpy, win, gc, 200*j + 20,  20, &item, 1);
		p = p -> next;
		j++;
	}
}
int main(){
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		fputs("Cannot open display", stderr);
		exit(1);
	}
	XEvent ev;
	int scr = XDefaultScreen(dpy);
	int display_width = DisplayWidth(dpy, scr);
	int display_height = DisplayHeight(dpy, scr);
	XColor col = color("green");

	Window win = XCreateSimpleWindow(dpy,
		XRootWindow(dpy, scr),
		display_width / 2  - 100 , display_height - 155, 200, 50, 3,
		col.pixel, XBlackPixel(dpy, scr));
	XStoreName(dpy, win, "mini");
	XSelectInput(dpy, win, ExposureMask | KeyPressMask | ButtonPressMask | 					SubstructureNotifyMask);
	
	hide = XInternAtom(dpy, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(dpy, win, &hide, 1);
	XMapWindow(dpy, win);
	GC gc = XDefaultGC(dpy, scr);
	XSetForeground(dpy, gc, col.pixel);
	int co;
	char **fn = XListFonts(dpy, "-misc-fixed-bold-r-normal--14-130-75-75-c-70-iso106*", 1,
						&co);
	
	XFontStruct *font = XLoadQueryFont(dpy, fn[0]);
	char *msg1 = "-----";
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
				if(count > 0) repaint(win, font, gc);
			}
			break;
			case ClientMessage:
			if (ev.xclient.data.l[0] == hide){
				if(ev.xclient.data.l[2] != NULL && ev.xclient.data.l[2] != 0){
					Window n = (Window)ev.xclient.data.l[2];
					if(add(n)){
						repaint(win, font, gc);
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
						XMoveWindow(dpy, w, 100, 100);
						count--;
						repaint(win, font, gc);
					}
				}
			}
			break;
		}
	}
}
