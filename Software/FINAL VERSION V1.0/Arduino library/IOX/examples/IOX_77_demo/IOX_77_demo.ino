/*    
 *    Demo for the IOX-77 devboard | C.G. | https://github.com/Dieu-de-l-elec/IOX-77-Devboard/
 *    demonstrate how to use the GPIO basic functions (pinMode, digitalWrite, analogWritePWM, digitalRead, analogRead and sleep modes.)
 */

#include "IOX.h"

void setup() {
  IOX.resetCH32_All();
  Serial.begin(115200);
  IOX.begin();
  delay (2000);
  
  IOX.pinMode (A_LED_BUILTIN, OUTPUT);
  IOX.pinMode (B_LED_BUILTIN, OUTPUT);
  IOX.pinMode (C_LED_BUILTIN, OUTPUT);
  IOX.pinMode (D_LED_BUILTIN, OUTPUT);
  IOX.pinMode (E_LED_BUILTIN, OUTPUT);

  IOX.pinMode (D_PA1, INPUT_PULLUP);  // Button
  IOX.pinMode (D_PA2, ANALOG_IN);     // Potentiometer
}

void loop() {

  for (uint8_t i = 0; i < 10; i++) {    //cycle 10 times LEDs BUILTIN
    cycle_LED_BUILTIN();
  }

  IOX.digitalWrite (A_LED_BUILTIN, LOW);
  IOX.digitalWrite (B_LED_BUILTIN, LOW);
  IOX.digitalWrite (C_LED_BUILTIN, LOW);
  IOX.digitalWrite (D_LED_BUILTIN, LOW);
  IOX.digitalWrite (E_LED_BUILTIN, LOW);

  delay (2000);

  IOX.digitalWrite (A_LED_BUILTIN, HIGH);
  IOX.digitalWrite (B_LED_BUILTIN, HIGH);
  IOX.digitalWrite (C_LED_BUILTIN, HIGH);
  IOX.digitalWrite (D_LED_BUILTIN, HIGH);
  IOX.digitalWrite (E_LED_BUILTIN, HIGH);

  IOX.analogReadSample (D_PA2, 10); //start sampling analog readings on pin PA2 (CH32 #D) every 100ms (16 readings on the buffer)
  IOX.digitalReadSample (D_PA1, 10); //start sampling digital readings on pin PA1 (CH32 #D) every 10ms (32 readings on the buffer)

  //////////////////////////////////  DIGITAL   READINGS  //////////////////////////////////

  for (int i = 0; i < 1000; i++) {
    Serial.print ("Digital Reading on D_PA1: buffer:");
    displayBuffer ();
    Serial.print ("    Last reading: ");
    displayLastReading ();
    Serial.println ("");
    delay (10);
  }


  //////////////////////////////////  ANALOG   READINGS  //////////////////////////////////

  for (int i = 0; i < 200; i++) {
    Serial.print ("Analog Reading on D_PA2. Average over 16 readings:  ");
    Serial.println (IOX.averageAnalogRead (D_PA2));
    delay (50);
  }

  //////////////////////////////////      SlEEP MODE      /////////////////////////////////

  delay (2000);

  IOX.digitalWrite (A_LED_BUILTIN, LOW);
  IOX.digitalWrite (B_LED_BUILTIN, LOW);
  IOX.digitalWrite (C_LED_BUILTIN, LOW);
  IOX.digitalWrite (D_LED_BUILTIN, LOW);
  IOX.digitalWrite (E_LED_BUILTIN, LOW);

  IOX.sleepCH32_All ();
  delay (500);
  esp_light_sleep_start();

}

/////////////////////////////////////////////////////////////////////////////////////////

void cycle_LED_BUILTIN () {
  for (int i = 0; i < 1018; i += 5) {
    IOX.analogWritePWM (A_LED_BUILTIN, i, 16000);           // pin, duty cycle (0 - 1023), frequency (Hz)
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

void displayBuffer () {
  uint32_t reading = IOX.digitalReadBuffer (D_PA1);
  for (int i = 31; i > 0; i--) {
    Serial.print ((reading >> i) & 1);
  }
}

void displayLastReading () {
  uint32_t reading = IOX.digitalReadBuffer (D_PA1);
  Serial.print (reading & 1);
}
