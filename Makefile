all: ptar
ptar: main.o
	gcc -o ./ptar ./outputs/main.o -lm
main.o:
	gcc -I headers -o ./outputs/main.o -c ./src/main.c -Wall -Wextra -g
clean:
	rm -rf ./outputs/*.o
mrproper: clean
	rm -rf ./outputs/

#	check:
