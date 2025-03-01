
void(* resetFunc) (void) = 0;

void blinker(unsigned int interval, unsigned int n) {
  for(int i = 0;i<n;i++) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(interval);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(interval);
  }

  digitalWrite(LED_BUILTIN, HIGH);
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


void inputPulldownSesnse( uint32_t ulPin )
{
  if (ulPin >= PINS_COUNT) {
    return;
  }

  ulPin = g_ADigitalPinMap[ulPin];

  NRF_GPIO_Type * port = nrf_gpio_pin_port_decode(&ulPin);

  // Set pin to input mode with pull-down resistor enabled and sense when High
  port->PIN_CNF[ulPin] = ((uint32_t)GPIO_PIN_CNF_DIR_Input        << GPIO_PIN_CNF_DIR_Pos)
                        | ((uint32_t)GPIO_PIN_CNF_INPUT_Connect    << GPIO_PIN_CNF_INPUT_Pos)
                        | ((uint32_t)GPIO_PIN_CNF_PULL_Pulldown    << GPIO_PIN_CNF_PULL_Pos)
                        | ((uint32_t)GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos)
                        | ((uint32_t)GPIO_PIN_CNF_SENSE_High       << GPIO_PIN_CNF_SENSE_Pos);

}
