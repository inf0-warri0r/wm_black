#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "sha.c"
#include "grapics.c"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int simb_num;
int next = 1;

Window* get_all_windows(Display *dpy){
	Window w1, w2;
	Window *w_arr;
	unsigned int n;
	XQueryTree(dpy, DefaultRootWindow(dpy), &w1, &w2, &w_arr, &n);
	simb_num = n;
	return w_arr;
}
Window find(Display *dpy, char *name){
	int i;
	Window* w_arr = get_all_windows(dpy);
	for(i = 0; i < simb_num; i++){
		char *na = '\0';
		XFetchName(dpy, w_arr[i], &na);
		if(na != NULL && strcmp(name, na) == 0){
			return w_arr[i];
		}
	}
	return 0; 
} 

void run_apps(char *pid){
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
		execlp("mini", "mini", NULL);
		exit(0);
	}
	sleep(2);
}
void set_borders(Display *dpy, Window skip, char *back, char *border){
	Window* w_arr = get_all_windows(dpy);
	int i = 0;
	XSetWindowAttributes at;
	at.background_pixel = color(dpy, back).pixel;
	at.border_pixel = color(dpy, border).pixel;
	
	for(i = 0; i < simb_num; i++){
		if( w_arr[i] != skip){
			XSetWindowBorderWidth(dpy, w_arr[i], 3);
			XChangeWindowAttributes(dpy, w_arr[i], CWBorderPixel, &at);
		}
	}
}
void send_message(Display *dpy, Window w_hide, Window w_des){
	XEvent ev;
		 
	memset(&ev, 0, sizeof (ev));
					 
	ev.xclient.type = ClientMessage;
	ev.xclient.window = w_des;
	ev.xclient.message_type = XInternAtom(dpy , "WM_PROTOCOLS", True);
	ev.xclient.format = 32;
	ev.xclient.data.l[0] = XInternAtom(dpy , "WM_DELETE_WINDOW", False);
	ev.xclient.data.l[1] = CurrentTime;
	if(w_hide != w_des)
		ev.xclient.data.l[2] = (long int)w_hide;
	XSendEvent(dpy, w_des, False, NoEventMask, &ev);
	XFlush(dpy);
}
int main(void)
{
	XSetErrorHandler(handler);

	XWindowAttributes attr;
	
	Display *dpy;
	if(!(dpy = XOpenDisplay(0x0))) return 1;
	XSelectInput(dpy, DefaultRootWindow(dpy), SubstructureNotifyMask);
	XGrabButton(dpy, 1, Mod1Mask, DefaultRootWindow(dpy), True,
				ButtonPressMask|ButtonReleaseMask|PointerMotionMask, 
				GrabModeAsync, GrabModeAsync, None, None);
	XGrabButton(dpy, 2, Mod1Mask, DefaultRootWindow(dpy), True,
				ButtonPressMask|ButtonReleaseMask|PointerMotionMask, 
				GrabModeAsync, GrabModeAsync,  None, None);
	XGrabButton(dpy, 3, Mod1Mask, DefaultRootWindow(dpy), True,
				ButtonPressMask|ButtonReleaseMask|PointerMotionMask, 
				GrabModeAsync, GrabModeAsync,  None, None);

	char pid[10];
	sprintf(pid, "%d",getpid());
	run_apps(pid);
	
	Window win_xx = find(dpy, "xx");
	Window win_clock = find(dpy, "clock");
	Window win_mini = find(dpy, "mini");

	if(win_mini != 0 && win_xx != 0 && win_clock != 0) printf("aaa");
	set_borders(dpy, win_xx, "black", "green");
	sleep(1);

	XButtonEvent start;
	XEvent ev;

	start.subwindow = None;
     while(1){
		XNextEvent(dpy, &ev);
          if(ev.type == MapNotify){
			set_borders(dpy, win_xx, "black", "green");
		}else if(ev.type == ButtonPress && ev.xbutton.subwindow != None){
			if(ev.xbutton.subwindow != win_xx && ev.xbutton.subwindow != win_clock ){
				XRaiseWindow(dpy, ev.xbutton.subwindow);
			}
			XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
			Window win_about = find(dpy, "about");
			if(ev.xbutton.button == 2 && ev.xbutton.subwindow != win_mini &&
				ev.xbutton.subwindow != win_xx && ev.xbutton.subwindow != win_clock
				&& ev.xbutton.subwindow != win_about){
					char *name = '\0';
					XFetchName(dpy, ev.xbutton.subwindow, &name);
					if(ev.xbutton.x_root > attr.x + attr.width /2  && 
						ev.xbutton.x_root < attr.x + attr.width){
						send_message(dpy, ev.xbutton.subwindow, ev.xbutton.subwindow);
					}else if(ev.xbutton.x_root < attr.x + attr.width / 2  && 
						ev.xbutton.x_root > attr.x && strcmp(name, "lon") != 0){
						send_message(dpy, ev.xbutton.subwindow, win_mini); 
					}
			}
			start = ev.xbutton;
		}else if(ev.type == MotionNotify && start.subwindow != None){

			Window win_lon = find(dpy, "lon");
			if(ev.xbutton.subwindow != win_xx 
				&& start.subwindow != win_xx 
				&& start.button ==1){
					
				int xdiff = 0;
				if(ev.xbutton.subwindow != win_lon && start.subwindow != win_lon) 
					xdiff = ev.xbutton.x_root - start.x_root;
				int ydiff = 0;
				if(ev.xbutton.subwindow != win_mini && start.subwindow != win_mini)
					ydiff = ev.xbutton.y_root - start.y_root;

			    	XMoveWindow(dpy, start.subwindow, attr.x + xdiff, attr.y + ydiff);
			}
			if(ev.xbutton.subwindow != win_xx 
				&& start.subwindow != win_xx  
				&& ev.xbutton.subwindow != win_clock
				&& start.subwindow != win_clock
				&& ev.xbutton.subwindow != win_lon 
				&& start.subwindow != win_lon
				&& start.button == 3 
				&& ev.xbutton.subwindow != win_mini
				&& start.subwindow != win_mini){

				int xdiff = ev.xbutton.x_root - start.x_root;
			     int ydiff = ev.xbutton.y_root - start.y_root;

			    	XResizeWindow(dpy, ev.xbutton.subwindow, 
							MAX(1, attr.width + xdiff),
			    				MAX(1, attr.height + ydiff));
			}
		}else if(ev.type == ButtonRelease) start.subwindow = None;
	}
}

