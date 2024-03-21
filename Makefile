all: tsh run
tsh: tsh.c
	gcc -o tsh tsh.c 
run: 
	./tsh