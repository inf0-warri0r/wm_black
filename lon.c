#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
 
XWindowAttributes attr;
Window root, win_xx, win_clock, win_wm;
Display *dpy;
Window win;
GC gc;
int scr;
Atom WM_DELETE_WINDOW;
XEvent ev;
XEvent ev2;
KeySym keysym;
int loop;
int simb_num;
char * home;

typedef struct app{
	char name[40];
	char command[40];
	struct app *next;
}app;			

app *list = NULL;

char *get_command(int index){
	app *p = list;
	int i;	
	for(i = 0; i < index; i++)
		p = p -> next;
	return p -> command;
}		
int handler(Display *d, XErrorEvent *ev){
	printf("erroe\n");
	int i = 5;
	return i;
}
int arg_count(char *s){
	int i = 1;
	int j = 0; 
 	
	while(i < strlen(s)){		
		j++;
		while((s[i] != ' ') && i < strlen(s)) i++;
		while(s[i] == ' ' && i < strlen(s)) i++;
	}
	return j;
}
int splitter(char *com, char **args){
	args[0] = strtok(com, " ");
	int i;	
	for(i = 0; args[i] != NULL; i++){
		args[i + 1] = strtok(NULL, " ");	
	}
}
int read_file(){
	FILE *f = fopen("apps", "r");
	if(f == NULL) return 0;
	int i = 0;
	int j = 0;
	char entry[80];
	//char command[20];
	char c;
	while (c != EOF) {
		
		while((c = fgetc(f)) != '\n' && j < 80){ 
			if(c == EOF) break;
			entry[j] = c;
			j++;
		}
		if(c == EOF) break; 
		entry[j] = '\0';
		j = 0;
		printf("%s\n", entry);
  		app *tmp = (app *)malloc(sizeof(app));
		char *name = strtok(entry, ":");
		char *command = strtok(NULL, ":");;
		
		strcpy(tmp -> name, name);
		strcpy(tmp -> command, command);
		tmp -> next = list;
		list = tmp;
		i++;
	}
	printf("%s\n", list -> name);
	return i;
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
}
Window* get_all_windows(Window w){
	Window w1, w2;
	Window *w_arr;
	unsigned int n;
	XQueryTree(dpy, w, &w1, &w2, &w_arr, &n);
	simb_num = n;
	printf(" -----> %d\n", n);
	return w_arr;
}
Window find(char *name, Window w){
	int i;
	Window* w_arr = get_all_windows(w);
	for(i = 0; i < simb_num; i++){
		char *na = '\0';
		XFetchName(dpy, w_arr[i], &na);
		//printf("%s\n", na);
		if(na != NULL && strcmp(name, na) == 0){
			printf("%s\n", na);
			return w_arr[i];
		}
	}
	return (int)NULL; 
}
void drew_grid(Window win, GC gc, int x, int y, int width, int N){
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
			XDrawLine(dpy, win, gc, 120 * (i + 1), 70 * j , 120 * (i + 1), 70 * (j + 1));	
			XFlush(dpy);
			n++;
			p = p -> next;
		}
		if(n >= N) break;
	}
	XFlush(dpy);
}
void run(char *com){
	printf("%s\n", com);
	int i = arg_count(com);
	if(i){
		char *args[i + 1];
		splitter(com, args);
		if(!fork()){ 
			execvp(args[0], args);
			perror("ERROR: ");
			exit(1);
		}	
	}else{
		if(!fork()){
			execlp(com, com, NULL);
			exit(0);
		}
	}
}
int main(int argc, char *argv[])
{
	int count = read_file();
	int x = 5;
	if(x > count) x = count;
	int y = count / x;
	if(count > y * x) y++;
	
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		fputs("Cannot open display", stderr);
		exit(1);
	}
	scr = XDefaultScreen(dpy);
	int display_width = DisplayWidth(dpy, scr);
	int display_height = DisplayHeight(dpy, scr);
	XColor col = color("green");
	
	win = XCreateSimpleWindow(dpy,
		XRootWindow(dpy, scr),
		display_width/2 -60*x, 200, 120*x, 70*y, 3,
		col.pixel, XBlackPixel(dpy, scr));
	XStoreName(dpy, win, "lon");	
	XFlush(dpy);
	XGetWindowAttributes(dpy, win, &attr);
	printf("%d\n", attr.x);
	XStoreName(dpy, win, "lon");
 
	XMapWindow(dpy, win);
	XFlush(dpy);

	gc = XDefaultGC(dpy, scr);
	WM_DELETE_WINDOW = XInternAtom(dpy, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(dpy, win, &WM_DELETE_WINDOW, 1);
 
	XSetForeground(dpy, gc, col.pixel);

	XColor col2 = color("black");
	XSetBackground(dpy, gc, col2.pixel);
	
	int co;
	char **fn = XListFonts(dpy, "-misc-fixed-bold-r-normal--14-130-75-75-c-70-iso106*", 1, &co);

	XFontStruct *font = XLoadQueryFont(dpy, fn[0]);
    XSelectInput(dpy, win, ExposureMask | KeyPressMask | ButtonPressMask | SubstructureNotifyMask);
	if(font == NULL) printf("aaaaaaa\n");
	
	
	XGetWindowAttributes(dpy, win, &attr);
	XSetErrorHandler(handler);
	drew_grid(win, gc, x, y, attr.width, count);
	XTextItem item;
	WM_DELETE_WINDOW = XInternAtom(dpy, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(dpy, win, &WM_DELETE_WINDOW, 1);
	loop = 1;
	while (loop) {
		XNextEvent(dpy, &ev);
		
		switch (ev.type)
		{
			case Expose:
			{
				drew_grid(win, gc, x, y, attr.width, count);
			}
			break;
			case ButtonPress:
			{
				XGetWindowAttributes(dpy, win, &attr);
				int bx = ev.xbutton.x_root - attr.x;
				int by = ev.xbutton.y_root - attr.y;
				int xx =  bx / 120;
				int yy =  by /  70;
				int index = yy * x + xx;
				printf("%d %d %d\n", ev.xbutton.x_root, attr.x, index);
				if(index < count && index > -1){
					
					run(get_command(index));
				}
			}
			break;
			case ClientMessage:
				if (ev.xclient.data.l[0] == WM_DELETE_WINDOW)
					exit(0);
			break;
		}
	}
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
	return 1;
}


