#include <esp_now.h>
#include <WiFi.h>

void blinker(unsigned int interval, unsigned int n) {
  for(int i = 0;i<n;i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(interval);
    digitalWrite(LED_BUILTIN, LOW);
    delay(interval);
  }
}

void blink_init() {
  blinker(100, 10);
}

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  unsigned int usages;
  unsigned int last_open;
} struct_message;

// Create a struct_message called myData
struct_message receivedData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Usages: ");
  Serial.println(receivedData.usages);
  Serial.print("Last open: ");
  Serial.println(receivedData.last_open);
  Serial.println();
}
 
void setup() {
  blink_init();
  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial.println("Starting receiver");
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  if(esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv)) != ESP_OK) {
    Serial.println("Error initializing receive callback!");
  };
}
 
void loop() {
}