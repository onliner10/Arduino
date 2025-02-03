void initComm() {
    // Initialization code here
    Serial.println("Initialized communications");
}

void sendComm(unsigned int usages, unsigned long last_used) {
  Serial.print("Usages: ");
  Serial.print(usages);
  Serial.print(", Last open: ");
  Serial.println(last_used);
}

void endComm() {
    // Deinitialization code here
    Serial.println("Deinitialized communications");
}