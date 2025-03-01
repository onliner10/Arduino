void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Initializing");
  setupMotor();
  int distInitErr = setupDistanceSensors();
  if (distInitErr != 0) {
    Serial.print("Error initializing sensors, code: ");
    Serial.println(distInitErr);
  }

  Serial.println("Initialized");
}

void loop() {
  uint16_t dist1, dist2, dist3;
  readDistanceSensors(dist1, dist2, dist3);

  Serial.print("Distance 1: ");
  Serial.print(dist1);
  Serial.print(" mm, Distance 2: ");
  Serial.print(dist2);
  Serial.print(" mm, Distance 3: ");
  Serial.print(dist3);
  Serial.println(" mm");

  delay(1000);
  // if (Serial.available()) {
  //   String command = Serial.readStringUntil('\n');  // Read input until newline
  //   command.trim();  // Remove any extra spaces or newlines

  //   if(command == "l") {
  //     steerLeft();
  //   } else if(command == "r") {
  //     steerRight();
  //   } else if(command == "f") {
  //     forward(150);
  //   } else if(command == "b") {
  //     backwards(150);
  //   } else {
  //     motorsIdle();
  //   }
  // }
}