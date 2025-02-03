#include <esp_now.h>
#include <WiFi.h>
#include <Preferences.h>
#include "driver/rtc_io.h"


#define uS_TO_S_FACTOR 1000000ULL /* Conversion factor for micro seconds to seconds */
#define CONTACTRON GPIO_NUM_1
#define TILT_SENSOR GPIO_NUM_2
#define DEBOUNCE_OPEN_SECONDS 5
#define DEBOUNCE_ROTATE_SECONDS 5

Preferences preferences;
unsigned int usages = 0;
unsigned long last_open = 0;

void setup() {
  pinMode(CONTACTRON, INPUT_PULLDOWN);
  pinMode(TILT_SENSOR, INPUT_PULLDOWN);
  attach_interrupts();

  pinMode(LED_BUILTIN, OUTPUT);
  preferences.begin("sender", false);

  // Init Serial Monitor
  Serial.begin(115200);
  blink_init();
  Serial.println("Starting sensor...");
  digitalWrite(LED_BUILTIN, 1);
  debug_gpio();
 
  initComm();
  handler();
  endComm();

  Serial.println("Going to sleep now");
  digitalWrite(LED_BUILTIN, 0);
  preferences.end();

  Serial.flush();
  esp_deep_sleep_start();
}

void debug_gpio() {
  int state;
  state = digitalRead(CONTACTRON);
  Serial.print("Contractron: ");
  Serial.print(state);
  Serial.print(", ");
  state = digitalRead(TILT_SENSOR);
  Serial.print("Tilt: ");
  Serial.println(state);
}

void handler(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : critical_error("Did not expect EXT0 wake up!"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : handle_input(); break;
    case ESP_SLEEP_WAKEUP_TIMER : handle_timer(); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : critical_error("Did not expect TOUCHPAD wake up!"); break;
    case ESP_SLEEP_WAKEUP_ULP : critical_error("Did not expect ULP wake up!"); break;
    default : initialize_time(); break;
  }
}

// Attach interrupts back after debounce period
void handle_timer() {
  Serial.println("TIMER HANDLER");
  attach_interrupts();

  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
  Serial.flush();
  esp_deep_sleep_start();
}

void attach_interrupts() {
  rtc_gpio_pullup_dis(CONTACTRON);
  rtc_gpio_pulldown_en(CONTACTRON);
  rtc_gpio_pullup_dis(TILT_SENSOR);
  rtc_gpio_pulldown_en(TILT_SENSOR);

  const uint64_t wakeup_pins = (1ULL << CONTACTRON) | (1ULL << TILT_SENSOR);

  esp_sleep_enable_ext1_wakeup(wakeup_pins, ESP_EXT1_WAKEUP_ANY_LOW);
}

void handle_input() {
  auto debounce = 1;
  Serial.println("INPUT HANDLER");

  // just to eliminate "wobbles"
  delay(1000);

  usages = preferences.getUInt("usages", 0);
  if(usages != 0 && digitalRead(CONTACTRON) == 0) {
    debounce = DEBOUNCE_OPEN_SECONDS;
    last_open = getTime();
    preferences.putULong("last_open", last_open);
    usages = 0;
  } else if(digitalRead(TILT_SENSOR) == 0) {
    debounce = DEBOUNCE_ROTATE_SECONDS;
    usages += 1;
  }

  preferences.putUInt("usages", usages);

  send_data();
  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_EXT1);

  // 5 minute debounce
  esp_sleep_enable_timer_wakeup(debounce * uS_TO_S_FACTOR);
}

void send_data() {
  usages = preferences.getUInt("usages", 0);
  last_open = preferences.getULong("last_open", 0);
  sendComm(usages, last_open);
   
  blink_sent();
}

void loop() {
}