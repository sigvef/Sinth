all:
	gcc *.c -g -Wall -framework Cocoa -lsdl -lsdlmain -o bin/sinth

run: all
	bin/sinth

debug: all
	gdb bin/sinth
