#define STEERING_MOTOR_1 GPIO_NUM_8
#define STEERING_MOTOR_2 GPIO_NUM_9

#define DRIVE_MOTOR_1 GPIO_NUM_10
#define DRIVE_MOTOR_2 GPIO_NUM_11

#define MOTOR_FAULT GPIO_NUM_5
#define MOTOR_SLEEP GPIO_NUM_2
#define MOTOR_FLT GPIO_NUM_3

void setupMotor() {
  pinMode(MOTOR_SLEEP, OUTPUT);
  pinMode(MOTOR_FAULT, INPUT_PULLUP);
  pinMode(STEERING_MOTOR_1, OUTPUT);
  pinMode(STEERING_MOTOR_2, OUTPUT);
  
  pinMode(DRIVE_MOTOR_1, OUTPUT);
  pinMode(DRIVE_MOTOR_2, OUTPUT);

  analogWriteResolution(STEERING_MOTOR_1, 8);
  analogWriteResolution(STEERING_MOTOR_2, 8);
  analogWriteResolution(DRIVE_MOTOR_1, 8);
  analogWriteResolution(DRIVE_MOTOR_2, 8);

  analogWrite(STEERING_MOTOR_1, 0);
  analogWrite(STEERING_MOTOR_2, 0);

  analogWrite(DRIVE_MOTOR_1, 0);
  analogWrite(DRIVE_MOTOR_2, 0);

  digitalWrite(MOTOR_SLEEP, HIGH);
}

void motorsIdle() {
  analogWrite(STEERING_MOTOR_1, 0);
  analogWrite(STEERING_MOTOR_2, 0);

  analogWrite(DRIVE_MOTOR_1, 0);
  analogWrite(DRIVE_MOTOR_2, 0);
}

void steerLeft() {
  analogWrite(STEERING_MOTOR_1, 0);
  analogWrite(STEERING_MOTOR_2, 255);
}

void steerRight() {
  analogWrite(STEERING_MOTOR_1, 255);
  analogWrite(STEERING_MOTOR_2, 0);
}

void forward(uint8_t speed) {
  analogWrite(DRIVE_MOTOR_1, 0);
  analogWrite(DRIVE_MOTOR_2, speed);
}

void backwards(uint8_t speed) {
  analogWrite(DRIVE_MOTOR_1, speed);
  analogWrite(DRIVE_MOTOR_2, 0);
}