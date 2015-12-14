/*
 * main_mcu.c
 *
 * Author: Xiangrui Liu
 *
 */
 
#include "mcu_api.h"
#include "mcu_errno.h"


 /*
  * This program is an MCU application.
  * It reads the data that come from Arduino UNO, wakes up and sends the data to the host CPU
  * when the data changed.
 */


/* the information buffer */
char location[5];
/* the flag indicate whether the information changed */
int flag = 0;


 void mcu_main()
 {
    /* your configuration code starts here */

    /* map between GPIO and DIG pin*/
    /* GPIO 13  to DIG5 */
    /* GPIO 182 to DIG6 */
    /* GPIO 48  to DIG7 */
    /* GPIO 49  to DIG8 */

    /* set GPIO 13 as input*/
    gpio_setup(13, 0);
    /* set GPIO 182 as input*/
    gpio_setup(182, 0);
    /* set GPIO 48 as input*/
    gpio_setup(48, 0);
    /* set GPIO 49 as input*/
    gpio_setup(49, 0);


    /* initialize the main memory buffer */
    location[0] = ' ';
    location[1] = ' ';
    location[2] = ' ';
    location[3] = ' ';
    location[4] = '\n';


    while (1)
    {
        debug_print(DBG_INFO, "The MCU application is still alive...\n");
        char bit_1 = gpio_read(13) + '0';
        char bit_2 = gpio_read(182) + '0';
        char bit_3 = gpio_read(48) + '0';
        char bit_4 = gpio_read(49) + '0';
        debug_print(DBG_INFO, "The MCU have read the information!\n");
        if(bit_1 != location[0]){
            location[0] = bit_1;
            flag = 1;
        }
        if(bit_2 != location[1]){
            location[1] = bit_2;
            flag = 1;
        }
        if(bit_3 != location[2]){
            location[2] = bit_3;
            flag = 1;
        }
        if(bit_4 != location[3]){
            location[3] = bit_4;
            flag = 1;
        }
        if(flag != 0){
            debug_print(DBG_INFO, "The MCU have sent the information!\n");
            host_send((unsigned char*)location, 5);
        }
        flag = 0;
		/* sleep 3 second */
        mcu_sleep(100);
    }
 }