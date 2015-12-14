/*
 * Sensor Module Sketch
 * Author: In Hwan "Chris" Baek
 */

#include <IRremote.h>
#include <Wire.h>
#include "rgb_lcd.h"

//Commands
#define SEND_DATA 0x9 //1001
#define SEND_ACK  0x6 //0110

//Address
#define UP    0x0   //0000
#define DOWN  0x5   //0101
#define LEFT  0xA   //1010
#define RIGHT 0xF   //1111

//PIR sensor data
#define L_ON_R_OFF  0xC  //1100
#define L_ON_R_ON   0xF  //1111
#define L_OFF_R_OFF 0x0  //0000
#define L_OFF_R_ON  0x3  //0011

void transmit(byte, byte, byte);
void initial_setup();

int setup_flag = 1;
int init_flag = 0;
int recv = 4;
IRsend irsend;
IRrecv irrecv(recv);
decode_results results;
rgb_lcd lcd;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
  lcd.begin(16, 2);
}

void loop() 
{
  unsigned long recv_value = 0;
  byte command, addr, data;

  command = SEND_DATA;
  addr = UP;
  data = L_ON_R_OFF;

  if (init_flag == 0) {
    Serial.println("\nEnter anything to send data.");
  }
  while(Serial.read() == -1 && init_flag == 0) {
    //wait until there is input.
  }
  /*
  while (digitalRead(8) != HIGH) {
    Serial.println(digitalRead(8));
  }
  */
  Serial.println("Sending data: 0x90C");
  lcd.setCursor(0,0);
  lcd.print("Sending data: 0x90c");
  
  transmit(command, addr, data);

  //read the incoming IR signal.
  Serial.println("Waiting for ACK signal...");
  lcd.setCursor(0, 1);
  lcd.print("Waiting for ACK signal...");
  
  int i = 0;
  irrecv.enableIRIn();
  /*
  if (irrecv.decode(&results)) {
    if (results.decode_type == SONY) {
      Serial.println(results.value, HEX);
      recv_value = results.value;
    }
  }
  irrecv.resume();
*/
  while(recv_value >> 8 != SEND_ACK) { //if the received signal is not an ACK signal
    //Serial.println("In the loop");
    delay(100);
    if (irrecv.decode(&results)) {
      if (results.decode_type == SONY) {
        //Serial.println(results.value, HEX);
        recv_value = results.value;
      }
    }    
    irrecv.resume();
    i++;
    if (i == 10)
      break;
  }
  irrecv.resume();

 // Serial.println("Outside the loop");
  if ((recv_value & 0xFF) != (addr << 4 | data)) {
    Serial.println("ERROR: There may be data corruption. Retransmitting.\n");
    init_flag = 1;
    transmit(command, addr, data);
  }
  else {
    Serial.println("!!!ACK received!!!\n");
    //Serial.println(recv_value, HEX);
    
    lcd.setCursor(0, 0);
    lcd.print("Waiting for ACK signal...");
    lcd.setCursor(0, 1);
    lcd.print("ACK received: ");
    lcd.print(recv_value, HEX);
     init_flag =0;
  }
  
  delay(100);
}

void transmit(byte command, byte addr, byte data)
{ 
  /*
   * +---------+----------+--------+
   * |  4-bit  |  4-bit   |  4-bit |
   * |---------+----------+--------| 
   * | command |   addr   |  data  |
   * +---------+----------+--------+
   */
  //Serial.println("Sending...");
  for (int i = 0; i < 3; i++) {
      irsend.sendSony(command << 8 | addr << 4 | data, 12); // Sony Protocol
      delay(40);
  }
}

void initial_setup()
{
  int run_flag = 1;
  setup_flag = 0;
  digitalWrite(5, HIGH);
  
  while(run_flag) {
        //Serial.println("in the loop");
    for (int i=0; i<8; i++) {
      transmit(0xF,0xF,0xF); //tells the receiver that it wants to align.
    }
     irrecv.enableIRIn(); //You need to enable Timer2 Interrupt.  

    for (int i=0; i<100; i++) {
      if (irrecv.decode(&results)) {
        Serial.println("here");
        if (results.decode_type == SONY) {
          Serial.println(results.value, HEX);
          if (results.value == 0x111) {
            run_flag = 0;
            break;
          }
        }
      }
      irrecv.resume(); // Receive the next value
      delay(10);
    }//end of for loop
    delay(100);
  }//end of while

  digitalWrite(5, LOW);
}


