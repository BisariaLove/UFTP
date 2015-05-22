LIST: dummy.o my402list.o
	gcc -o LIST -g dummy.o my402list.o
dummy.o: dummy.c
	gcc -g -c -Wall dummy.c -o dummy.o
my402list.o: my402list.c my402list.h
	gcc -g -c -Wall my402list.c
clean:
	rm -f *.o LIST


