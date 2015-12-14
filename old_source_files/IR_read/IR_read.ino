/*
 * Modified by In Hwan "Chris" Baek
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>
#include <Wire.h>
#include "rgb_lcd.h"

int RECV_PIN = 4;
int i = 0;
rgb_lcd lcd;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  irrecv.enableIRIn(); // Start the receiver
  // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
}

void loop() {
  digitalWrite(3, LOW);
  if (irrecv.decode(&results)) {
//    Serial.print("Receiving...\t");
    if (results.decode_type == SONY) {
      Serial.print(i);
      lcd.setCursor(0, 0);
      lcd.print(i);
      Serial.print(": ");
      lcd.setCursor(0, 1);
      lcd.print(results.value, HEX);
      Serial.println(results.value, HEX);
      i++;
    }
  irrecv.resume(); // Receive the next value
  delay(100);
  }
}
