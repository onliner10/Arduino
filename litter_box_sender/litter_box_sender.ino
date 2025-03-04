#include <nrf52840.h>
#include "LSM6DS3.h"

#define CONTACTRON 1
#define DEBOUNCE_OPEN_SECONDS 5
#define DEBOUNCE_ROTATE_SECONDS 5

LSM6DS3 lsm6ds3(I2C_MODE, 0x6A);    
uint32_t latchState = 0;

void setup() {
  // Init Serial Monitor
  Serial.begin(9600);

  int initError = lsm6ds3.begin();
  if(0 != initError) {
    Serial.println("Failed to configure lsm6ds3");
  }

  if (initError != 0) {
    Serial.println("Device error");
  } 
  if (0 != configure_wake()) {
      Serial.println("Fail to configure!");
  } 
}

void loop() {
  handler();
  inputPulldownSesnse(CONTACTRON);
  inputPulldownSesnse(PIN_LSM6DS3TR_C_INT1);
  
  reset_latch();
  go_sleep();
}

void handler(){
  latchState = NRF_GPIO->LATCH;
  
  Serial.println("INPUT HANDLER");
  Serial.print("Latch");
  Serial.println(latchState);
  int debounce = DEBOUNCE_OPEN_SECONDS; 
  float y = random(1, 500) / 100.0;

  if(latchState & 8) {
    debounce = DEBOUNCE_OPEN_SECONDS;
    // delay(2000);

    if(digitalRead(CONTACTRON) == HIGH) {
      sendData(true, false);
    }
  } else if(latchState & 2048) {
    sendData(false, true);
  }

  // debounce
  Serial.print("Debouncing for ");
  Serial.print(debounce);
  Serial.println(" seconds");
  delay(debounce * 1000);
}

void reset_latch() {
  NRF_GPIO->LATCH = NRF_GPIO->LATCH;  
  (void)NRF_TIMER0->EVENTS_COMPARE[0]; 
}

void go_sleep() {
  nrf_power_system_off(NRF_POWER);
}

int configure_wake(void) {
    uint8_t error = 0;
    uint8_t dataToWrite = 0;

    dataToWrite |= LSM6DS3_ACC_GYRO_BW_XL_50Hz;
    dataToWrite |= LSM6DS3_ACC_GYRO_FS_XL_2g;
    dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_13Hz;

    error += lsm6ds3.writeRegister(LSM6DS3_ACC_GYRO_CTRL1_XL, dataToWrite);
    
    error += lsm6ds3.writeRegister(LSM6DS3_ACC_GYRO_TAP_CFG1, 0x90);
    error += lsm6ds3.writeRegister(LSM6DS3_ACC_GYRO_WAKE_UP_DUR, 0xff); 
    error += lsm6ds3.writeRegister(LSM6DS3_ACC_GYRO_WAKE_UP_THS, 0x02); 
    error += lsm6ds3.writeRegister(LSM6DS3_ACC_GYRO_MD1_CFG, 0x20);

    return error;
}