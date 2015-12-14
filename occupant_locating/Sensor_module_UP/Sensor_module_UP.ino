/*
 * Sensor Module code for Up position sensor
 * Author: In Hwan "Chris" Baek
 * 
 * Note:
 * ISR is not available because GPIO interrupts are only avaiable on
 * Pin2 and Pin3, which is reserved for PWM.
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

#define LED 5 //red
#define L_PIR 6
#define R_PIR 7

void transmit(byte, byte, byte);
void control_LED(int, int);
byte calc_sensor_data(int, int);

IRsend irsend;
byte address = UP; //this code is for up position module.
int l_pir_value = 0;
int r_pir_value = 0;
int prev_l_pir_value = 0;
int prev_r_pir_value = 0;
int change_flag;

void setup()
{
  Serial.begin(9600);
  pinMode(L_PIR, INPUT);
  pinMode(R_PIR, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  l_pir_value = digitalRead(L_PIR);
  r_pir_value = digitalRead(R_PIR);

  Serial.print("Left: ");
  Serial.print(l_pir_value);
  Serial.print(" Right: ");
  Serial.println(r_pir_value);
  control_LED(l_pir_value, r_pir_value);

  if (l_pir_value != prev_l_pir_value) {
    change_flag = 1;
    prev_l_pir_value = l_pir_value;
  }
  
  if (r_pir_value != prev_r_pir_value) {
    change_flag = 1;
    prev_r_pir_value = r_pir_value;
  }

  if (change_flag == 1) {
    byte data = calc_sensor_data(l_pir_value, r_pir_value);
    transmit(SEND_DATA, address, data);
  }

  change_flag = 0;
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
}

void control_LED(int l_pir, int r_pir)
{
  if (l_pir || r_pir) {
    digitalWrite(LED, 1);
  }
  else {
    digitalWrite(LED, 0);
  }
}

byte calc_sensor_data(int l_pir_value, int r_pir_value)
{
  if (l_pir_value == 0 && r_pir_value == 0) {
    return L_OFF_R_OFF;
  }
  else if (l_pir_value == 0 && r_pir_value == 1) {
    return L_OFF_R_ON; 
  }
  else if (l_pir_value == 1 && r_pir_value == 0) {
    return L_ON_R_OFF;
  }
  else if (l_pir_value == 1 && r_pir_value == 1) {
    return L_ON_R_ON;
  }
}

