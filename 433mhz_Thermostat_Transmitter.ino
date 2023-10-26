// Copyright (c) 2023, Sasha (Sasha's Narrowboat Life)
// youtube.com/SashasNarrowboatLife
// All rights reserved.

// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. 

// Google Nest thermostat to 433mhz Chinese Diesel Heater interface V1.02

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

//---------User changeable setting---------
const int buttonPin = 5;  // the pin that connects to the terminal labled 2 in the nest Thermostat
const int ledPin = 13;    // the pin that the onboard LED is attached to
char rfSend[4][32] = { {"0000000000000000000000000000000"}, {"0000000000000000000000000000000"}, {"0000000000000000000000000000000"}, {"0000000000000000000000000000000"}}; //On, Off, Up, Down
int DelayAfterOn = 1200000;    // Minimum length of time in miliseconds the heater should stay on (Recommended 1200000)
int DelayAfterOff = 300000;    // Minimum length of time in miliseconds the heater should stay Off (Recommended 300000)

#define send433 3 // The pin that connects to the data pin on the 433mhz sender
//---------User changeable setting end---------

int buttonState = 0;        // current state of the button
int lastButtonState = 0;    // previous state of the button

void setup() {
  pinMode(buttonPin, INPUT);   // initialize the button pin as a input:
  pinMode(ledPin, OUTPUT); // initialize the LED as an output:
  Serial.begin(9600);  // initialize serial communication:

  mySwitch.enableTransmit(send433);
  mySwitch.setPulseLength(254); //PulseLength as read from remote
}

void loop() {
  buttonState = digitalRead(buttonPin); // read the Nest Thermostat state
  if (buttonState != lastButtonState) { // compare the Thermostat state to its previous state
    if (buttonState == HIGH) { // if the current state is HIGH then the Thermostat went from off to on
      TurnHeatingOn();
    } else {  // if the current state is LOW then the Thermostat went from on to off:
      TurnHeatingOff(); 
    }
    delay(50); // Delay a little bit to avoid bouncing
  }
  lastButtonState = buttonState; // save the current state as the last state, for next time through the loop
}

void TurnHeatingOn() //Turn the heater on function
{
  TurnHeatingOnSending();
  delay(2000); // pause for 2 seconds and sends again to improve reliability
  TurnHeatingOnSending();

  Serial.println("The Heater is on");
  digitalWrite(ledPin, HIGH);
  delay(DelayAfterOn);
  Serial.println("Checking For thermostat change again");
}

void TurnHeatingOff() //Turn the heater off function
{
  TurnHeatingOffSending();;
  delay(2000); // pause for 2 seconds and sends again to improve reliability
  TurnHeatingOffSending();

  Serial.println("The Heater is off");
  digitalWrite(ledPin, LOW);
  delay(DelayAfterOff);
  Serial.println("Checking For thermostat change again");
}

void TurnHeatingOnSending()
{
  Serial.print("Transmitting ");
  Serial.println(rfSend[0]);
  mySwitch.send(rfSend[0]);
  delay(300);
  mySwitch.send(rfSend[0]);
  delay(300);
  mySwitch.send(rfSend[0]);
  delay(300);
  mySwitch.send(rfSend[0]);
}

void TurnHeatingOffSending()
{
  Serial.print("Transmitting ");
  Serial.println(rfSend[1]);
  mySwitch.send(rfSend[1]);
  delay(300);
  mySwitch.send(rfSend[1]);
  delay(300);
  mySwitch.send(rfSend[1]);
  delay(300);
  mySwitch.send(rfSend[1]);
}
