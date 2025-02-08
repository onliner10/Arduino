#include <BTHome.h>

#define DEVICE_NAME "Kuwetka"

BTHome bthome;

void initComm() {
    bthome.begin(DEVICE_NAME, false, "", false);
    Serial.println("Initialized communications");
}

void reportSensors(unsigned int usages) {
  bthome.resetMeasurement();
  
  // Add battery voltage
  bthome.addMeasurement(ID_COUNT, (uint64_t)usages);

  bthome.sendPacket();
  bthome.stop();
}

void endComm() {
    // Deinitialization code here
}