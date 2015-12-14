/*
 * Sensor Module Sketch
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
unsigned long receive();
void initial_setup();

int setup_flag = 1;
int recv = 4;
IRsend irsend;
IRrecv irrecv(recv);
decode_results results;

void setup()
{
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  irrecv.enableIRIn();
}

void loop() {
  check_recv_up();
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
   Serial.println("sending...");
  for (int i = 0; i < 3; i++) {
      irsend.sendSony(command << 8 | addr << 4 | data, 12); // Sony Protocol
      delay(40);
  } 
    irrecv.enableIRIn();
 
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
        while(1){
        transmit(0x1, 0x1, 0x1);}
      }
      addr = (results.value & 0xF0) >> 4;
      data = results.value & 0xF;
      
      transmit(SEND_ACK, addr, data);
    }
  }
  //Serial.println("resume");
  irrecv.resume(); // Receive the next value
}
