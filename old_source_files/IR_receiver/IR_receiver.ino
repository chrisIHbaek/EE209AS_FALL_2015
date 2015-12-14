/*
 * IR Receiver
 * Author: In Hwan "Chris" Baek
 */

#include <IRremote.h>

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
void check_recv_up();

int recv = 4;
//int recv_down = 9;
//int recv_left = 10;
//int recv_right = 11;
IRsend irsend; //need up, down, left, right
IRrecv irrecv(recv);
//IRrecv irrecv_down(recv_down);
//IRrecv irrecv_left(recv_left);
//IRrecv irrecv_right(recv_right);
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
  //irrecv_down.enableIRIn();
  //irrecv_left.enableIRIn();
  //irrecv_right.enableIRIn();

}

void loop() {
  check_recv_up();
 // delay(10);
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
   Serial.println("sending...");
  for (int i = 0; i < 3; i++) {
      irsend.sendSony(command << 8 | addr << 4 | data, 12); // Sony Protocol
      delay(40);
  }
  irrecv.enableIRIn(); //You need to enable Timer2 Interrupt. 
}

void check_recv_up()
{
  byte addr;
  byte data;

  if (irrecv.decode(&results)) {
      Serial.println("here");

    if (results.decode_type == SONY) {
      Serial.println(results.value, HEX);
      if (results.value == 0xFFF) {
        transmit(0x1, 0x1, 0x1);
      }
      else {
        addr = (results.value & 0xF0) >> 4;
        data = results.value & 0xF;
      
      transmit(SEND_ACK, addr, data);
      }
    }
  }
  irrecv.resume(); // Receive the next value
}

