all: serial parallel generatefile 

serial: serial.c
	gcc -Wall -Werror serial.c -o serial

parallel: parallel.c
	mpicc -Wall -Werror parallel.c -o parallel

generatefile: generatefile.c
	gcc -Wall -Werror generatefile.c -o generatefile

clean:
	rm serial generatefile parallel *.txt

.PHONY: clean all