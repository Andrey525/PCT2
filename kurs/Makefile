all: serial generatefile

serial: serial.c
	gcc -Wall -Werror serial.c -o serial

generatefile: generatefile.c
	gcc -Wall -Werror generatefile.c -o generatefile

clean:
	rm serial generatefile *.txt