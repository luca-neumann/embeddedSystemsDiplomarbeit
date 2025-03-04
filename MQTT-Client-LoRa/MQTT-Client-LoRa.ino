#include <SPI.h>
#include <LoRa.h>
#include <U8x8lib.h>
#include <U8g2lib.h>

// LoRa PINs für Heltec ESP32 LoRa
#define ss 18
#define rst 14
#define dio0 26

// Frequenz für Europa (868 MHz)
#define BAND 868E6

// OLED Display für Heltec ESP32 LoRa
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(21, 22, U8X8_PIN_NONE);

// PMS7003 Sensor (Nur TX benötigt)
#define PMS_TX 12  // TX vom Sensor → ESP32 RX
HardwareSerial pmsSerial(1);  // Verwende Serial1 (UART1) für den Sensor

unsigned char data[32];  // Puffer für Sensordaten

void setup() {
  // OLED Display initialisieren
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  
  // Serielle Kommunikation starten
  Serial.begin(115200);
  Serial.println("\n----------------------------");
  Serial.println("  Heltec ESP32 LoRa Sender  ");
  Serial.println("----------------------------");
  
  u8x8.drawString(0, 1, "LoRa Start...");

  // Start der HardwareSerial Kommunikation für PMS7003
  pmsSerial.begin(9600, SERIAL_8N1, PMS_TX, -1);  // RX wird nicht genutzt
  
  delay(2000);

  // LoRa initialisieren
  SPI.begin(5, 19, 27, 18);  // SPI-Pins für Heltec ESP32 LoRa
  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(BAND)) {
    Serial.println("LoRa Start fehlgeschlagen!");
    while (1);
  }

  Serial.println("LoRa erfolgreich gestartet!");
  u8x8.drawString(0, 2, "LoRa bereit!");
}

void loop() {
  if (pmsSerial.available() >= 32) {  // Mindestens 32 Bytes für einen gültigen Frame
    Serial.println("Daten vom Sensor empfangen!");

    // Daten lesen
    for (int i = 0; i < 32; i++) {
      data[i] = pmsSerial.read();
    }

    // Daten auswerten
    unsigned int PM1_0 = (data[10] << 8) | data[11];
    unsigned int PM2_5 = (data[12] << 8) | data[13];
    unsigned int PM10  = (data[14] << 8) | data[15];

    // Werte auf Serial Monitor ausgeben
    Serial.printf("PM1.0: %u ug/m3, PM2.5: %u ug/m3, PM10: %u ug/m3\n", PM1_0, PM2_5, PM10);

    // Überprüfen, ob ein Wert über 100 ist, und dann keine LoRa-Nachricht senden
    if (PM1_0 <= 50 && PM2_5 <= 80 && PM10 <= 100) {
      // Werte auf OLED anzeigen
      u8x8.clear();
      u8x8.drawString(0, 1, "PM Werte:");
      u8x8.setCursor(0, 2);
      u8x8.printf("PM1.0: %u", PM1_0);
      u8x8.setCursor(0, 3);
      u8x8.printf("PM2.5: %u", PM2_5);
      u8x8.setCursor(0, 4);
      u8x8.printf("PM10 : %u", PM10);

      // LoRa Paket senden
      LoRa.beginPacket();
      String jsonOutput = "{\"Id\":2,\"PM1_0\":" + String(PM1_0) + ",\"PM2_5\":" + String(PM2_5) + ",\"PM10\":" + String(PM10) + "}";
      LoRa.print(jsonOutput);
      LoRa.endPacket();

      Serial.println("LoRa-Paket gesendet!");
      u8x8.drawString(0, 6, "LoRa gesendet!");
    } else {
      Serial.println("Ein Wert ist zu hoch, LoRa Paket wird nicht gesendet.");
      u8x8.drawString(0, 6, "Werte zu hoch!");
    }

    delay(5000);  // 2s warten bis zur nächsten Messung
  } else {
    Serial.println("Keine Daten vom Sensor...");
    u8x8.clear();
    u8x8.drawString(0, 1, "Kein Sensor!");
    delay(5000);
  }
}
