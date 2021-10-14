#include <BLDC.h>

#define hall_1 2
#define hall_2 3
#define hall_3 4

//output pin
#define HA 9
#define HB 10
#define HC 11

#define LA 6
#define LB 7
#define LC 8

#define analogPin A0

int val = 0;

BLDC bldc1(hall_1, hall_2, hall_3, HA, LA, HB, LB, HC, LC);


void setup() {
  Serial.begin(9600);  
}

void loop() {
  // put your main code here, to run repeatedly:
  val = analogRead(analogPin);
  Serial.print(String(bldc1.getDutyCycles(val)) + "%");
  Serial.print(" ");
  Serial.println(bldc1.getHall());
  bldc1.controller(val);
}
