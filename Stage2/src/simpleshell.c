#include <stdio.h> //printf(), fgets()
#include <stdlib.h> //exit(), setenv(), realpath()
#include <string.h>
#include <unistd.h> //getcwd(), chdir()
#include <limits.h> //PATH_MAX
#include "simpleshell.h" //custom header file

int main(int argc, char *argv[]){
    char line[MAX_BUFFER]; //Store User input
    char *args[MAX_ARGS]; //stores command args
    char fullpath[PATH_MAX]; //stores full path of shell executable
    realpath(argv[0], fullpath); //convert relative path of program to absolute path
    setenv("shell", fullpath, 1); //set environment variable "shell" to full path
    FILE *input = stdin; //By default, input comes from keyboard

    if (argc > 1){
        input = fopen(argv[1], "r"); //Opens file to be read
        if (!input){
            perror("Error with opening batch file");
            exit(1);
        }
    }

    //Infinite loop until it quits
    while (1){
        if (input == stdin) //only show prompt in interactive mode
            print_prompt();
        if (fgets(line, MAX_BUFFER, input) == NULL) //read line from our input
            break; //reached end of the file
        parse_input(line, args);
        if(args[0] != NULL)
            execute_cmd(args);
    }

    if (input != stdin)
            fclose(input); //release files properly
    return 0;
}

/*Matt Dela Paz, StudentID 23835 - Stage 2
  CSC1021 SimpleShell - Stage 2
  I acknowledge the DCU integrity policy.*/
