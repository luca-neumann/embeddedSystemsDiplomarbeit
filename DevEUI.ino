#include <WiFi.h>
#include "heltec.h"

// WiFi-Konfigurationsdaten
//WICHTIG: WLAN muss auf 2,4 GHz laufen
const char* ssid = "LaMartinaEG";         //SSID
const char* password = "OLZFLFGXYMLPLLJN";    //Passwort

void setup() {
  // Initialisieren der serielle Schnittstelle für Debugging
  Serial.begin(115200);

  // Initialisieren des Heltec-Boards
  Heltec.begin(false /*Display*/, false /*LoRa*/, true /*Serial*/);

  // WiFi-Verbindung herstellen
  WiFi.begin(ssid, password);
  Serial.print("Verbindung zu WiFi wird hergestellt");

  // Warten, bis die Verbindung hergestellt ist
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // WiFi-Verbindung hergestellt
  Serial.println("");
  Serial.println("Mit WiFi verbunden!");
  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.localIP());

  //MAC-Ausgeben:
  Serial.print("MAC:");
  Serial.println(WiFi.macAddress());

  printDevEUI();
  }

void generateDevEUI(uint8_t *devEUI) {
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
  Serial.println("DevEUI:"); delay(1000);
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

void loop() {
  // Der loop() bleibt leer, da alle relevanten Aktionen im setup() ausgeführt wurden
}

