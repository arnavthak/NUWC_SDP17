#define CMD_SET_LOW   0x0F
#define CMD_SET_HIGH  0xFF
#define CMD_READ      0x6F

void setup() {
  // 1. Start Serial at the SAME speed as your Qt app
  Serial.begin(115200); 
  
  // 2. Wait a moment for the connection to stabilize
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  if (Serial.available() >= 4) {
    
    // read packet
    byte cmd = Serial.read();
    byte pin = Serial.read();
    byte crc1 = Serial.read();
    byte crc2 = Serial.read();

    pinMode(pin, OUTPUT);

    switch (cmd) {
      case CMD_SET_LOW:
        digitalWrite(pin, LOW);
        Serial.print("Pin ");
        Serial.print(pin);
        Serial.println(" is set to Low.");
        break;
      
      case CMD_SET_HIGH:
        digitalWrite(pin, HIGH);
        Serial.print("Pin ");
        Serial.print(pin);
        Serial.println(" is set to High.");
        break;
      
      case CMD_READ:
        pinMode(pin, INPUT);
        int value = digitalRead(pin);
        Serial.print("Pin: ");
        Serial.print(pin);
        Serial.print(" Value: ");
        Serial.println(value);
        break;
    }
  }
}