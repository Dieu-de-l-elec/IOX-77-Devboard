#ifndef IOX_H
#define IOX_H

#include <Arduino.h>
#include <driver/i2c.h>

#define ALTERNATE_FUNCT_OD 0x06
#define ALTERNATE_FUNCT_PP 0x07
#define ANALOG_IN 0xC0

// pin definition:

const uint8_t CH32_A_ADDR = 0x01;   //02
const uint8_t CH32_B_ADDR = 0x03;   //06
const uint8_t CH32_C_ADDR = 0x04;   //08
const uint8_t CH32_D_ADDR = 0x05;   //0A
const uint8_t CH32_E_ADDR = 0x06;   //0C

const uint8_t CH32_A = 1; // NRST pin
const uint8_t CH32_B = 4; // NRST pin
const uint8_t CH32_C = 6; // NRST pin
const uint8_t CH32_D = 7; // NRST pin
const uint8_t CH32_E = 5; // NRST pin

const uint8_t A_PA1 = 0xA0; //CH32 A - pin 0
const uint8_t A_PA2 = 0xA1; //CH32 A - pin 1
const uint8_t A_PC0 = 0xA2; //CH32 A - pin 2
const uint8_t A_PC3 = 0xA3; //CH32 A - pin 3
const uint8_t A_PC4 = 0xA4; //CH32 A - pin 4
const uint8_t A_PC5 = 0xA5; //CH32 A - pin 5
const uint8_t A_PC6 = 0xA6; //CH32 A - pin 6
const uint8_t A_PC7 = 0xA7; //CH32 A - pin 7
const uint8_t A_PD0 = 0xA8; //CH32 A - pin 8
const uint8_t A_PD2 = 0xA9; //CH32 A - pin 9
const uint8_t A_PD3 = 0xAA; //CH32 A - pin 10
const uint8_t A_PD4 = 0xAB; //CH32 A - pin 11
const uint8_t A_PD5 = 0xAC; //CH32 A - pin 12
const uint8_t A_PD6 = 0xAD; //CH32 A - pin 13

const uint8_t B_PA1 = 0xB0; //CH32 B - pin 0
const uint8_t B_PA2 = 0xB1; //CH32 B - pin 1
const uint8_t B_PC0 = 0xB2; //CH32 B - pin 2
const uint8_t B_PC3 = 0xB3; //CH32 B - pin 3
const uint8_t B_PC4 = 0xB4; //CH32 B - pin 4
const uint8_t B_PC5 = 0xB5; //CH32 B - pin 5
const uint8_t B_PC6 = 0xB6; //CH32 B - pin 6
const uint8_t B_PC7 = 0xB7; //CH32 B - pin 7
const uint8_t B_PD0 = 0xB8; //CH32 B - pin 8
const uint8_t B_PD2 = 0xB9; //CH32 B - pin 9
const uint8_t B_PD3 = 0xBA; //CH32 B - pin 10
const uint8_t B_PD4 = 0xBB; //CH32 B - pin 11
const uint8_t B_PD5 = 0xBC; //CH32 B - pin 12
const uint8_t B_PD6 = 0xBD; //CH32 B - pin 13

const uint8_t C_PA1 = 0xC0; //CH32 C - pin 0
const uint8_t C_PA2 = 0xC1; //CH32 C - pin 1
const uint8_t C_PC0 = 0xC2; //CH32 C - pin 2
const uint8_t C_PC3 = 0xC3; //CH32 C - pin 3
const uint8_t C_PC4 = 0xC4; //CH32 C - pin 4
const uint8_t C_PC5 = 0xC5; //CH32 C - pin 5
const uint8_t C_PC6 = 0xC6; //CH32 C - pin 6
const uint8_t C_PC7 = 0xC7; //CH32 C - pin 7
const uint8_t C_PD0 = 0xC8; //CH32 C - pin 8
const uint8_t C_PD2 = 0xC9; //CH32 C - pin 9
const uint8_t C_PD3 = 0xCA; //CH32 C - pin 10
const uint8_t C_PD4 = 0xCB; //CH32 C - pin 11
const uint8_t C_PD5 = 0xCC; //CH32 C - pin 12
const uint8_t C_PD6 = 0xCD; //CH32 C - pin 13

