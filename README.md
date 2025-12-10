# IOX-77 : An ESP32 based Devboard with 75 GPIOs available (ESP32 Core with 5 CH32V003 MCUs)

The IOX-77 is an ESP32-C3-MINI-1-N4 based development board that comes with a lot more GPIOs than typical ESP32 devboards. The Main ESP32 core communicates via I2C with 5 cheap RISC-V MCUs (5 CH32V003). This way, each CH32 adds 14 more GPIOs. It is an upgraded ESP32 devboard, gathering the computational power of the ESP32 and the many GPIOs offered by all the CH32

This board comes with more IOs than the Arduino Mega, while being cheaper, smaller, and having WiFi / Bluetooth capabilities. 

Unlike basic GPIO expanders, each CH32V003 can be programmed to execute a specific task in parallel with the ESP32. (the CH32V003 has 16KB of FLASH). It's like a 6 core configuration: 6 tasks can run at the same time!
Both the ESP32 and the 5 CH32V003 runs at 3.3V, but among the 75 available GPIOs, 10 are 5.0V tolerant.

Thanks to the two 2x25 1.27mm pitched headers, shields can easily be plugged onto the IOX-7 (obviously, they need to be custom shields with the right two 1.27mm 2x25 male headers)
Modules can also be connected to the devboard via I2C or UART (one 6P .5mm FPC is dedicated to these protocols). A 40 pins FPC is another way of accessing some of the GPIOs

# Key features 
- **ESP32 C3** main Core
- **x5 CH32V003** MCUs
- **Runs at 3.3V**, but 10 GPIOs are 5V tolerant
- **75 Usable GPIOs**
- **USB-C** for programming / powering the board
- **relatively small devboard**: 71mm x 42mm (slighly longer than the Arduino® UNO but way smaller than the Arduino® MEGA)
- **Wifi/Bluetooth** calpabilities
- **Onboard 3.3V LDO** [TLV75733PDBVR] up to 1A, input 4V - 5.5V
- **Multiple Ways of Powering the devboard**: USB-C 5V IN, solderable pads +5V IN or +3.3V IN, Power pins on the headers *(it's possible to power the devboard via a shield)*
- **ALL Pins (GPIOs, PWR pins, UART, I²C, ESP32 ENABLE and BOOT pins) are accessible with two 2x25 1.27mm pitched female headers**
- **40P .5mm FPC connector** for accessing 32 of the GPIOs (coming out of the CH32s), with GND and +3.3V pins
- **6P .5mm FPC connector**: I²C, UART, GND and +3.3V pins
- **2x8 common 2.54mm pitched male header for quick prototyping** including x2 +3.3V, x1 +5V, x2 GND, TX and RX (ESP32), SCL and SDA, EN pin (ESP32), x2 GPIOs (ESP32), x2 Analog pins and x2 5V tolerant IO pins (CH32V003 MCU #D)
- **8P .5mm FPC connector** used for programming the 5 CH32V003. Each MCU has a SWIO pin. All the CH32Vs can be programmed one by one with a dedicated programmer (WCH link-E)
- Each NRST pin of the CH32s are controlled by the ESP32. It serves 2 purposes:
  1. If one CH32 isn't responding (when the ESP32 can't communicate with this specific CH32 via I²C for example), the ESP32 can reboot the faulty MCU
  2. When the devboard enters low power mode and if the CH32v003 aren't needed, they can go to sleep mode when the I²C master (ESP32) asks for it (each CH32V003 only draws a few µAmps in sleep mode). When the ESP32 needsthem back, it simply need to pull down the NRST pins to reset the MCUs

     
# Schematic

<img src="IOX_77_Schematic_V1.0.svg" alt="SCHEMATIC" width="2300"/>

# PCB Layout
Top Copper Layer (+3.3V plane)

Inner Layer 1

Inner Layer 2

Bottom Copper Layer (GND plane)
