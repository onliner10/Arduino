#include <nrf52840.h>
#include "LSM6DS3.h"

#define CONTACTRON 1
#define DEVICE_NAME "Kuwetka"
#define DEBOUNCE 300000

LSM6DS3 lsm6ds3(I2C_MODE, 0x6A);    
uint32_t latchState = 0;

void setup() {
  Serial.begin(9600);
  int initError = lsm6ds3.begin();
  if(0 != initError) {
    Serial.println("Failed to configure lsm6ds3");
  }
  if (0 != configure_wake()) {
      Serial.println("Fail to configure!");
  } 

  inputPulldownSesnse(CONTACTRON);
  inputPulldownSesnse(PIN_LSM6DS3TR_C_INT1);

  // bthome.begin(DEVICE_NAME, "", false);
}

void loop() {
  latchState = NRF_GPIO->LATCH;

  if(digitalRead(CONTACTRON) == HIGH) {
    bool shouldSend = true;

    // Probing for 5 seconds to avoid false positives
    for(int i = 0; i < 40;i++) {
      if(digitalRead(CONTACTRON) == LOW) {
        shouldSend = false;
        break;
      }
      delay(125);
    }

    if(shouldSend) {
      sendData(true, false);
    }
  } else if(latchState & 2048 && digitalRead(CONTACTRON) == LOW) {
    sendData(false, true);
  }

  delay(DEBOUNCE);
 
  reset_latch();
  go_sleep();
}

void reset_latch() {
  NRF_GPIO->LATCH = NRF_GPIO->LATCH;  
  (void)NRF_TIMER0->EVENTS_COMPARE[0]; 
}

void go_sleep() {
  nrf_power_system_off(NRF_POWER);
  // LowPower.deepSleep();
}


int configure_wake(void) {
    uint8_t error = 0;
    uint8_t dataToWrite = 0;

    dataToWrite |= LSM6DS3_ACC_GYRO_BW_XL_50Hz;
    dataToWrite |= LSM6DS3_ACC_GYRO_FS_XL_2g;
    dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_13Hz;

    error += lsm6ds3.writeRegister(LSM6DS3_ACC_GYRO_CTRL1_XL, dataToWrite);
    
    error += lsm6ds3.writeRegister(LSM6DS3_ACC_GYRO_TAP_CFG1, 0x90);
    error += lsm6ds3.writeRegister(LSM6DS3_ACC_GYRO_WAKE_UP_DUR, 0xB4); //180 
    error += lsm6ds3.writeRegister(LSM6DS3_ACC_GYRO_WAKE_UP_THS, 0x02); 
    error += lsm6ds3.writeRegister(LSM6DS3_ACC_GYRO_MD1_CFG, 0x20);

    return error;
}