const uint8_t D_PA1 = 0xD0; //CH32 D - pin 0
const uint8_t D_PA2 = 0xD1; //CH32 D - pin 1
const uint8_t D_PC0 = 0xD2; //CH32 D - pin 2
const uint8_t D_PC3 = 0xD3; //CH32 D - pin 3
const uint8_t D_PC4 = 0xD4; //CH32 D - pin 4
const uint8_t D_PC5 = 0xD5; //CH32 D - pin 5
const uint8_t D_PC6 = 0xD6; //CH32 D - pin 6
const uint8_t D_PC7 = 0xD7; //CH32 D - pin 7
const uint8_t D_PD0 = 0xD8; //CH32 D - pin 8
const uint8_t D_PD2 = 0xD9; //CH32 D - pin 9
const uint8_t D_PD3 = 0xDA; //CH32 D - pin 10
const uint8_t D_PD4 = 0xDB; //CH32 D - pin 11
const uint8_t D_PD5 = 0xDC; //CH32 D - pin 12
const uint8_t D_PD6 = 0xDD; //CH32 D - pin 13

const uint8_t E_PA1 = 0xE0; //CH32 E - pin 0
const uint8_t E_PA2 = 0xE1; //CH32 E - pin 1
const uint8_t E_PC0 = 0xE2; //CH32 E - pin 2
const uint8_t E_PC3 = 0xE3; //CH32 E - pin 3
const uint8_t E_PC4 = 0xE4; //CH32 E - pin 4
const uint8_t E_PC5 = 0xE5; //CH32 E - pin 5
const uint8_t E_PC6 = 0xE6; //CH32 E - pin 6
const uint8_t E_PC7 = 0xE7; //CH32 E - pin 7
const uint8_t E_PD0 = 0xE8; //CH32 E - pin 8
const uint8_t E_PD2 = 0xE9; //CH32 E - pin 9
const uint8_t E_PD3 = 0xEA; //CH32 E - pin 10
const uint8_t E_PD4 = 0xEB; //CH32 E - pin 11
const uint8_t E_PD5 = 0xEC; //CH32 E - pin 12
const uint8_t E_PD6 = 0xED; //CH32 E - pin 13

const uint8_t A_LED_BUILTIN = A_PC0;
const uint8_t B_LED_BUILTIN = B_PC0;
const uint8_t C_LED_BUILTIN = C_PC0;
const uint8_t D_LED_BUILTIN = D_PC0;
const uint8_t E_LED_BUILTIN = E_PC0;


class IOXClass {
  public:
    void begin();
    void digitalWrite (uint8_t pin, uint8_t state);
    void analogWritePWM (uint8_t pin, uint16_t duty_cycle, uint32_t freqOUT);
    void pinMode (uint8_t pin, uint8_t state);
    void digitalReadSample (uint8_t pin, uint16_t interval);
    uint32_t digitalReadBuffer (uint8_t pin);
    void resetCH32 (uint8_t CH32);
    void resetCH32_All ();
    uint16_t analogRead(uint8_t pin);
    void analogReadSample (uint8_t pin, uint16_t interval);
    uint16_t averageAnalogRead(uint8_t pin);
    void sleepCH32 (uint8_t CH32_addr);
    void sleepCH32_All ();
    void disableAnalog(uint8_t pin);
    void analogReadSampleDisable (uint8_t pin);
    void digitalReadSampleDisable (uint8_t pin);

  private:
    void sendI2CCommand (uint8_t *CmdData, uint8_t ADDRESS);
    void retrieveI2CData (uint8_t ADDRESS);
    uint8_t getAddr (uint8_t pin);
};

extern IOXClass IOX;

#endif
