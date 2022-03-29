#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include<dirent.h>

#define BUFFER_LEN 1024
#define rows 100
#define cols 100
int totalcommand = 0;

// Change firectory function 
int cd(char *path) {
    return chdir(path);
}

// Print Environment variables
extern char **environ;
int env() 
{ 
  int i = 1;
  char *s = *environ;
  for (; s; i++) {
    printf("%s\n", s);
    s = *(environ+i);
  }
  return 0;
}

// Clear screen
int clear() 
{ 
    printf("\033[H\033[J");
    return 0; 
} 

// Help function
int help() 
{ 
    printf("Welcome to shell\nUser Manual\nCommand you can use\n1.clr : Clear the screen\n2.pause:Pause operations of the shell until ‘Enter’ is pressed\n3.help: Display User Manual\n4.quit: Ctrl+D/Quit the shell\n5.history:Display the list of previously executed commands, even on shell restart\n");
    printf("6.cd <directory>: Change the current default directory to <directory>. If the <directory> argument is not present, report the current directory. If the directory doesn’t exist an appropriate error should be reported. This command should change the PWD environment variable for current shell\n");
    printf("7.dir <directory>: List all the contents of the directory <directory>\n8.environ: List all the environment strings of the current shell and the bash shell\n9.echo <comment>: Display <comment> on the display followed by a new line. Multiple spaces/tab should be reduced to a single space.\n");
    printf("9.myshell <batchfile>: batchfile> is assumed to contain a set of command lines for the shell to process\n");
    return 0; 
} 

//Pause function
int pause() {
    printf("Press ENTER key to Continue\n");  
    getchar();  
}

//List all the content of directory with 
int directory(char* path) {
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
    return 0;
}

// Run command after reading from File
int myshell(char* path, char** arr) {
    FILE *fp;
    fp  = fopen(path, "r");
    char line[BUFFER_LEN];
    int i = 0; 
    totalcommand = 0;
    while (fgets(line, sizeof(line), fp)) {
        strcpy(arr[i],line);
        i++;
    }
    strcpy(arr[i], "quit");
    totalcommand = i + 1;

}

int main(void)
{
    char line[BUFFER_LEN];  
    char arguments[100][100];
    pid_t pid;
    int myshelltrue = 0, commandctr = 0;
    //myshell file 
    char** x;
    x = malloc(rows * sizeof *x);
    for (int i=0; i<rows; i++)
    {
        x[i] = malloc(cols * sizeof *x[i]);
    }

    //Get current working directory and open History file to write logs
    char cwd2[1024];
    getcwd(cwd2, sizeof(cwd2));
    setenv("SHELL",cwd2,1); 

    FILE *fp;
    char historyfile[1024];
    strcpy(historyfile, cwd2);
    strcat(historyfile, "history.txt");
    fp  = fopen (historyfile, "w+");
    while (1)
    { 
        char cwd1[1024];
        getcwd(cwd1, sizeof(cwd1));
        printf("%s  ",cwd1);
        fflush(stdout);
        if(myshelltrue) {
            strcpy(line, x[commandctr]);
            commandctr++;
            if(commandctr == totalcommand) {
                myshelltrue = 0, commandctr = 0, totalcommand = 0;
            }

        }
        else if(!fgets(line, BUFFER_LEN, stdin)){  
            break;                                
        }
        if(strcmp(line, "quit\n")==0 || strcmp(line, "quit")==0){            
            break;
        }
        //Break the arugment from the command
        int j=0, ctr=0;
        for(int i=0;i<=(strlen(line));i++)
            {
                if(line[i]==' '||line[i]=='\0' || line[i] == '\n')
                {
                    arguments[ctr][j]='\0';
                    ctr++;  //for next word
                    if(ctr == 1) {
                        fwrite(line, j, 1, fp);
                        fwrite("\n", sizeof(char), 1, fp);
                    }
                    j=0;    //for next word, init index to 0
                }
            else
            {
                arguments[ctr][j]=line[i];
                j++;
            }
        }
        if(strcmp(arguments[0], "cd") == 0) {
            if(cd(arguments[1]) == 0) {
                getcwd(cwd1, sizeof(cwd1));
                setenv("PWD",cwd1,1); 
            }
            else {
                perror("No such directory\n");
            }

        }
        if(strcmp(arguments[0],"clr") == 0) {
            clear();
        }
        if(strcmp(arguments[0],"environ") == 0) {
            printf("Printing Environment Variables");
            env();
        } 
        if(strcmp(arguments[0],"help") == 0) {
            help();
        }
        if(strcmp(arguments[0],"pwd") == 0) {
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("Current working dir: %s\n", cwd);
            }
        }
        if(strcmp(arguments[0],"history") == 0) {
            fclose(fp);
            FILE *f;
            char s;
            f=fopen(historyfile,"r");
            while((s=fgetc(f))!=EOF) {
                printf("%c",s);
            }
            fclose(f);
            fp  = fopen (historyfile, "a");
        } 
        if(strcmp(arguments[0],"pause") == 0) {
            pause();
        }
        if(strcmp(arguments[0],"dir") == 0) {
            getcwd(cwd1, sizeof(cwd1));
            strcat(cwd1, "/");
            strcat(cwd1, arguments[1]);
            directory(cwd1);
        }
        if(strcmp(arguments[0],"myshell") == 0) {
            printf("%d\n", ctr);
            if(ctr <= 2) {
                printf("Enter correct arguments\n");
                continue;
            }
            getcwd(cwd1, sizeof(cwd1));
            strcat(cwd1, "/");
            strcat(cwd1, arguments[1]);
            myshell(cwd1, x);
            myshelltrue = 1;
            commandctr = 0;
        }
        if(strcmp(arguments[0],"echo") == 0) {
            for(int i = 1; i < ctr; i++)
                printf("%s ", arguments[i]);
            printf("\n");
        }                
    }        
}
