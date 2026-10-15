#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "simpleshell.h"

//stage 2 added libraries
#include <fcntl.h>
#include <sys/wait.h>

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
    CommandMeta meta = {NULL, NULL, 0, 0}; //creates metadata struct
    parse_special(args, &meta); //Extracts: '<', '>', '>>', '&'
    if (is_internal(args)){//if command is internal
        int saved_stdout = -1; //backup stdout
        int saved_stdin = -1; //backup stdin

        if (meta.input != NULL){//if input redirection is requested...
            saved_stdin = dup(STDIN_FILENO); // save original stdin for restoration later

            int fd = open(meta.input, O_RDONLY); // open input file
            if (fd < 0){
                perror("Input redirection failed");
                return 1;
            }

            dup2(fd, STDIN_FILENO); // redirect stdin to fd. Any input from stdin will go to the file instead of keyboard
            close(fd);
        }

        if(meta.output != NULL){ //if output redirection is requested...
            saved_stdout = dup(STDOUT_FILENO); //saves the original stdout for restoration later
            int flags = O_WRONLY | O_CREAT | // tells OS to write only to this file, if file does not exist then create it
                (meta.append ? O_APPEND : O_TRUNC); // checks if we want append mode, all writes go to the file, overwrite the file , file permission mode used in creating new files
            int fd = open(meta.output, flags, 0644);
            if(fd < 0){
                perror("Output redirection failed");
                return 1;
            }
            dup2(fd, STDOUT_FILENO); //redirect stdout to fd. Any output will go to file instead of terminal
            close(fd);
        }
    
        //execution internal commands
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

        if(saved_stdout != -1){//restore stdout
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdout); //close backup fd, not needed anymore
        }
        if (saved_stdin != -1) {//restore stdin
            dup2(saved_stdin, STDIN_FILENO);
            close(saved_stdin); //close backup fd, not needed anymore
        }
    } else{
        execute_external(args, &meta); // forks new child process, apply the appropriate redirection
        //in child: Replace the process with external program
        //in parent: wait for child if not in background
    }
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

//Stage 2 added code below:
void parse_special(char **args, CommandMeta *meta) {
    for (int i = 0; args[i] != NULL; i++){ //loopings through all args
        if (strcmp(args[i], "<") == 0){
            meta->input = args[i + 1]; //this stores the filename
            args[i] = NULL; //removes the < from arguments because we do not want exec function to see this
        }

        else if (strcmp(args[i], ">") == 0){ //if we find ">" then we overwrite the output
            meta->output = args[i + 1]; // store the output file
            meta->append = 0; //overwriting mode set
            args[i] = NULL; //rmeoving the ">"
        }

        else if (strcmp(args[i], ">>") == 0){//for if we find ">>" the append output
            meta->output = args[i + 1]; //store output file
            meta->append = 1; //appending mode is set
            args[i] = NULL; //remove the ">>"
        }

        else if (strcmp(args[i], "&") == 0){//if we encounter "&" then we execute backgorund exevution
            meta->background = 1;//marking it as the background
            args[i] = NULL;//removing "&"
        }
    }
}

void apply_redirection(CommandMeta *meta){
    if(meta->input != NULL){//input redirection
        FILE *fp = freopen(meta->input, "r", stdin); //freopen here replaces stdin with file
        if (!fp){ //checking for errors
            perror("Input redirection has failed");
            exit(1); //exit the child process
        }
    }
    if (meta->output != NULL){//output redirection
        FILE *fp;
        if(meta->append) //if it is append mode
            fp = freopen(meta->output, "a", stdout);
        else
            fp = freopen(meta->output, "w", stdout);//if not append then overwrite the file
        if (!fp){
            perror("Output redirection failure");
            exit(1);
        }
    }
}

int is_internal(char **args){
    const char *cmds[] ={
        "cd", "clr", "dir", "environ", "echo", "help", "pause", "quit", NULL};
    
        for (int i = 0; cmds[i] != NULL; i++){
            //If match is found
            if (strcmp(args[0], cmds[i]) == 0)
                return 1; //determines it is internal
        }
        return 0; //not internal and therefore external
}

void execute_external(char **args, CommandMeta *meta){
    pid_t pid = fork(); //this creates a child process
    if (pid < 0){//catches if the fork fails
        perror("Fork failed");
        return;
    }
    if (pid == 0){//child process
        setenv("parent", "/simpleshell", 1); //set required environement variable
        apply_redirection(meta); //apply redirection BEFORE exec
        execvp(args[0], args); //replaces child process with program
        perror("Exec failed"); //if exec fails, we catch the error
        exit(1);
    }
    else{//parent process
        if (!meta->background){//if not background set
            waitpid(pid, NULL, 0); //wait for child
        }
        else{
            printf("[background pid %d]\n", pid);//background execution
        }
    }
}

/*Matt Dela Paz, StudentID 23835 - Stage 2
CSC1021 SimpleShell - Stage 2
I acknowledge the DCU integrity policy.*/