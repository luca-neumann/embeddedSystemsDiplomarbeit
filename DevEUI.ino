#include <Arduino.h>
#include <heltec.h>
#include <WiFi.h>  // Für WiFi.macAddress()

void generateDevEUI(uint8_t *devEUI) {
  Serial.println("Generate"); delay(5000);
  uint8_t mac[6];
  WiFi.macAddress(mac);  // MAC-Adresse des ESP32 auslesen

  // Die DevEUI wird generiert, indem die MAC-Adresse verwendet und angepasst wird
  devEUI[0] = 0x00;
  devEUI[1] = 0x00;
  devEUI[2] = mac[0];
  devEUI[3] = mac[1];
  devEUI[4] = mac[2];
  devEUI[5] = 0xFF;
  devEUI[6] = 0xFE;
  devEUI[7] = mac[3];
}

void printDevEUI() {
  Serial.println("Print"); delay(5000);
  uint8_t devEUI[8];

  // DevEUI generieren
  generateDevEUI(devEUI);

  // DevEUI auf der seriellen Schnittstelle ausgeben
  Serial.print("DevEUI: ");
  for (int i = 0; i < 8; i++) {
    Serial.printf("%02X", devEUI[i]);
  }
  Serial.println();
}

void setup() {
  // Serielle Kommunikation starten
  Serial.begin(115200);
  
  // Initialisiere das Heltec-Board ohne Display
  Heltec.begin(false /*DisplayEnable*/, true /*LoRaEnable*/, true /*SerialEnable*/);

  // DevEUI auslesen und anzeigen
  printDevEUI();
}

void loop() {
  // Hier könntest du andere Aktionen ausführen
}
