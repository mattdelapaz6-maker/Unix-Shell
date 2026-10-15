FLAGS = -Wall -Wextra -std=c11 -D_XOPEN_SOURCE=700 #contains warnings. the standard gcc should compile to
# also -D_XOPEN_SOURCE=700 lets us use the readlink command in utility.c properly

TARGET = bin/simpleshell
OBJ = simpleshell.o utility.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) #compile and link the object files to executable

simpleshell.o: src/simpleshell.c #building simpleshell
	$(CC) $(CFLAGS) -c src/simpleshell.c -o simpleshell.o

utility.o: src/utility.c # building the utility
	$(CC) $(CFLAGS) -c src/utility.c -o utility.o

#Matt Dela Paz, 23835 - Stage 2
#CSC1021 SimpleShell - Stage 2
#I acknowledge the DCU integrity policy.