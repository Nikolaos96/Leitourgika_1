main: ask1.o main.o lista.o
	gcc ask1.o main.o lista.o -o main

main.o: main.c
	gcc -c main.c

ask1.o: ask1.c
	gcc -c ask1.c

lista.o: lista.c
	gcc -c lista.c
	
clean:
	rm -f main ask1.o main.o lista.o

