/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by pcbreflux
   Modified to work with NimBLE
   Modified for V2 by Chreece
   Modified by countrysideboy: Code cleanups, Chop Data, Encryption
   BLE advertisement format from https://bthome.io/

*/

#include <BTHome.h>

#define DEVICE_NAME "Kuwetka"  // The name of the sensor
// #define DEVICE_NAME_LENGTH (sizeof(DEVICE_NAME) - 1)

// Create a global instance of the BTHome class
BTHome bthome;
time_t currentTime;
uint8_t counter = 0;
uint8_t uint48Buffer[6] = {0,0,0,0,0,0};

void setup() {
  Serial.begin(115200);

  Serial.println("Creating the BTHome BLE device...");
  bthome.begin(DEVICE_NAME, false, "", false);

  initialize_time();
}

void loop() {
  counter += 1;
  currentTime = getTime();
  // timeToByteArray(currentTime, uint48Buffer);
  //MEASUREMENT_MAX_LEN = 23, ENABLE_ENCRYPT will use extra 8 bytes, so each Measurement should smaller than 15

  // 1st method: just addMeasurement as much as you can, the code will split and send the adv packet automatically
  // each adv packet sending lasts for 1500ms
  bthome.resetMeasurement();
  // bthome.addMeasurement(sensorid, value) you can use the sensorids from the BTHome.h file
  // *** The Object ids of addMeasurement have to be applied in numerical order (from low to high) in your advertisement ***
  bthome.addMeasurement_state(STATE_POWER_ON, STATE_ON);//2
  bthome.addMeasurement_state(EVENT_BUTTON, EVENT_BUTTON_PRESS);
  bthome.addMeasurement(ID_COUNT, (uint64_t)counter);
  bthome.addMeasurement(ID_TIMESTAMP,(uint64_t)currentTime);

  bthome.sendPacket();
  bthome.stop();

  delay(10000);
}

//Object ids by order
#if 0
#define ID_PACKET				0x00
#define ID_BATTERY				0x01
#define ID_COUNT				0x09
#define STATE_POWER_ON			0x10
#define STATE_BATTERY_LOW		0x15
#define STATE_PROBLEM			0x26
#define STATE_RUNNING			0x27
#define EVENT_BUTTON			0x3A
#endif