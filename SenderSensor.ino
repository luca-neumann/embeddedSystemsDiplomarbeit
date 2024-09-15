#include <SPI.h>
#include <LoRa.h>
#include <U8x8lib.h>
#include <U8g2lib.h>

//Paketzähler ID setzen
int readingID = 0;

int counter = 0;
String LoRaMessage = "";

#define ss 18
#define rst 14
#define dio0 26

//ISM Band festlegen (hier in Europa 868MHZ)
#define BAND 866E6

// Display passend konfigurieren 
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(15, 4, 16);

// Um Grafiken umzuwandeln sollte diese in S/W sein die passende Größe hier 128x64 Pixel haben
// und als xbm exportiert werden.
static unsigned char TK_OLED_SOURCE_bits[] = {
   // Dein XBM Bilddaten hier...
};

#define uS_TO_S_FACTOR 1000000  // Umberechnungsfaktor Mikro Sekunden auf Sekunden 
#define TIME_TO_SLEEP  120      // Dauer des Tiefschlafzustands in Sekunden 

RTC_DATA_ATTR int bootCount = 0;


//Sensor
// Define the RX and TX pins for hardware serial
const int rxPin = 16;
const int txPin = 17;

// Create a HardwareSerial object (Serial1 in case of ESP32)
HardwareSerial pmsSerial(2);

unsigned char data[32];  // Data buffer for reading sensor data

void setup() {
  u8g2.begin();
  Serial.begin(115200);
  Serial.println("");
  Serial.println("----------------------------");
  Serial.println("  LoRa Sender ");
  Serial.println("----------------------------");
  Serial.println("");
  Serial.println("Booting...");

  u8g2.clearBuffer();          // Internen Display Speicher komplett löschen 
  u8g2.drawXBM(0,0, 128, 64, TK_OLED_SOURCE_bits);
  u8g2.sendBuffer();          // Inhalt des Speichers direkt auf das Display pushen

  delay(2000);  

  // Start the hardware serial communication for the PMS7003
  pmsSerial.begin(115200, SERIAL_8N1, rxPin, txPin);

  delay(2000); 

  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0, 2, " Sending Data ");
  u8x8.drawString(0, 3, "     &       ");
  u8x8.drawString(0, 4, " Deep Sleep  ");
  u8x8.drawString(0, 6, "..for 120 sec");
  delay(3000);
  u8x8.clear();

  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(868E6)) {
    Serial.println("Start of LoRa failed!");
    while (1);
  }
}

void loop() {
  // Testnachricht senden
  Serial.println("Sending test message...");

  // Anfang lesen sensor
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

        LoRa.beginPacket();
        LoRa.print(PM1_0);
        LoRa.print(PM2_5);
        LoRa.print(PM10);
        LoRa.endPacket();
        
        delay(1000);  // Wait a second before reading again
      }
    }
  }
  // Ende lesen Sensor

  Serial.println("LoRa Message sent.");
}
