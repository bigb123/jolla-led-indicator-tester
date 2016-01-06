leds_modificator: main.c
	gcc -g -Wall -o leds_modificator main.c

nodebug: main.c
	gcc -Wall -o leds_modificator main.c
	
clean:
	rm leds_modificator