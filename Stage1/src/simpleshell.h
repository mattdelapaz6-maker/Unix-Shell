//header guards prevent duplication errors
//keeping work modular

#ifndef SIMPLESHELL_H //defined so we do not include this header
#define SIMPLESHELL_H

#define MAX_BUFFER 1024 //max no. of characters
#define MAX_ARGS 64 //max arguments

//Instantiating function names ahead of time, modularity
void print_prompt(); //will print cwd as prompt
void parse_input(char *input, char **args); //splits input
int execute_cmd(char **args); //decides which command to execute

//Internal cmd implementation
void shell_cd(char **args); // change directory
void shell_clr(); //clear screen
void shell_dir(char **args); //list dir contents
void shell_environ(); //display environment variables
void shell_echo(char **args); //print text
void shell_help(char **args); //show help menu
void shell_pause(); //wait for user input
void shell_quit(); //exit shell

#endif //Ends header guard

/*Matt Dela Paz, StudentID: 23835 - Stage 1
  CSC1021 SimpleShell - Stage 1
  I acknowledge the DCU integrity policy.*/