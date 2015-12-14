#include <IRremote.h>

int i = 0;

IRrecv irrecv_up(12);
IRrecv irrecv_down(10);
IRrecv irrecv_left(11);
IRrecv irrecv_right(9);

decode_results results;

void check_up();
void check_down();
void check_left();
void check_right();

void setup()
{
  Serial.begin(9600);
}

void loop() {
  check_up();
  check_down();
  check_left();
  check_right();
}

void check_up() {
  irrecv_up.enableIRIn();
  Serial.println("here");
  
  if (irrecv_up.decode(&results)) {
    if (results.decode_type == SONY) {
      Serial.print(i);
      Serial.print("\t Up: ");
      Serial.println(results.value, HEX);
      i++;
    }
  irrecv_up.resume(); //Receive the next value
  delay(100);
  }
}

void check_down() {
  irrecv_down.enableIRIn();
  
  if (irrecv_down.decode(&results)) {
    if (results.decode_type == SONY) {
      Serial.print(i);
      Serial.print("\t Down: ");
      Serial.println(results.value, HEX);
      i++;
    }
  irrecv_down.resume(); // Receive the next value
  delay(100);
  }
}

void check_left() {
  irrecv_left.enableIRIn();
  
  if (irrecv_left.decode(&results)) {
    if (results.decode_type == SONY) {
      Serial.print(i);
      Serial.print("\t Left: ");
      Serial.println(results.value, HEX);
      i++;
    }
  irrecv_left.resume(); // Receive the next value
  delay(100);
  }
}

void check_right() {
  irrecv_right.enableIRIn();
  
  if (irrecv_right.decode(&results)) {
    if (results.decode_type == SONY) {
      Serial.print(i);
      Serial.print("\t Right: ");
      Serial.println(results.value, HEX);
      i++;
    }
  irrecv_right.resume(); // Receive the next value
  delay(100);
  }
}
