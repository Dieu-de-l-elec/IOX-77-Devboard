
#include "IOX.h"

#define I2C_MASTER_SCL_IO 8
#define I2C_MASTER_SDA_IO 10
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 400000

#define SLAVE_ADDR 0x02
#define SIZE 8


uint8_t TxData[SIZE] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6, 0xAE, 0xFF};
uint8_t CmdDataFunctionA[SIZE] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x37, 0xFF};

uint8_t RxData[SIZE] = {0};

IOXClass IOX;

void IOXClass::begin() {

  pinMode (1, INPUT);      //setting all the RST pins at HIGH-Z
  pinMode (4, INPUT);
  pinMode (5, INPUT);
  pinMode (6, INPUT);
  pinMode (7, INPUT);

  i2c_config_t conf = {};
  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = I2C_MASTER_SDA_IO;
  conf.scl_io_num = I2C_MASTER_SCL_IO;
  conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  conf.master.clk_speed = I2C_MASTER_FREQ_HZ;

  i2c_param_config(I2C_MASTER_NUM, &conf);
  i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}


void IOXClass::sendI2CCommand (uint8_t *CmdData, uint8_t ADDRESS) {
  i2c_cmd_handle_t cmd = i2c_cmd_link_create(); // Create command link
  i2c_master_start(cmd); // START
  i2c_master_write_byte(cmd, (ADDRESS << 1) | I2C_MASTER_WRITE, true); // Address + Write
  i2c_master_write(cmd, CmdData, SIZE, true); // Send data
  i2c_master_stop(cmd); // STOP
  i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS); // Execute transaction
  i2c_cmd_link_delete(cmd);

}

void IOXClass::retrieveI2CData (uint8_t ADDRESS) {
  i2c_cmd_handle_t cmd = i2c_cmd_link_create(); // Create command link
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (ADDRESS << 1) | I2C_MASTER_READ, true); // Address + Read
  i2c_master_read(cmd, RxData, SIZE - 1, I2C_MASTER_ACK);  // Read bytes
  i2c_master_read_byte(cmd, &RxData[SIZE - 1], I2C_MASTER_NACK);
  i2c_master_stop(cmd); // STOP
  i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS); // Execute transaction
  i2c_cmd_link_delete(cmd);
}

uint8_t IOXClass::getAddr (uint8_t pin) {
  switch (pin >> 4) {
    case 0x0A :
      return CH32_A_ADDR;
    case 0x0B :
      return CH32_B_ADDR;
    case 0x0C :
      return CH32_C_ADDR;
    case 0x0D :
      return CH32_D_ADDR;
    case 0x0E :
      return CH32_E_ADDR;
  }
  return 0xFF;
}

void IOXClass::resetCH32 (uint8_t CH32) {
  ::pinMode (CH32, OUTPUT);
  ::digitalWrite (CH32, LOW);
  delay (200);
  ::pinMode (CH32, INPUT);
}

void IOXClass::resetCH32_All () {
  ::pinMode (1, OUTPUT);
  ::pinMode (4, OUTPUT);
  ::pinMode (5, OUTPUT);
  ::pinMode (6, OUTPUT);
  ::pinMode (7, OUTPUT);

  ::digitalWrite (1, LOW);
  ::digitalWrite (4, LOW);
  ::digitalWrite (5, LOW);
  ::digitalWrite (6, LOW);
  ::digitalWrite (7, LOW);
  delay (200);
  ::pinMode (1, INPUT);
  ::pinMode (4, INPUT);
  ::pinMode (5, INPUT);
  ::pinMode (6, INPUT);
  ::pinMode (7, INPUT);
}


