#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "share.h"
#include "grapics.h"
#include "wm_black.h"

int main(int argc, char *argv[]){

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

	run_apps();
	
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
						XIconifyWindow(dpy, ev.xbutton.subwindow, DefaultScreen(dpy));
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

