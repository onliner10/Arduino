#include <BTHome.h>

#define DEVICE_NAME "Kuwetka"

BTHome bthome;

void initComm() {
    bthome.begin(DEVICE_NAME, false, "", false);
    Serial.println("Initialized communications");
}

void sendComm(unsigned int usages, time_t last_opened) {
  bthome.resetMeasurement();
  
  // TODO: Add battery voltage
  bthome.addMeasurement(ID_COUNT, (uint64_t)usages);
  bthome.addMeasurement(ID_TIMESTAMP,(uint64_t)last_opened);

  bthome.sendPacket();
  bthome.stop();
}

void endComm() {
    // Deinitialization code here
}