#include "IRremote.h"
#include "CppList.h"

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


bool initialized = false;

int rcv_count;
IRrecv **all_rcv;

void setup() {

  if (initialized) return;
  Serial.begin(9600);

  // initialize the IR reciever 
  rcv_count = 4;
  all_rcv = (IRrecv **)malloc(rcv_count*sizeof(int));
  all_rcv[0] = new IRrecv(12);
  all_rcv[1] = new IRrecv(10);
  all_rcv[2] = new IRrecv(11);
  all_rcv[3] = new IRrecv(9);
  
  for (int i=0; i<rcv_count; ++i){
    all_rcv[i]->enableIRIn();
  }

  // initialize the digital pins that connect to the GPIO of the Intel Edison
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  
  initialized = true;
}

void loop() {
  byte addr;
  byte data;
  
  for (int i=0; i<rcv_count; ++i){
    decode_results results;
    if (all_rcv[i]->decode(&results)) {
      if(results.decode_type == SONY){
        Serial.print("receiver_");
        Serial.print(i);
        Serial.print(":");
        Serial.println(results.value, HEX);

        addr = (results.value & 0xF0) >> 4;
        data = results.value & 0xF;

        if(addr == UP){
          if(data == L_ON_R_OFF){
            digitalWrite(5, HIGH);
            digitalWrite(6, LOW);
          }
          else if(data == L_ON_R_ON){
            digitalWrite(5, HIGH);
            digitalWrite(6, HIGH);
          }
          else if(data == L_OFF_R_OFF){
            digitalWrite(5, LOW);
            digitalWrite(6, LOW);
          }
          else if(data == L_OFF_R_ON){
            digitalWrite(5, LOW);
            digitalWrite(6, HIGH);
          }
        }

        if(addr == DOWN){
          if(data == L_ON_R_OFF){
            digitalWrite(7, LOW);
            digitalWrite(8, HIGH);
          }
          else if(data == L_ON_R_ON){
            digitalWrite(7, HIGH);
            digitalWrite(8, HIGH);
          }
          else if(data == L_OFF_R_OFF){
            digitalWrite(7, LOW);
            digitalWrite(8, LOW);
          }
          else if(data == L_OFF_R_ON){
            digitalWrite(7, HIGH);
            digitalWrite(8, LOW);
          }
        }
        
        if(addr == LEFT){
          if(data == L_ON_R_OFF){
            digitalWrite(7, HIGH);
            digitalWrite(5, LOW);
          }
          else if(data == L_ON_R_ON){
            digitalWrite(7, HIGH);
            digitalWrite(5, HIGH);
          }
          else if(data == L_OFF_R_OFF){
            digitalWrite(7, LOW);
            digitalWrite(5, LOW);
          }
          else if(data == L_OFF_R_ON){
            digitalWrite(7, LOW);
            digitalWrite(5, HIGH);
          }
        }

        if(addr == RIGHT){
          if(data == L_ON_R_OFF){
            digitalWrite(8, LOW);
            digitalWrite(6, HIGH);
          }
          else if(data == L_ON_R_ON){
            digitalWrite(8, HIGH);
            digitalWrite(6, HIGH);
          }
          else if(data == L_OFF_R_OFF){
            digitalWrite(8, LOW);
            digitalWrite(6, LOW);
          }
          else if(data == L_OFF_R_ON){
            digitalWrite(8, HIGH);
            digitalWrite(6, LOW);
          }
        }     
      }
      all_rcv[i]->resume();
    }
  }
}
