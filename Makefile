all: main
main: main.o
	gcc -o ./outputs/main ./outputs/main.o
main.o: function.o
	gcc -I headers -o ./outputs/main.o -c ./src/main.c -Wall -Wextra -g
function.o : agenda.o
	gcc -I headers -o ./outputs/function.o -c ./src/function.c -Wall -Wextra -g
agenda.o: event.o
	gcc -I headers -o ./outputs/agenda.o -c ./src/agenda.c -Wall -Wextra -g
event.o:
	gcc -I headers -o ./outputs/event.o -c ./src/event.c -Wall -Wextra -g
clean:
	rm -rf ./outputs/*.o
mrproper: clean
	rm -rf ./outputs/testEvent
