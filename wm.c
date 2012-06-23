
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

Window root, win_xx, win_clock;
XWindowAttributes w;
GC g;
Display * dpy;
XWindowAttributes attr;
int simb_num;


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

int handler(Display *d, XErrorEvent *ev){
	int i = 5;
	printf("Error\n");
	return i;
}
Pixmap c_p(Window w, int x, int y){
	Pixmap pixmap;
	int depth = DefaultDepth(dpy, w);
	pixmap = XCreatePixmap(dpy, w, x, y, depth);
	GC gc = XDefaultGC(dpy, XDefaultScreen(dpy));
	XDrawPoint(dpy, pixmap, gc, 15, 20);
	return pixmap;
}
/*
void get_up(Window wn, int n){
	XGetWindowAttributes(dpy, wn, &attr);
	XWindowChanges win;
	//win.x = 1;
	//win.y = 1;
	//win.width = attr.width;
	//win.height = attr.height;
	//win.border_width = 10;
	//win.sibling = None;
	win.stack_mode = n;
	XConfigureWindow(dpy, wn
		, CWStackMode, &win);
	XFlush(dpy);
}*/

Window* get_all_windows(){
	Window w1, w2;
	Window *w_arr;
	unsigned int n;
	XQueryTree(dpy, DefaultRootWindow(dpy), &w1, &w2, &w_arr, &n);
	simb_num = n;
	printf(" -----> %d\n", n);
	return w_arr;
}
Window find(char *name){
	int i;
	Window* w_arr = get_all_windows();
	for(i = 0; i < simb_num; i++){
		char *na = '\0';
		XFetchName(dpy, w_arr[i], &na);
		if(na != NULL && strcmp(name, na) == 0){
			printf("%s\n", na);
			return w_arr[i];
		}
	}
	return (int)NULL; 
} 
void set_borders(){
	Window* w_arr = get_all_windows();
	int i = 0;
	XSetWindowAttributes at;

	at.background_pixel = color("black").pixel;
	at.border_pixel = color("green").pixel;
	
	for(i = 0; i < simb_num; i++){
		if( w_arr[i] != win_xx &&  w_arr[i] != win_clock){
			XSetWindowBorderWidth(dpy, w_arr[i], 3);
			XChangeWindowAttributes(dpy, w_arr[i], CWBorderPixel, &at);
		}
	}
}
int main(void)
{
	XButtonEvent start;
	XEvent ev;

	if(!(dpy = XOpenDisplay(0x0))) return 1;
	XSelectInput(dpy, DefaultRootWindow(dpy), SubstructureNotifyMask);
	XGrabButton(dpy, 1, Mod1Mask, DefaultRootWindow(dpy), True,
		ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, 
		GrabModeAsync, None, None);
	XGrabButton(dpy, 2, Mod1Mask, DefaultRootWindow(dpy), True,
		ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, 
		None, None);
	XGrabButton(dpy, 3, Mod1Mask, DefaultRootWindow(dpy), True,
		ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, 
		None, None);

	start.subwindow = None;
	char pid[10];
	sprintf(pid, "%d",getpid());
	if(!fork()){
		execlp("xx", "xx", pid, NULL);
		exit(0);
	}
	sleep(1);
	if(!fork()){
		execlp("clock", "clock", NULL);
		exit(0);
	}
	
	if(!fork()){
		execlp("xterm", "xterm", NULL);
		exit(0);
	}
	sleep(2);
	win_xx = find("xx");
	win_clock = find("clock");
	//if(win_xx != NULL) printf("aaa");
	XSetErrorHandler(handler);
	set_borders();
    for(;;){
		XNextEvent(dpy, &ev);
          if(ev.type == MapNotify){
			set_borders();
		}else if(ev.type == ButtonPress && ev.xbutton.subwindow != None){
			if(ev.xbutton.subwindow != win_xx && ev.xbutton.subwindow != win_clock){
				//get_up(ev.xbutton.subwindow, Above);
				XRaiseWindow(dpy, ev.xbutton.subwindow);
				//XCirculateSubwindowsUp(dpy, ev.xbutton.subwindow);
			}
			XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
			if(ev.xbutton.button == 2  ){
				//if(ev.xbutton.x_root > attr.x + attr.width - 50 && 
					//ev.xbutton.x_root < attr.x + attr.width){
					XEvent ev2;
		 
					memset(&ev2, 0, sizeof (ev2));
					 
					ev2.xclient.type = ClientMessage;
					ev2.xclient.window = ev.xbutton.subwindow;
					ev2.xclient.message_type = XInternAtom(dpy
						, "WM_PROTOCOLS", True);
					ev2.xclient.format = 32;
					ev2.xclient.data.l[0] = XInternAtom(dpy
						, "WM_DELETE_WINDOW", False);
					ev2.xclient.data.l[1] = CurrentTime;
					XSendEvent(dpy, ev.xbutton.subwindow, False
						, NoEventMask, &ev2);
					printf("aaa\n");
				//}
			}
			start = ev.xbutton;
		}else if(ev.type == MotionNotify && start.subwindow != None){
			char *name = '\0';
			XFetchName(dpy, start.subwindow, &name);
			
			if(ev.xbutton.subwindow != win_xx && start.button ==1){
					
				int xdiff = 0;
				if(strcmp(name, "lon") != 0) 
					xdiff = ev.xbutton.x_root - start.x_root;
				int ydiff = ev.xbutton.y_root - start.y_root;

			    	XMoveResizeWindow(dpy, start.subwindow,
			    	attr.x + xdiff,
			    	attr.y + ydiff,
			    	attr.width,
			    	attr.height);
			}
			if(ev.xbutton.subwindow != win_xx && ev.xbutton.subwindow != win_clock
				&& strcmp(name, "lon") != 0 && start.button == 3){
				int xdiff = ev.xbutton.x_root - start.x_root;
			     int ydiff = ev.xbutton.y_root - start.y_root;

			    	XMoveResizeWindow(dpy, start.subwindow,
			    	attr.x,
			    	attr.y,
			    	MAX(1, attr.width + xdiff),
			    	MAX(1, attr.height + ydiff));
			}
		}else if(ev.type == ButtonRelease) start.subwindow = None;
	}
}

