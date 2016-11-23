all: ptar
ptar: main.o
	gcc -o ./ptar ./outputs/main.o -lm -lpthread
main.o:
	gcc -I headers -o ./outputs/main.o -c ./src/main.c -Wall -Wextra -ggdb
clean:
	rm -rf ./outputs/*.o
mrproper: clean
	rm -rf ./outputs/

#	check:
