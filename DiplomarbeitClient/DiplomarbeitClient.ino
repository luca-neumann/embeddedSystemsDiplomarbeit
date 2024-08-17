// Define the RX and TX pins for hardware serial
const int rxPin = 16;
const int txPin = 17;

// Create a HardwareSerial object (Serial1 in case of ESP32)
HardwareSerial pmsSerial(2);

unsigned char data[32];  // Data buffer for reading sensor data

void setup() {
  // Start the hardware serial communication for debug
  Serial.begin(9600);
  // Start the hardware serial communication for the PMS7003
  pmsSerial.begin(9600, SERIAL_8N1, rxPin, txPin);
}

void loop() {
  Serial.println("error");
  if (pmsSerial.available()) {
    Serial.println("Data available from sensor.");
    // Wait until there is enough data (32 bytes) to read
    if (pmsSerial.read() == 0x42) {  // Header byte 1
      if (pmsSerial.read() == 0x4d) {  // Header byte 2
        for (int i = 0; i < 30; i++) {
          data[i] = pmsSerial.read();
        }
        
        // Data parsing
        unsigned int PM1_0 = ((data[10] << 8) + data[11]);  // PM1.0 concentration (ug/m3)
        unsigned int PM2_5 = ((data[12] << 8) + data[13]);  // PM2.5 concentration (ug/m3)
        unsigned int PM10  = ((data[14] << 8) + data[15]);  // PM10 concentration (ug/m3)
        
        // Print the PM values to the serial monitor
        Serial.print("PM1.0: ");
        Serial.print(PM1_0);
        Serial.print(" ug/m3, ");
        
        Serial.print("PM2.5: ");
        Serial.print(PM2_5);
        Serial.print(" ug/m3, ");
        
        Serial.print("PM10: ");
        Serial.print(PM10);
        Serial.println(" ug/m3");
        
        delay(1000);  // Wait a second before reading again
      }
    }
  }
}
