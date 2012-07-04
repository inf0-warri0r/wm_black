#include "share.h"
	
int simb_num;
	
int handler(Display *d, XErrorEvent *ev){
	printf("error\n");
	int i = 5;
	return i;
}

char *full_path(char *file){
	char *home = getenv("HOME");
	char *path = (char*)malloc(strlen(home) + strlen(file) + 2);   
	
	strcpy(path, home);
	strcat(path, "/");
	strcat(path, file);   

	return path;
}

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
int get_simb_num(){
	return simb_num;
}
