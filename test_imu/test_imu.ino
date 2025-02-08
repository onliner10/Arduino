#include <nrf52840.h>

#include "LSM6DS3.h"
#include "Wire.h"

#define int1Pin PIN_LSM6DS3TR_C_INT1
#define CONTACTRON_PIN 1

LSM6DS3 lsm6ds3(I2C_MODE, 0x6A);    
volatile uint16_t detectCount = 0;

void blinkLED(int ledPin, int interval, int numBlinks) {
  for (int i = 0; i < numBlinks; i++) {
    digitalWrite(ledPin, HIGH);
    delay(interval);
    digitalWrite(ledPin, LOW);
    delay(interval);
  }
}


void setup() {
    // 2048 seems to be the interrupt, 8 is contactron
    auto latchState = NRF_GPIO->LATCH;

    uint8_t readDataByte = 0;
    int initError = lsm6ds3.begin();

    pinMode(PIN_LSM6DS3TR_C_INT1, INPUT_PULLDOWN_SENSE);
    lsm6ds3.readRegister(&readDataByte, LSM6DS3_ACC_GYRO_D6D_SRC); 

    pinMode(LED_BUILTIN, OUTPUT);
    blinkLED(LED_BUILTIN, 100, 3);

    Serial.begin(9600);
    while(!Serial);

    if (initError != 0) {
        Serial.println("Device error");
    } 

    if(latchState == 2048) {
      Serial.println("Rotation detected!");
    } else if(latchState == 8) {
      Serial.println("Open detected!");
    }

    readDataByte &= 0x40;
    if (readDataByte) {
        detectCount ++;
        Serial.println("ANGLE CHANGED");
        blinkLED(LED_BUILTIN, 100, 20);
    } 
    delay(1000);

    if (0 != configure_angle()) {
        Serial.println("Fail to configure!");
    } 
    delay(1000);
    //Reset the latch before sleep
    lsm6ds3.readRegister(&readDataByte, LSM6DS3_ACC_GYRO_D6D_SRC); 

    pinMode(PIN_LSM6DS3TR_C_INT1, INPUT_PULLDOWN_SENSE);
    pinMode(CONTACTRON_PIN, INPUT_PULLDOWN_SENSE);

    digitalWrite(LED_BUILTIN, HIGH);
    Serial.flush();

    NRF_GPIO->LATCH = NRF_GPIO->LATCH;  
	  (void)NRF_TIMER0->EVENTS_COMPARE[0]; 
	
    NRF_POWER->SYSTEMOFF = 1;
}

void loop() {
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
