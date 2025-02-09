#include <nrf52840.h>
#include "LSM6DS3.h"
#include <BTHome.h>

// #define PIN_LSM6DS3TR_C_INT1 (18)

#define CONTACTRON 1
#define DEBOUNCE_OPEN_SECONDS 1200
#define DEBOUNCE_ROTATE_SECONDS 600
#define DEVICE_NAME "Kuwetka"

BTHome bthome;
LSM6DS3 lsm6ds3(I2C_MODE, 0x6A);    

uint8_t readDataByte = 0;

void setup() {
  // Init Serial Monitor
  Serial.begin(9600);
  blink_init();
  Serial.println("Starting sensor...");

  inputPulldownSesnse(CONTACTRON);
  int initError = lsm6ds3.begin();

  inputPulldownSesnse(PIN_LSM6DS3TR_C_INT1);
  lsm6ds3.readRegister(&readDataByte, LSM6DS3_ACC_GYRO_D6D_SRC); 

  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, 1);

  if (initError != 0) {
    Serial.println("Device error");
  } 
  if (0 != configure_angle()) {
      Serial.println("Fail to configure!");
  } 
  delay(1000);
  handler();

  Serial.println("Going to sleep now");
  digitalWrite(LED_BUILTIN, 0);

  go_sleep();
}

void handler(){
  auto latchState = NRF_GPIO->LATCH;
  reset_latch();

  Serial.println("INPUT HANDLER");
  Serial.print("Latch");
  Serial.println(latchState);
  auto debounce = DEBOUNCE_OPEN_SECONDS; // 5 min
  
  bthome.begin(DEVICE_NAME, "", false);
  bthome.resetMeasurement();

  if(latchState & 8) {
    debounce = DEBOUNCE_OPEN_SECONDS;
    Serial.println("Open detected");
    bthome.addMeasurement_state(EVENT_BUTTON, EVENT_BUTTON_PRESS);
    bthome.addMeasurement_state(EVENT_BUTTON, EVENT_BUTTON_NONE);
  } else if(latchState & 2048) {
    debounce = DEBOUNCE_ROTATE_SECONDS;

    Serial.println("Rotation detected");
    bthome.addMeasurement_state(EVENT_BUTTON, EVENT_BUTTON_NONE);
    bthome.addMeasurement_state(EVENT_BUTTON, EVENT_BUTTON_PRESS);
  }
  
  bthome.sendPacket();
  delay(10000);
  bthome.stop();

  // debounce
  Serial.print("Debouncing for ");
  Serial.print(debounce);
  Serial.println(" seconds");
  delay(debounce * 1000);
  go_sleep();
}

void reset_latch() {

  //Reset the latch before sleep
  lsm6ds3.readRegister(&readDataByte, LSM6DS3_ACC_GYRO_D6D_SRC); 

  NRF_GPIO->LATCH = NRF_GPIO->LATCH;  
  (void)NRF_TIMER0->EVENTS_COMPARE[0]; 
}

void go_sleep() {
  Serial.flush();

  NRF_POWER->SYSTEMOFF = 1;
}

int configure_angle(void) {
    uint8_t error = 0;
    uint8_t dataToWrite = 0;

    dataToWrite |= LSM6DS3_ACC_GYRO_BW_XL_50Hz;
    dataToWrite |= LSM6DS3_ACC_GYRO_FS_XL_2g;
    dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_13Hz;

    error += lsm6ds3.writeRegister(LSM6DS3_ACC_GYRO_CTRL1_XL, dataToWrite);
    
    error += lsm6ds3.writeRegister(LSM6DS3_ACC_GYRO_TAP_CFG1, 0x81);
    error += lsm6ds3.writeRegister(LSM6DS3_ACC_GYRO_TAP_THS_6D, 0xc0); // c0 11000000 d4d enable, 60 degrees
    error += lsm6ds3.writeRegister(LSM6DS3_ACC_GYRO_CTRL8_XL, 0x01); // low pass
    error += lsm6ds3.writeRegister(LSM6DS3_ACC_GYRO_MD1_CFG, 0x04);

    return error;
}


void loop() {
}