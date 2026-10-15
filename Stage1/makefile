FLAGS = -Wall -Wextra -std=c11 -D_XOPEN_SOURCE=700 #contains warnings. the standard gcc should compile to
# also -D_OPENX_SOURCE=700 lets us use the readlink command in utility.c properly

simpleshell: src/simpleshell.c src/simpleshell.h src/utility.c
	gcc $(FLAGS) src/simpleshell.c src/utility.c -o bin/simpleshell

#Matt Dela Paz, 23835 - Stage 1
#CSC1021 SimpleShell - Stage 1
#I acknowledge the DCU integrity policy.
