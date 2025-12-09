# IOX-77 Devboard
IOX-77 : An ESP32 based SuperBoard (75+ GPIOs)

The IOX-77 is an ESP32-C3-MINI-1-N4 based development board that comes with a lot more GPIOs than typical ESP32 devboards. The Main ESP32 core communicates via I2C with 5 cheap RISC-V MCUs (5 CH32V003). This way, each CH32 adds 14 more GPIOs. It is an upgraded ESP32 devboard, gathering the computational power of the ESP32 and the many GPIOs offered by all the CH32

This board comes with as many IOs as the Arduino Mega, while being cheaper, smaller, and having WiFi / Bluetooth capabilities. 

Unlike basic GPIO expanders, each CH32V003 can be programmed to execute a specific task in parallel with the ESP32. (the CH32V003 has 16KB of FLASH). It's like a 6 core configuration: 6 tasks can run at the same time!
Both the ESP32 and the 5 CH32V003 runs at 3.3V, but among the 76 available GPIOs, 10 are 5.0V tolerant.

Thanks to the two 2x25 1.27mm pitch headers, shields can easily be plugged onto the IOX-7 (obviously, they need to be custom shields with the right two 1.27mm 2x25 male headers)
Modules can also be connected to the devboard via I2C or UART (one 6P .5mm FPC is dedicated to these protocols). The 40 pins FPC is another way of accessing some of the GPIOs
