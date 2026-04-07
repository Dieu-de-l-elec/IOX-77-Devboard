#include "IOX.h"

void setup() {
  IOX.resetCH32_All();
  Serial.begin(115200);
  IOX.begin();
  IOX.pinMode (D_PA1, OUTPUT);

  IOX.pinMode (A_LED_BUILTIN, OUTPUT);
  IOX.pinMode (B_LED_BUILTIN, OUTPUT);
  IOX.pinMode (C_LED_BUILTIN, OUTPUT);
  IOX.pinMode (D_LED_BUILTIN, OUTPUT);
  IOX.pinMode (E_LED_BUILTIN, OUTPUT);
}

void loop() {
  IOX.analogWritePWM (D_PA1, 100, 16000);
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
