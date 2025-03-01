#include <BTHome.h>

#define DEVICE_NAME "Kuwetka"
#define RETRIES 3

uint64_t counter = 0;
BTHome bthome;

void sendData(bool open, bool rotation) {
  bthome.begin(DEVICE_NAME, "", false);
  for(int i = 0;i<RETRIES;i++) {
    bthome.addMeasurement(ID_COUNT, counter++);
        
    bthome.sendPacket();

    bthome.resetMeasurement();
    delay(1000);
  }

  bthome.addMeasurement_state(EVENT_BUTTON, open ? EVENT_BUTTON_PRESS : EVENT_BUTTON_NONE);
  bthome.addMeasurement_state(EVENT_BUTTON, rotation ? EVENT_BUTTON_PRESS : EVENT_BUTTON_NONE);
  bthome.sendPacket();
}