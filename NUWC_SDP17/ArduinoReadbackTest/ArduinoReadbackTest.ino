void setup() {
  // 1. Start Serial at the SAME speed as your Qt app
  Serial.begin(9600); 
  
  // 2. Wait a moment for the connection to stabilize
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  // 3. Check if Qt sent us anything
  if (Serial.available() > 0) {
    
    // Read the incoming string until a newline character
    String incomingData = Serial.readStringUntil('\n');
    
    // 4. Send it BACK to Qt (Verification)
    Serial.println(incomingData);
    
    // Optional: Turn on built-in LED briefly to show activity
    digitalWrite(LED_BUILTIN, HIGH);
    delay(10);
    digitalWrite(LED_BUILTIN, LOW);
  }
}