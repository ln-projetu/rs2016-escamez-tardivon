all: tarlist
tarlist: main.o
	gcc -o ./outputs/tarlist ./outputs/main.o
main.o:
	gcc -I headers -o ./outputs/main.o -c ./src/main.c -Wall -Wextra -g
clean:
	rm -rf ./outputs/*.o
mrproper: clean
	rm -rf ./outputs/

#check:
