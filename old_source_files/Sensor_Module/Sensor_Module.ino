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

void loop() 
{
  unsigned long recv_value = 0;
  byte command, addr, data;

  command = SEND_DATA;
  addr = UP;
  data = L_ON_R_OFF;


  if (setup_flag == 1) {
    initial_setup();
  }


  Serial.println("Sending data...");
  transmit(command, addr, data);

  //read the incoming IR signal.
  int i = 0;

  if (irrecv.decode(&results)) {
    if (results.decode_type == SONY) {
      Serial.println(results.value);
      recv_value = results.value;
    }
  }
  irrecv.resume();

  while(recv_value >> 8 != SEND_ACK) { //if the received signal is not an ACK signal
    Serial.println("In the loop");
    delay(100);
    if (irrecv.decode(&results)) {
      if (results.decode_type == SONY) {
        Serial.println(results.value);
        recv_value = results.value;
      }
    }    
    irrecv.resume();
    i++;
    if (i == 50)
      break;
  }
  irrecv.resume();

  Serial.println("Outside the loop");
  if ((recv_value & 0xFF) != (addr << 4 | data)) { 
    transmit(command, addr, data);
  }
  else {
    Serial.println(recv_value);
    digitalWrite(5, HIGH);
    delay(10000);
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
  Serial.println("Sending...");
  for (int i = 0; i < 3; i++) {
      irsend.sendSony(command << 8 | addr << 4 | data, 12); // Sony Protocol
      delay(40);
  }
  irrecv.enableIRIn(); //You need to enable Timer2 Interrupt.  
}

unsigned long receive()
{
  if (irrecv.decode(&results)) {
    if (results.decode_type == SONY) {
      return results.value;
    }
  }
  else
    return NULL;
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


