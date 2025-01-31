#include "WiFi.h"
#define LED_BUILTIN 15

// ESP32-S2
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);

  while(!Serial);

  digitalWrite(LED_BUILTIN, LOW);
  Serial.println(WiFi.macAddress());
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
