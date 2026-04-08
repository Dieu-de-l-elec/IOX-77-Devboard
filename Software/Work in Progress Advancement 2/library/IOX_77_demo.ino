#include "IOX.h"



void setup() {
  IOX.resetCH32_All();
  Serial.begin(115200);
  IOX.begin();
  //IOX.pinMode (D_PA1, OUTPUT);
  delay (9000);
  IOX.pinMode (A_LED_BUILTIN, OUTPUT);
  IOX.pinMode (B_LED_BUILTIN, OUTPUT);
  IOX.pinMode (C_LED_BUILTIN, OUTPUT);
  IOX.pinMode (D_LED_BUILTIN, OUTPUT);
  IOX.pinMode (E_LED_BUILTIN, OUTPUT);

  IOX.pinMode (D_PA1, ANALOG_IN);
  IOX.pinMode (D_PA2, ANALOG_IN);

  //IOX.analogReadSample (D_PA1, 64);
  //IOX.analogReadSample (D_PA2, 32);
}

void loop() {

  for (uint8_t i = 0; i < 5; i++) {

    //IOX.analogWritePWM (D_PA1, 100, 16000);
    IOX.analogWritePWM (D_PA2, 50, 16000);

    for (int i = 0; i < 1018; i += 5) {
      IOX.analogWritePWM (A_LED_BUILTIN, i, 16000);
    }
    for (int i = 0; i < 1018; i += 5) {
      IOX.analogWritePWM (B_LED_BUILTIN, i, 16000);
    }
    for (int i = 0; i < 1018; i += 5) {
      IOX.analogWritePWM (E_LED_BUILTIN, i, 16000);
    }
    for (int i = 0; i < 1018; i += 5) {
      IOX.analogWritePWM (D_PC0, i, 16000);
    }
    for (int i = 0; i < 1018; i += 5) {
      IOX.analogWritePWM (C_PC0, i, 16000);
    }

    //IOX.resetCH32 (CH32_B);
    //IOX.resetCH32 (CH32_A);
    //delay (1000);

    for (int i = 1020; i > 0; i -= 5) {
      IOX.analogWritePWM (A_LED_BUILTIN, i, 16000);
    }
    for (int i = 1020; i > 0; i -= 5) {
      IOX.analogWritePWM (B_PC0, i, 16000);
    }
    for (int i = 1020; i > 0; i -= 5) {
      IOX.analogWritePWM (E_PC0, i, 16000);
    }
    for (int i = 1020; i > 0; i -= 5) {
      IOX.analogWritePWM (D_PC0, i, 16000);
    }
    for (int i = 1020; i > 0; i -= 5) {
      IOX.analogWritePWM (C_PC0, i, 16000);
    }

  }


  IOX.sleepCH32_All ();
  delay (3000);
  esp_light_sleep_start();

  //delay (256);
  //Serial.print ("0, 1023, ");
  //IOX.pinMode (D_PA1, ANALOG_IN);
  //IOX.pinMode (D_PA2, ANALOG_IN);
  //Serial.print ("Analog PA1: ");
  //IOX.analogRead(D_PA1);
  //Serial.print (IOX.averageAnalogRead(D_PA1));
  //Serial.print (", ");
  //Serial.print ("PA2");
  //IOX.analogRead(D_PA2);
  //Serial.print ("Analog PA2: ");

  //Serial.println (IOX.averageAnalogRead(D_PA2));
  //Serial.println (IOX.analogRead(D_PA2));

}
