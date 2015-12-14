/*
 * power_efficiency.c
 *
 * Author: Xiangrui Liu
 *
 */
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <string.h>


void* read_mcu();
char message[5] = "0000";


int main()
{
       pthread_t tid;
       // create a thread to read the data that come from MCU
       pthread_create(&tid, NULL, read_mcu, NULL);

       while(1){

           char message_prev[5];
           strncpy(message_prev, message, 5);

           // let the cpu go into sleep mode
           // stop the wifi
           system("systemctl stop wpa_supplicant");
           // enter into hibernation debugging mode
           system("echo devices > /sys/power/pm_test");      
           while(1){
               system("echo mem > /sys/power/state");
               if(strcmp(message_prev, message) != 0){
                   break;
               }
           }

           // start the wifi
           system("systemctl start wpa_supplicant");
       }

       pthread_join(tid, NULL);
       return 0;
}


void* read_mcu()
{   
    while(1){
       FILE *mcu_interface;
       mcu_interface = fopen("/dev/ttymcu0", "r");
       fgets(message, 5, mcu_interface);
	   // used for debugging
       char buffer[1024];
       snprintf(buffer, 1024, "echo \"%s $(date)\" >> ./mcu_debug.txt", message);
       system(buffer);
    }
}