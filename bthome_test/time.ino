#include "esp_netif_sntp.h"
#include "esp_sntp.h"
#include <time.h>
#include <sys/time.h>
#include <WiFi.h>

const char* ntpServer = "pool.ntp.org";

time_t getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

void initialize_time() {
  sntp_sync_status_t syncStatus;

  Serial.println("Setting up time..");
  Serial.println("Connecting to WiFi ..");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  configTime(0, 0, ntpServer);
  syncStatus = sntp_get_sync_status();
  while (syncStatus != SNTP_SYNC_STATUS_COMPLETED) {
      syncStatus = sntp_get_sync_status();
      delay(100); // Adjust the delay time as per your requirements
  }
  esp_sntp_stop();
  WiFi.disconnect(true); 
  WiFi.mode(WIFI_OFF);
  Serial.print("Current time: ");
  Serial.print(getTime());
  Serial.println();
}