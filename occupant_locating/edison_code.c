/*
 * edison_code.c: IR receiver and server interraction code
 * Authors: In Hwan "Chris" Baek and Shubham Khandelwal
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <mraa/gpio.h>

static volatile int interrupt_flag = 0;

void display_location(char, char, char, char);

void do_when_interrupted() {
	puts("Location changed!");
	interrupt_flag = 1;
}

int main( )
{
	char command[300];
	char status[5] = "0000";
	mraa_gpio_context up_left, up_right, down_left, down_right;

	up_left = mraa_gpio_init(5);
	up_right = mraa_gpio_init(6);
	down_left = mraa_gpio_init(7);
	down_right = mraa_gpio_init(8);

	mraa_gpio_dir(up_left, MRAA_GPIO_IN);
	mraa_gpio_dir(up_right, MRAA_GPIO_IN);
	mraa_gpio_dir(down_left, MRAA_GPIO_IN);
	mraa_gpio_dir(down_right, MRAA_GPIO_IN);

	mraa_gpio_isr(up_left, MRAA_GPIO_EDGE_BOTH, &do_when_interrupted, NULL);
	mraa_gpio_isr(up_right, MRAA_GPIO_EDGE_BOTH, &do_when_interrupted, NULL);
	mraa_gpio_isr(down_left, MRAA_GPIO_EDGE_BOTH, &do_when_interrupted, NULL);
	mraa_gpio_isr(down_right, MRAA_GPIO_EDGE_BOTH, &do_when_interrupted, NULL);

	while(1) {
		if (interrupt_flag == 1) {		
			status[0] = mraa_gpio_read(up_left) + '0';
			status[1] = mraa_gpio_read(up_right) + '0';
			status[2] = mraa_gpio_read(down_left) + '0';
			status[3] = mraa_gpio_read(down_right) + '0';

//			printf("%s\n", status);
			display_location(status[0], status[1], status[2], status[3]);

			memset(command, 0, sizeof(command));
			snprintf(command, 300, "ssh ucla@164.67.194.180 'echo %s > /var/www/bit.txt'", status);
			system(command);	
			
			interrupt_flag = 0;
		}
	}
	return 0;
}

void display_location(char status0, char status1, char status2, char status3)
{
	printf("+---+---+\n");
	//first row
	printf("|");

	if (status0 == '1')
		printf(" * ");
	else
		printf("   ");

	printf("|");

	if (status1 == '1')
		printf(" * ");
	else
		printf("   ");

	printf("|\n");

        printf("+---+---+\n");                                                  

	//second row
	printf("|");	                                                                        
        if (status2 == '1')                                                       
                printf(" * ");                                                  
        else                                                                    
                printf("   ");                                                  
                                                                                
        printf("|");                                                            
                                                                                
        if (status3 == '1')                                                       
                printf(" * ");                                                  
        else                                                                    
                printf("   ");                                                  
                                                                                
        printf("|\n");

	printf("+---+---+\n\n");   
}   
