all:
	clear
	gcc -o out escalonador.c
	./out
	rm out