# Running Interactive Mode
./bin/simpleshell

# Running Batch Mode
./bin/simpleshell batchfile.txt

# Internal Commands
cd <dir>    Change current directory
            Note: If no argument is given for <dir> it will print the current directory.
clr         Clear screen
dir <dir>   List directory contents
environ     Display environment variables
echo <text> Display text
            Note: Any tab or space characters will be converted to a single space
help        Display help information
pause       Wait for user to press Enter
quit        Exit the shell

# Stage 2 below:

# External Commands

Any command that is not built into the shell is treated as an external command.
The shell creates a new process using fork() and then replaces that process
with the requested program using execvp().

Example:
ls -l

This will list files in the current directory using the system's ls program.

# I/O redirection

The shell has input and output redirection so you can control
where a program reads input from or writes output to.

<  Redirects input from a file instead of the keyboard
>  Redirects output to a file (overwrites existing content)
>> Redirects output to a file (appends to existing content)

Example:
sort < input.txt > output.txt

This command reads data from input.txt, sorts it, and writes the result
to output.txt.

# Redirection with internal commands

Output redirection works with internal commands such as:
dir, echo, environ, and help.

Example:
dir > files.txt

This will write the directory listing into files.txt instead of displaying it.

# Background execution

If a command ends with the '&' symbol, it will run in the background.
This means the shell does not wait for the program to finish before
displaying the prompt again.

Example:
sleep 10 &

The shell will immediately return to the prompt while the program continues running.
Foreground execution means the shell waits until the program finishes.

# Environment Variable

When executing external programs, the shell sets an environment variable:

parent=/simpleshell

This allows child processes to know which shell started them.

# Notes
Shell sets the environment variable 'shell' to the full path of the executable.
Batch mode executes commands from a file instead of standard input.