void IOXClass::digitalWrite (uint8_t pin, uint8_t state) {
  uint32_t initTime = micros();
  uint8_t CMD [SIZE] = {0x10, (uint8_t)(pin & 0x0F), state, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  sendI2CCommand (CMD, (uint8_t) getAddr (pin));
  while (micros() - initTime < 1000);
}

void IOXClass::analogWritePWM (uint8_t pin, uint16_t duty_cycle, uint32_t freqOUT) {
  uint32_t initTime = micros();
  uint8_t CMD [SIZE] = {0x11, (uint8_t)(pin & 0x0F), (duty_cycle >> 8) & 0xFF, duty_cycle & 0xFF, (freqOUT >> 24) & 0xFF, (freqOUT >> 16) & 0xFF, (freqOUT >> 8) & 0xFF, freqOUT & 0xFF};
  sendI2CCommand (CMD, (uint8_t) getAddr (pin));
  while (micros() - initTime < 1000);
}

void IOXClass::pinMode (uint8_t pin, uint8_t state) {
  uint32_t initTime = micros();
  uint8_t CMD [SIZE] = {0x12, (uint8_t)(pin & 0x0F), (uint8_t)state, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  sendI2CCommand (CMD, (uint8_t) getAddr (pin));
  while (micros() - initTime < 5000);
}

void IOXClass::digitalReadSample (uint8_t pin, uint16_t interval) {
  uint32_t initTime = micros();
  uint8_t CMD [SIZE] = {0x13, (uint8_t)(pin & 0x0F), (interval >> 8) & 0xFF, interval & 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  sendI2CCommand (CMD, (uint8_t) getAddr (pin));
  while (micros() - initTime < 1000);
}

uint32_t  IOXClass::digitalReadBuffer (uint8_t pin) {
  uint32_t initTime = micros();
  uint8_t CMD [SIZE] = {0x14, (uint8_t)(pin & 0x0F), 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  sendI2CCommand (CMD, (uint8_t) getAddr (pin));
  while (micros() - initTime < 500);
  retrieveI2CData((uint8_t) getAddr (pin));
  while (micros() - initTime < 1000);
  uint32_t reading = (uint32_t)RxData[2] << 24 | (uint32_t)RxData[3] << 16 | (uint32_t)RxData[4] << 8 | (uint32_t)RxData[5];
  return reading;

}

uint16_t IOXClass::analogRead(uint8_t pin) {
  uint32_t initTime = micros();
  uint8_t CMD [SIZE] = {0x15, (uint8_t)(pin & 0x0F), 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  sendI2CCommand (CMD, (uint8_t) getAddr (pin));
  while (micros() - initTime < 5000);
  retrieveI2CData((uint8_t) getAddr (pin));
  while (micros() - initTime < 1000);
  uint32_t reading = (uint32_t)RxData[2] << 8 | (uint32_t)RxData[3];
  return reading;
}

void IOXClass::analogReadSample (uint8_t pin, uint16_t interval) {
  uint32_t initTime = micros();
  uint8_t CMD [SIZE] = {0x16, (uint8_t)(pin & 0x0F), (interval >> 8) & 0xFF, interval & 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  sendI2CCommand (CMD, (uint8_t) getAddr (pin));
  while (micros() - initTime < 3000);
}

uint16_t IOXClass::averageAnalogRead(uint8_t pin) {
  uint32_t initTime = micros();
  uint8_t CMD [SIZE] = {0x17, (uint8_t)(pin & 0x0F), 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  sendI2CCommand (CMD, (uint8_t) getAddr (pin));
  while (micros() - initTime < 5000);
  retrieveI2CData((uint8_t) getAddr (pin));
  while (micros() - initTime < 1000);
  uint32_t reading = (uint32_t)RxData[2] << 8 | (uint32_t)RxData[3];
  return reading;
}

void IOXClass::digitalReadSampleDisable (uint8_t pin) {
  uint32_t initTime = micros();
  uint8_t CMD [SIZE] = {0x18, (uint8_t)(pin & 0x0F), 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  sendI2CCommand (CMD, (uint8_t) getAddr (pin));
  while (micros() - initTime < 1000);
}

void IOXClass::analogReadSampleDisable (uint8_t pin) {
  uint32_t initTime = micros();
  uint8_t CMD [SIZE] = {0x19, (uint8_t)(pin & 0x0F), 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  sendI2CCommand (CMD, (uint8_t) getAddr (pin));
 while (micros() - initTime < 1000);
}

void IOXClass::disableAnalog(uint8_t pin) {
  uint32_t initTime = micros();
  uint8_t CMD [SIZE] = {0x20, (uint8_t)(pin & 0x0F), 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  sendI2CCommand (CMD, (uint8_t) getAddr (pin));
  while (micros() - initTime < 1000);
}

void IOXClass::sleepCH32 (uint8_t CH32_addr) {
  uint32_t initTime = micros();
  uint8_t CMD [SIZE] = {0xAA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  sendI2CCommand (CMD, CH32_addr);
  while (micros() - initTime < 3000);
}

void IOXClass::sleepCH32_All () {
  sleepCH32 (CH32_A_ADDR);
  sleepCH32 (CH32_B_ADDR);
  sleepCH32 (CH32_C_ADDR);
  sleepCH32 (CH32_D_ADDR);
  sleepCH32 (CH32_E_ADDR);
}
