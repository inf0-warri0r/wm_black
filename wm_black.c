/*
*Author :Tharindra Galahena
*Project:wm_black - a simple window manager in linux 
*Date   :07/07/2012
*/

#include "grapics.h"
#include "wm_black.h"

void run_apps(){
	if(!fork()){
		execlp("base_win", "base_win", NULL);
		exit(0);
	}
	sleep(1);
	if(!fork()){
		execlp("clock", "clock", NULL);
		exit(0);
	}
	if(!fork()){
		execlp("minimize", "minimize", NULL);
		exit(0);
	}
	sleep(2);
}
void set_borders(Display *dpy, Window skip, char *back, char *border){
	Window* w_arr = (Window *)get_all_windows(dpy);
	int i = 0;
	XSetWindowAttributes at;
	at.background_pixel = color(dpy, back).pixel;
	at.border_pixel = color(dpy, border).pixel;
	
	for(i = 0; i < get_simb_num(); i++){
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

