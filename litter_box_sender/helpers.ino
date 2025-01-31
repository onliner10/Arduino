#define LED_BUILTIN 15

void(* resetFunc) (void) = 0;

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

void error(const char* msg) {
  Serial.println(msg);
  blinker(500, 3);
}

void critical_error(const char* msg) {
  Serial.println(msg);
  blinker(1000, 5);
  resetFunc();
}

void blink_sent() {
  blinker(100, 10);
}