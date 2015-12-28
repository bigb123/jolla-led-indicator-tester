/*
 * Simple program to change Jolla's led indicator brightness and colors
 * 
 * 
 * Return codes:
 * 0 - all ok
 * 1 - file opening error
 * 2 - bad brightness value to set
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 	// to slow down brightness change
#include <string.h>
#include <getopt.h>

#define MIN_BRIGHT 0
#define MAX_BRIGHT 255

typedef enum {RED, GREEN, BLUE} colors;		// avaliable base colors


const char *program_name;


// return address of the led file
char *get_addr(colors color){
	
	const char *addr_beg = "/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8xxx-led/leds/led:rgb_";
	const char *addr_end = "/brightness";
	const int size_of_cat_addr = strlen(addr_beg)+strlen(addr_end)+6;
	char cat_addr[size_of_cat_addr];		// will keep address here
	short i;
	
	// VERY IMPORTANT erase potential data from memory (without - problems with memory wastes)
	for(i = 0; i < size_of_cat_addr; ++i)
		cat_addr[i] = '\0';
	
	strncat(cat_addr, addr_beg, strlen(addr_beg));
	
	switch(color){
	case RED:
		strncat(cat_addr, "red", strlen("red"));
		break;
		
	case GREEN:
		strncat(cat_addr, "green", strlen("green"));
		break;
	
	case BLUE:
		strncat(cat_addr, "blue", strlen("blue"));
		break;
		
	default:
		printf("Something sh*tty happened! Don't know what to do!\n");
		break;	
	}
	
	strncat(cat_addr, addr_end, strlen(addr_end));
	
	char *final_addr = malloc(sizeof(char) * size_of_cat_addr);
	strncpy(final_addr, cat_addr, size_of_cat_addr);
	return final_addr;
}


// checks if led file is accessible
void check_access(FILE *led_file, char *addr){
	
	if (NULL == led_file){
		printf("Error accessing file: %s\n", addr);
		exit(1);
	}
}


int get_brightness(FILE *led_file){
	
	char *line = NULL;		// will keep brightness level as string
	char *ret_val = NULL;	
	size_t len = 0;			// size of buffer (will be set automatically)
	
	rewind(led_file);		// go to start of file
	
	getline(&line, &len, led_file);
	ret_val = line;
	free(line);
	
	return atoi(ret_val);
}


int get_brightness_file(colors color){
	
	char *addr = get_addr(color);		// get led file address
	FILE *led_file = fopen(addr, "r");
	
	// check if file was being opened
	check_access(led_file, addr);
	
	int ret_val = get_brightness(led_file);
	
	fclose(led_file);
	free(addr);
	
	return ret_val;
}


void set_brightness(FILE *led_file, short brightness_value){
	
	// check if brightness_value is not less than smallest value and more than highest value
	if ((MAX_BRIGHT < brightness_value) || (MIN_BRIGHT > brightness_value)){
		printf("Value must be between %d and %d\n", MIN_BRIGHT, MAX_BRIGHT);
		exit(2);
	}
		
	char char_value[3]; 	// this will keep char value of brightness level
	
	sprintf(char_value, "%d", brightness_value);
	fputs(char_value, led_file);
	fflush(led_file);
}


// handling file operations - useful for single value setting
void set_brightness_file(colors color, short brightness_value){
	
	char *addr = get_addr(color);		/* get led file address*/
	FILE *led_file = fopen(addr, "r+");
	
	/* check if file was being opened*/
	check_access(led_file, addr);
	
	set_brightness(led_file, brightness_value);
	
	fclose(led_file);
	free(addr);
}


// wrapper: set led to value 0
void reset_led(FILE *led_file){
	set_brightness(led_file, 0);
}


void print_usage(){
	printf("%s [[-r 0-255] [-g 0-255] [-b 0-255]] [-t] [-v] [-h]\n\n", program_name);
	
	printf("Program to test RGB led indicator in Jolla phone\n\n"
	
	"Usage: %s options [value (from 0 to 255)]\n"
	"where: \n"
	"  -h  --help              dispplay this help message\n"
	"  -r  --red               set red led brightness\n"
	"  -g  --green             set green led brightness\n"
	"  -b  --blue              set blue led brightness\n"
	"  -t  --test              test all leds turning them on and off one by one and all at the end\n"
	"  -i  --info-values       inform about actual setted brightness\n"
	, program_name
	);
}


void test(){
	
	short brightness_level, color;
	char *addr;
	FILE *led_file;
	
	for(color = RED; color <= BLUE; ++color) {
		
		
		addr = get_addr(color);
		led_file = fopen(addr, "r+");
		check_access(led_file, addr);
		
		reset_led(led_file); /* initial state for led*/
		
		for(brightness_level = MIN_BRIGHT; brightness_level <= MAX_BRIGHT; ++brightness_level){
			set_brightness(led_file, brightness_level);
			usleep(1000);
		}
		
		for(brightness_level = MAX_BRIGHT; brightness_level >= MIN_BRIGHT; --brightness_level){
			set_brightness(led_file, brightness_level);
			usleep(1000);
		}
		
		reset_led(led_file); /* initial state for led*/
		
		fclose(led_file);
		free(addr);
	}
}


int main(int argc, char *argv[]){

	program_name = argv[0];
	int next_option; // number of option
	char *red = NULL, *green = NULL, *blue = NULL;
	short test_bool = 0, info_values = 0;		// if test mode was requested by user
	
	// program arguments
	
	const char* const short_options = "hr:g:b:ti";
	
	const struct option long_options[] = {
		{"help",				0, NULL, 'h'},
		{"red",					1, NULL, 'r'},
		{"green",				1, NULL, 'g'},
		{"blue",				1, NULL, 'b'},
		{"test",				0, NULL, 't'},
		{"info-values",			0, NULL, 'i'},
		{NULL, 		0, NULL, 0}
	};
	
	do {
		next_option = getopt_long(argc, argv, short_options, long_options, NULL);
		
		switch(next_option){
		
		case 'h':
			print_usage();
			break;
			
		case 'r':
			red = optarg;
			break;
		
		case 'g':
			green = optarg;
			break;
			
		case 'b':
			blue = optarg;
			break;
			
		case 't':
			test_bool = 1;
			break;
			
		case 'i':
			info_values = 1;
			break;
			
		default:
			break;
		}
		
	} while (next_option != -1);
	
	if (test_bool)
		test();
	
	if (NULL != red)
		set_brightness_file(RED, atoi(red));

	if (NULL != green)
		set_brightness_file(GREEN, atoi(green));
	
	if (NULL != blue)
		set_brightness_file(BLUE, atoi(blue));
	
	if (info_values){
		printf( "red:      %d\n"
				"green:    %d\n"
				"blue:     %d\n", 
		get_brightness_file(RED), 
		get_brightness_file(GREEN), 
		get_brightness_file(BLUE));
	}
	
	return 0;	
}
