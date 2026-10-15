# Running Interactive Mode
./bin/simpleshell

# Running Batch Mode
.bin/simpleshell batchfile.txt

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

# Notes
Shell sets the environment variable 'shell' to the full path of the executable.
Batch mode executes commands from a file instead of standard input.