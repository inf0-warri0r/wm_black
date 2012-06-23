#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
 
XWindowAttributes attr;
Display *dpy;
Window win;
GC gc;
int scr;
Atom WM_DELETE_WINDOW;
XEvent ev;
XEvent ev2;
KeySym keysym;
int loop;

char * home;

Pixmap load_bitmap(char *name){
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
		return bitmap;
	}
	return NULL;
}
XColor color(char *color){
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

	if (rc == 0) {
		fprintf(stderr,
				"XAllocNamedColor - allocation of 'red' color failed.\n");
	}
	return system_color;
	//XSetForeground(display, my_gc, screen_color_1.pixel);
}
int main(int argc, char *argv[])
{
	home = getenv("HOME");
	int pid = atoi(argv[1]);
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		fputs("Cannot open display", stderr);
		exit(1);
	}
	scr = XDefaultScreen(dpy);
	int display_width = DisplayWidth(dpy, scr);
	int display_height = DisplayHeight(dpy, scr);
	XColor col = color("green");
	printf("%d\n", display_height);
	win = XCreateSimpleWindow(dpy,
		XRootWindow(dpy, scr),
		0, 0, display_width, display_height, 2, /*XWhitePixel(dpy, scr),*/
		XBlackPixel(dpy, scr), XBlackPixel(dpy, scr));
	XFlush(dpy);
	XGetWindowAttributes(dpy, win, &attr);
	printf("%d\n", attr.x);
	XStoreName(dpy, win, "xx");
 
	XSelectInput(dpy, win, ExposureMask | KeyPressMask | ButtonPressMask | SubstructureNotifyMask);

	
	XFlush(dpy);

	gc = XDefaultGC(dpy, scr);
	WM_DELETE_WINDOW = XInternAtom(dpy, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(dpy, win, &WM_DELETE_WINDOW, 1);
 
	col = color("green");
	XSetForeground(dpy, gc, col.pixel);

	XColor col2 = color("black");
	XSetBackground(dpy, gc, col2.pixel);
	int co;
	char **fn = XListFonts(dpy, 
		"-misc-fixed-bold-r-normal--14-130-75-75-c-70-iso106*", 1, &co);
	
	XFontStruct *font = XLoadQueryFont(dpy, fn[0]);
	printf("aaaaaaa %d\n", co);
	XTextItem item;
	item.delta = 10;
	item.font = font -> fid; 
	XMapWindow(dpy, win);
	loop = 1;
	while (loop) {
		XNextEvent(dpy, &ev);
		printf("aaccccc %d \n", ev.type);
		//XFillRectangle(dpy, win, gc, 0, 0, 150, 100);
		XGetWindowAttributes(dpy, win, &attr);
		switch (ev.type)
		{
			case Expose:
			{
				char msg1[] = "nautilus";
				char msg2[] = "editer";
				char msg3[] = "ternimal";
				char msg4[] = "launcher";
				char msg5[] = "log out";
				
				item.chars = msg1;
				item.nchars = strlen(msg1);
				XDrawText(dpy, win, gc, 0 * attr.width/4 + 20,  80, &item, 1);
				item.chars = msg2;
				item.nchars = strlen(msg2);
				XDrawText(dpy, win, gc, 1 * attr.width/4 + 20,  80, &item, 1);
				item.chars = msg3;
				item.nchars = strlen(msg3);
				XDrawText(dpy, win, gc, 2 * attr.width/4 + 20,  80, &item, 1);
				item.chars = msg4;
				item.nchars = strlen(msg4);
				XDrawText(dpy, win, gc, 3 * attr.width/4 + 20,  80, &item, 1);
				item.chars = msg5;
				item.nchars = strlen(msg5);
				XDrawText(dpy, win, gc, 2 * attr.width/4 + 20,  
							attr.height - 50, &item, 1);

				XDrawLine(dpy, win, gc, 0, 100, attr.width, 100);
				XDrawLine(dpy, win, gc, 0,  attr.height - 100, attr.width,  attr.height - 100);
				XDrawLine(dpy, win, gc, attr.width/4,   0, attr.width/4, 100);
				XDrawLine(dpy, win, gc, attr.width/2,   0, attr.width/2, 100);
				XDrawLine(dpy, win, gc, 3 * attr.width/4,  0, 3 * attr.width/4, 100);
				Pixmap bitmap = load_bitmap("flagdown.bmp");
				if(bitmap != NULL){
					XCopyPlane(dpy, bitmap, win, gc, 0, 0, 150, 50,
								 0 * attr.width/4 + 20, 20, 1);
					XCopyPlane(dpy, bitmap, win, gc, 0, 0, 150, 50,
								 1 * attr.width/4 + 20, 20, 1);
					XCopyPlane(dpy, bitmap, win, gc, 0, 0, 150, 50,
								 2 * attr.width/4 + 20, 20, 1);
					XCopyPlane(dpy, bitmap, win, gc, 0, 0, 150, 50,
								 3 * attr.width/4 + 20, 20, 1);
				}
			}
			break;
 
			case ButtonPress:
			{
				
				if(ev.xbutton.y_root - attr.y > attr.height - 100){
					kill(pid, SIGKILL);
					loop = 0;
				}else if(ev.xbutton.y_root - attr.y < 100){
					if(ev.xbutton.x_root - attr.x < attr.width / 4){
						if(!fork()){
							execlp("nautilus", "nautilus", NULL);	
						}
					}else if(ev.xbutton.x_root - attr.x < attr.width / 2){
						if(!fork()){
							execlp("gedit", "gedit", NULL);	
						}		
					}else if(ev.xbutton.x_root - attr.x < 3*attr.width / 4){
						if(!fork()){
							execlp("gnome-terminal", "gnome-terminal", NULL);	
						}		
					}else if(ev.xbutton.x_root - attr.x > 3*attr.width / 4){
						if(!fork()){
							execlp("lon", "lon", NULL);	
						}		
					}
				}
			}
			break;
 
			case ClientMessage:
				if (ev.xclient.data.l[0] == WM_DELETE_WINDOW)
				break;
		}
	}
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
	return 1;
}
