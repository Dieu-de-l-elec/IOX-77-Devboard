/* C GAILLARD - Dieu de l'élec
This is the code for the the ESP32 as master (advancement 1), it's still a WIP! the final code will be available soon...
  */

#include "driver/i2c.h"

#define I2C_MASTER_SCL_IO 8
#define I2C_MASTER_SDA_IO 10
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 400000

#define SLAVE_ADDR 0x02
#define SIZE 8

#define ALTERNATE_FUNCT_OD 0x06
#define ALTERNATE_FUNCT_PP 0x07

uint8_t TxData[SIZE] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6, 0xAE, 0xFF};
uint8_t CmdDataFunctionA[SIZE] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x37, 0xFF};

uint8_t RxData[SIZE] = {0};

void i2c_master_init()
{
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

void setup()
{
  Serial.begin(115200);
  delay (7000);
  i2c_master_init();
  uint32_t flag1 = micros();
  IOX_pinMode (6, INPUT_PULLUP);
  delay (1000);
  uint32_t flag2 = micros();
  Serial.println (flag2 - flag1);
  flag1 = micros();
  IOX_digitalReadSample (6, 1);
  flag2 = micros();
  Serial.println (flag2 - flag1);
  delay (1000);
}

void loop()
{
  IOX_digitalReadSample (6, 100);
  for (int i = 0; i <= 500; i++) {
    uint32_t reading = IOX_digitalReadBuffer (6);
    for (int i = 31; i >= 0; i--) {
    Serial.print((reading >> i) & 1);
    }
    Serial.println ();
    delay (10);
  }

  IOX_digitalReadSample (6, 10);
  for (int i = 0; i <= 500; i++) {
    uint32_t reading = IOX_digitalReadBuffer (6);
    for (int i = 31; i >= 0; i--) {
    Serial.print((reading >> i) & 1);
    }
    Serial.println ();
    delay (10);
  }

  IOX_digitalReadSample (6, 1);
  for (int i = 0; i <= 500; i++) {
    uint32_t reading = IOX_digitalReadBuffer (6);
    for (int i = 31; i >= 0; i--) {
    Serial.print((reading >> i) & 1);
    }
    Serial.println ();
    delay (10);
  }
  IOX_digitalReadSample (6, 1000);
  for (int i = 0; i <= 500; i++) {
    uint32_t reading = IOX_digitalReadBuffer (6);
    for (int i = 31; i >= 0; i--) {
    Serial.print((reading >> i) & 1);
    }
    Serial.println ();
    delay (10);
  }

}

void sendI2CCommand (uint8_t *CmdData) {
  i2c_cmd_handle_t cmd = i2c_cmd_link_create(); 
  i2c_master_start(cmd); 
  i2c_master_write_byte(cmd, (SLAVE_ADDR << 1) | I2C_MASTER_WRITE, true);
  i2c_master_write(cmd, CmdData, SIZE, true); 
  i2c_master_stop(cmd);
  i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);

}

void retrieveI2CData () {
  i2c_cmd_handle_t cmd = i2c_cmd_link_create(); 
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (SLAVE_ADDR << 1) | I2C_MASTER_READ, true); 
  i2c_master_read(cmd, RxData, SIZE - 1, I2C_MASTER_ACK); 
  i2c_master_read_byte(cmd, &RxData[SIZE - 1], I2C_MASTER_NACK);
  i2c_master_stop(cmd);
  i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS); 
  i2c_cmd_link_delete(cmd);
}

void IOX_digitalWrite (uint8_t pin, uint8_t state) {
  uint32_t initTime = micros();
  uint8_t CMD [SIZE] = {0x10, pin, state, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  sendI2CCommand (CMD);
  while (initTime + 1000 >= micros());
}

void IOX_analogWritePWM (uint8_t pin, uint16_t duty_cycle, uint32_t freqOUT) {
  uint32_t initTime = micros();
  uint8_t CMD [SIZE] = {0x11, pin, (duty_cycle >> 8) & 0xFF, duty_cycle & 0xFF, (freqOUT >> 24) & 0xFF, (freqOUT >> 16) & 0xFF, (freqOUT >> 8) & 0xFF, freqOUT & 0xFF};
  sendI2CCommand (CMD);
  while (initTime + 1000 >= micros());
}
void IOX_pinMode (uint8_t pin, uint8_t state) {
  uint32_t initTime = micros();
  uint8_t CMD [SIZE] = {0x12, (uint8_t)pin, (uint8_t)state, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  sendI2CCommand (CMD);
  while (initTime + 1000 > micros());
}

void IOX_digitalReadSample (uint8_t pin, uint16_t interval) {
  uint32_t initTime = micros();
  uint8_t CMD [SIZE] = {0x13, pin, (interval >> 8) & 0xFF, interval & 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  sendI2CCommand (CMD);
  while (initTime + 1000 > micros());
}

uint32_t  IOX_digitalReadBuffer (uint8_t pin) {
  uint32_t initTime = micros();
  uint8_t CMD [SIZE] = {0x14, pin, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  sendI2CCommand (CMD);
  while (initTime + 500 > micros());
  uint32_t diff = micros() - initTime;
  retrieveI2CData();
  while (initTime + 1000 > micros());
  uint32_t diff2 = micros() - initTime;
  uint32_t reading = (uint32_t)RxData[2] << 24 | (uint32_t)RxData[3] << 16 | (uint32_t)RxData[4] << 8 | (uint32_t)RxData[5];
  return reading;

}
