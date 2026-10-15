#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "simpleshell.h"
//note that if an 'if' statement does not have the {...} it is because it only has on statement to use. THis is valid in C.

extern char **environ; //Needed to access environment variables
void print_prompt(){ //prints current working directory
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) //getcwd gets our cwd and stores it
        printf("%s> ", cwd); //print dir and the shell comamnd 
    else
        perror("getcwd failed"); //catching if getcwd were to fail

}

void parse_input(char *input, char **args){ //splits the user's input
    int i = 0;
    input[strcspn(input, "\n")] = 0; //removes newline
    args[i] = strtok(input, " \t"); //split by space and tab
    while (args[i] != NULL){ //continue splitting until NULL
        i++;
        args[i] = strtok(NULL, " \t");
    }
}

int execute_cmd(char **args){ //helper function to run our commands
    if (strcmp(args[0], "cd") == 0)
        shell_cd(args);
    else if (strcmp(args[0], "clr") == 0)
        shell_clr();
    else if (strcmp(args[0], "dir") == 0)
        shell_dir(args);
    else if (strcmp(args[0], "environ") == 0)
        shell_environ();
    else if (strcmp(args[0], "echo") == 0)
        shell_echo(args);
    else if (strcmp(args[0], "help") == 0)
        shell_help(args);
    else if (strcmp(args[0], "pause") == 0)
        shell_pause();
    else if (strcmp(args[0], "quit") == 0)
        shell_quit();
    else
        printf("Unknown command\n");
    return 1;
}

void shell_cd(char **args){
    //this is if user types just'cd'
    if (args[1] == NULL){//change directory
        char cwd[PATH_MAX]; //stores current dir

        //get the current working directory
        if (getcwd(cwd, sizeof(cwd)) != NULL){
            printf("%s\n", cwd); //prints the cwd
        }
        else{
            perror("getcwd error"); //if directory doesn't exist, it catches it
        }
    }
    else {
        if (chdir(args[1]) != 0){
            perror("cd failed");
        }
        else{
            char cwd[PATH_MAX];
            //THis will update the PWD env variable
            if (getcwd(cwd, sizeof(cwd)) != NULL){ //obtains new directory path
                setenv("PWD", cwd, 1);//updates our path by overwriting the previous
            }
        }
    }
        
}

void shell_clr(){
    system("clear"); //Clear screen command
}

void shell_dir(char **args){
    if (args[1] == NULL)
        system("ls -al");
    else{
        char command[1024];
        snprintf(command, sizeof(command), "ls -al %s", args[1]); //snprintf prints text to string safely by setting a limit to the characters (buffer)
        system(command);
    }
}

void shell_environ(){
    for (int i = 0; environ[i] != NULL; i++) //print environment variables
        printf("%s\n", environ[i]);
}

void shell_echo(char **args){
    int i = 1;
    //function will catch uneeded tabs and spaces
    
    //if user only types 'echo' and nothing afterwards
    if (args[1] == NULL){
        printf("Did you mean to add something after echo?\n");
        return;
    }

    while(args[i] != NULL){
        printf("%s", args[i]);

        if (args[i + 1] != NULL){ //tracks all the tabs and spaces and converts to single space
            printf(" ");
        }
        i++;
    }
    printf("\n");
}

void shell_help(char **args){

    (void)args; // because help doesn't need any arguments, we can hide the warning so it doesn't trigger.

    char exe_path[PATH_MAX]; //buffer to contain executable
    char help_path[PATH_MAX]; //buffer to contain path to README file

    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1); //Get full path of the executable

    if (len == -1){
        perror("Error determining executable path"); //should the readlink fail, we catch the error
        return;
    }

    exe_path[len] = '\0'; //adds terminate to string

    char *bin_dir = strstr(exe_path, "/bin/"); //Remove '/bin/simpleshell' from path
    if (bin_dir == NULL){ //if bin cannot be found, detect error
        printf("Error determining project path\n");
        return;
    }

    *bin_dir = '\0'; //cut and shorten the path at /bin

    //make path to README
    if (snprintf(help_path, sizeof(help_path), "%s/manual/README.txt", exe_path) >= (int)sizeof(help_path)){ //snpritnf will limit the number of characters written. Another buffer essentially.
        printf("Error: Help file path is too long\n"); //check for if too long
        return;
    }

    //will display help file
    char command[PATH_MAX + 20];
    snprintf(command, sizeof(command), "more \"%s\"", help_path);

    system(command); //execute comamnd to display README
}

void shell_pause(){
    printf("Press Enter to continue ...");
    while (getchar() != '\n'); //Pause execution
}

void shell_quit(){
    exit(0);
}

/*Matt Dela Paz, StudentID 23835 - Stage 1
CSC1021 SimpleShell - Stage 1 
I acknowledge the DCU integrity policy.*/