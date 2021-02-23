exec: main.c map.c Decode.c
	gcc -o exec main.c map.c Decode.c
	./exec
