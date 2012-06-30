#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
	
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
