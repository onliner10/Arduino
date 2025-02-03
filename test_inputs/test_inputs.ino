#define CONTACTRON GPIO_NUM_1
#define TILT_SENSOR GPIO_NUM_2

int contactron = 0;  
int tilt = 0;  

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  // put your setup code here, to run once:
  pinMode(CONTACTRON, INPUT_PULLDOWN);
  pinMode(TILT_SENSOR, INPUT_PULLDOWN);
}

void loop() {
  // put your main code here, to run repeatedly:
  contactron = digitalRead(CONTACTRON); 
  tilt = digitalRead(TILT_SENSOR); 
  Serial.print("Contactron:");
  Serial.print(contactron);
  Serial.print(",");
  Serial.print("Tilt:");
  Serial.println(tilt);
  delay(250);
}
