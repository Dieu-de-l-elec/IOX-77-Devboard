# IOX-77 Firmware

The final code is now available, with the Arduino library for the ESP32. I keep the code on advancement 1 and 2 (WIP 1 and WIP 2) for archive. 

The Arduino® library  IOX.h for the ESP32 C3 make the use of this devboard easy, controlling the 'external' GPIOs (handling I2C com with the 5 CH32s) like we do with any other devboard (`pinMode()`, `digitalWrite()`, `digitalRead()`, `analogRead()`, `analogWrite()` functions ...). The CH32 can be programmed using MounRiver Studio and even though I've made a ready to use software to make the CH32 act as smart GPIO expander, the main advantage of this setup is the ability to run custom software, macros, on the CH32, to free the main MCU, ESP32, from some tasks.

The current code for each CH32 takes ~51% of RAM and ~44% of FLASH, which let enough space for the user to implement custom macros that can run on an CH32 while the ESP32 can do more important things.

## IOX.h library: Some explanations

first to initialize I²C with the CH32s:

`IOX.begin();`

There are quite a few similar functions available to command the GPIOs:


`IOX.pinMode (uint8_t pin, uint8_t state)`

`IOX.digitalWrite (uint8_t pin, uint8_t state)`

`IOX.analogWritePWM (uint8_t pin, uint16_t duty_cycle, uint32_t freqOUT)`

`IOX.analogRead(uint8_t pin)`

And some functions I created a bit differently:

for digital Reading, the digital Sampling first has to be enabled, with the sampling interval (`ms`):
`IOX.digitalReadSample (uint8_t pin, uint16_t interval);`

and the 32 last readings can be retrieved in a 32 bit variable with:

`uint32_t reading = IOX.digitalReadBuffer (uint8_t pin);`

For the analogReading, Sampling every `interval (ms)` can be enabled too (16 readings keept in history) to then retrieve the average value back:

`IOX.analogReadSample (uint8_t pin, uint16_t interval)`

`uint16_t IOX.averageAnalogRead(uint8_t pin);`


Once the sample has been enabled on one pin for digital or analog readings, it can still be disabled:

`IOX.disableAnalog(uint8_t pin)`

`IOX.digitalReadSampleDisable (uint8_t pin)`

The ESP32 can reset each CH32 separatly or all at once:

from `IOX.resetCH32 (CH32_A)` to `IOX.resetCH32 (CH32_E)`

or: `IOX.resetCH32_All ()`

to reduce power consumption down to <1mA, the CH32 can go into power down mode (each CH32 separatly or all at once):

from ` IOX.resetCH32(CH32_A_ADDR)` to ` IOX.resetCH32(CH32_E_ADDR)`

or: `IOX.sleepCH32_All ()`

then the ESP32 can go to light sleep as usual: `esp_light_sleep_start();`

## Accessing the pins:
each GPIO can be called by X_PYZ, where X is the CH32 (from A to E) and PXY is the pin name (like PC0 or PD4).

for example: `A_PA1`, `E_PD6` or `B_PC0`.

refer to the PINOUT pdf.

Each CH32 comes with a PWM on board orange LED, which can be accessed by `X_LED_BUILTIN`, from `A_LED_BUILTIN` to `E_LED_BUILTIN`
## pinModes states available:
The usual `INPUT`, `OUTPUT`, `INPUT_PULLUP`, `INPUT_PULLDOWN`
and `ANALOG_IN` (required to initialize ADC), as well as the less common `OUTPUT_OPEN_DRAIN`, `ALTERNATE_FUNCT_OD`, `ALTERNATE_FUNCT_PP`

