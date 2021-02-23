exec: main.o map.o Decode.o
	gcc -o exec main.o map.o Decode.o
	
main.o: main.c
	gcc -c main.c
	
map.o: map.c map.h
	gcc -c map.c
	
Decode.o: Decode.c Decode.h
	gcc -c Decode.c

clean:
	rm *.o exec
