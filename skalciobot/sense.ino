#include <Wire.h>
#include <VL53L0X.h>

#define SDA_PIN GPIO_NUM_33
#define SCL_PIN GPIO_NUM_35

#define DIST1_XSHUT GPIO_NUM_1
#define DIST2_XSHUT GPIO_NUM_2
#define DIST3_XSHUT GPIO_NUM_3

VL53L0X sensor1;
VL53L0X sensor2;
VL53L0X sensor3;

int setupDistanceSensors() {
  Serial.println("I2C initialization");
  Wire.begin(SDA_PIN, SCL_PIN);
  delay(500);
  Serial.println("I2C initialized");

  // Initialize the XSHUT pins
  pinMode(DIST1_XSHUT, OUTPUT);
  pinMode(DIST2_XSHUT, OUTPUT);
  pinMode(DIST3_XSHUT, OUTPUT);

  // Turn off all sensors
  digitalWrite(DIST1_XSHUT, LOW);
  digitalWrite(DIST2_XSHUT, LOW);
  digitalWrite(DIST3_XSHUT, LOW);
  delay(200);

  Serial.println("Initializing sensor 1");
  // Turn on sensor 1 and set its address
  pinMode(DIST1_XSHUT, INPUT);
  sensor1.setTimeout(500);
  delay(100); // Increased delay for stabilization
  if (!sensor1.init(true)) {
    Serial.println("Failed to initialize sensor 1");
    return 1; // Error code for sensor 1 initialization failure
  }
  delay(500);
  sensor1.setAddress(0x30); // New I2C address for sensor 1
  

  Serial.println("Initializing sensor 2");
  // Turn on sensor 2 and set its address
  pinMode(DIST2_XSHUT, INPUT);
  sensor2.setTimeout(500);
  delay(100); // Increased delay for stabilization
  if (!sensor2.init(true)) {
    Serial.println("Failed to initialize sensor 2");
    return 2; // Error code for sensor 2 initialization failure
  }
  delay(500);
  sensor2.setAddress(0x31); // New I2C address for sensor 2

  Serial.println("Initializing sensor 3");
  // Turn on sensor 3 and set its address
  pinMode(DIST3_XSHUT, INPUT);
  sensor3.setTimeout(500);
  delay(100); // Increased delay for stabilization
  if (!sensor3.init(true)) {
    Serial.println("Failed to initialize sensor 3");
    return 3; // Error code for sensor 3 initialization failure
  }
  delay(500);
  sensor3.setAddress(0x32); // New I2C address for sensor 3

  sensor1.startContinuous();
  sensor2.startContinuous();
  sensor3.startContinuous();

  return 0; // Success
}

void readDistanceSensors(uint16_t &dist1, uint16_t &dist2, uint16_t &dist3) {
  dist1 = sensor1.readRangeContinuousMillimeters();
  dist2 = sensor2.readRangeContinuousMillimeters();
  dist3 = sensor3.readRangeContinuousMillimeters();

  // Check for errors
  if (sensor1.timeoutOccurred()) { 
    Serial.println("Sensor 1 timeout");
    dist1 = 0; 
  }
  if (sensor2.timeoutOccurred()) { 
    Serial.println("Sensor 2 timeout");
    dist2 = 0; 
  }
  if (sensor3.timeoutOccurred()) { 
    Serial.println("Sensor 3 timeout");
    dist3 = 0; 
  }
}
