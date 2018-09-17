#include <JC_Button.h>

#define PCTL  5
#define PWR_BTN A7
#define LED   13
#define PWR_GOOD  28

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(PCTL, OUTPUT);
  pinMode(PWR_BTN , INPUT_PULLDOWN);
  pinMode(PWR_GOOD , INPUT_PULLUP);
  digitalWrite(PCTL, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(1000);               // wait for a second
//  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second

  Serial.println(analogRead(PWR_BTN));

  if (digitalRead(PWR_GOOD) == LOW) {
    Serial.println("Power good");
  } else {      
    Serial.println("Power NOT good");
  }

//  digitalWrite(PCTL, LOW);
//  delayMicroseconds(1);
//  digitalWrite(PCTL, HIGH);

  if (millis() > 30000) {
    digitalWrite(PCTL, LOW);
  }
}
