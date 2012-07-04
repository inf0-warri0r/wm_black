
#include "minimize.h"

int count = 0;
int simb_num = 0;
window *list = NULL;
window *st = NULL;

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
	if(count == 0){
		return 0;
	}
	if(count == 1){
		Window w = st -> w;
		st = list = NULL;
		return w;	
	}
	if(n == 0){
		Window w = st -> w;
		st = st -> next;	
		return w;
	}
	for(i = 0; i < n - 1 && p != NULL; i++){  
		p = p -> next;	
	}
	Window w = (p -> next) -> w;
	p -> next = p -> next -> next;
	if(p -> next  == NULL) list = p; 
	return w;
}
void repaint(Display *dpy, Window win, XFontStruct *font, GC gc){

	if(count <= 1){
		XClearArea(dpy, win, 0, 0, 200, 50, False);
		XResizeWindow(dpy, win, 200, 50);
	}else{
		XClearArea(dpy, win, 0, 0, (count - 1)*200, 50, False);
		XResizeWindow(dpy, win, count*200, 50);
	}

	window *p = st;
	int j = 0;
	while(p != NULL){
		Window w = p -> w;
		
		char *name = '\0';
		XFetchName(dpy, w, &name); 
		if(name == NULL) name = "untitled";
		if(strlen(name) > 20) name[20] = '\0';
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
