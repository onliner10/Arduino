#include <esp_now.h>
#include <WiFi.h>
#include <Preferences.h>
#include "driver/rtc_io.h"


#define uS_TO_S_FACTOR 1000000ULL /* Conversion factor for micro seconds to seconds */
#define MAX_RETRY_TIME 1024          /* Time ESP32 will go to sleep (in seconds) */
#define INITIAL_RETRY  16

unsigned int retryTime = 2;

Preferences preferences;


uint8_t broadcastAddress[] = {0x84, 0xFC, 0xE6, 0xC5, 0x33, 0x7A};

typedef struct struct_message {
  unsigned int usages;
  unsigned long last_open;
} struct_message;
// Create a struct_message called myData
struct_message payload;

esp_now_peer_info_t peerInfo;


void setup() {
  pinMode(GPIO_NUM_0, INPUT_PULLUP);
  rtc_gpio_pullup_en(GPIO_NUM_0);
  rtc_gpio_pulldown_dis(GPIO_NUM_0);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_0,0);

  pinMode(LED_BUILTIN, OUTPUT);
  preferences.begin("sender", false);

  // Init Serial Monitor
  Serial.begin(115200);
  blink_init();
  Serial.println("Starting sensor...");
  digitalWrite(LED_BUILTIN, 1);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    critical_error("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    critical_error("Failed to add peer");
    return;
  }

  payload.usages = 0;
  payload.last_open = 0;

  handler();

  Serial.println("Going to sleep now");
  digitalWrite(LED_BUILTIN, 0);
  preferences.end();

  Serial.flush();
  esp_deep_sleep_start();
}

void handler(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : handle_input(); break;
    case ESP_SLEEP_WAKEUP_EXT1 : critical_error("Did not expect EXT1 wake up!"); break;
    case ESP_SLEEP_WAKEUP_TIMER : handle_timer(); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : critical_error("Did not expect TOUCHPAD wake up!"); break;
    case ESP_SLEEP_WAKEUP_ULP : critical_error("Did not expect ULP wake up!"); break;
    default : initialize_time(); break;
  }
}

void handle_timer() {
  Serial.println("Retrying to send the data");
  send_data();
}

void handle_input() {
  Serial.println("Input handler");

  payload.usages = preferences.getUInt("usages", 0);
  payload.usages++;
  payload.last_open = getTime();
  preferences.putULong("last_open", payload.last_open);
  preferences.putUInt("usages", payload.usages);
  send_data();
}

void send_data() {
  payload.usages = preferences.getUInt("usages", 0);
  payload.last_open = preferences.getULong("last_open", 0);
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &payload, sizeof(payload));
   
  if (result == ESP_OK) {
    retryTime = INITIAL_RETRY;
    blink_sent();
  }
  else {
    error("Error sending the data, will retry");
    // retry in 30s
    Serial.flush();
    scheduleRetry();
  }
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if(status != ESP_NOW_SEND_SUCCESS) {
    scheduleRetry();
    Serial.flush();
    esp_deep_sleep_start();
  }
}

void scheduleRetry() {
  esp_sleep_enable_timer_wakeup(retryTime * uS_TO_S_FACTOR);
  retryTime = retryTime * 2;
  if(retryTime >= MAX_RETRY_TIME) {
    retryTime = MAX_RETRY_TIME;
  }
}
 
void loop() {
}