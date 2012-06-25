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
int read_file(char *file){
	FILE *f = fopen(file, "r");
	if(f == NULL) return 0;
	int i = 0;
	int j = 0;
	char entry[80];
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
  		app *tmp = (app *)malloc(sizeof(app));
		char *name = strtok(entry, ":");
		char *command = strtok(NULL, ":");;
		
		strcpy(tmp -> name, name);
		strcpy(tmp -> command, command);
		tmp -> next = list;
		list = tmp;
		i++;
	}
	return i;
}
void run(char *com){
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
