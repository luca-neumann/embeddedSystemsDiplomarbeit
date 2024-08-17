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
  Serial.println("Mit WiFi verbunden");
  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.localIP());

  //MAC-Ausgeben:
  Serial.print("MAC:");
  Serial.println(WiFi.macAddress());

  printDevEUI();
  }

void printDevEUI() {
  uint8_t mac[6];
  uint8_t devEUI[8];

  // MAC-Adresse abrufen
  WiFi.macAddress(mac);

  // Umwandlung der MAC-Adresse in EUI-64
  devEUI[0] = mac[0] ^ 0x02;  // Universal/Local-Bit umkehren (optional)
  devEUI[1] = mac[1];
  devEUI[2] = mac[2];
  devEUI[3] = 0xFF;
  devEUI[4] = 0xFE;
  devEUI[5] = mac[3];
  devEUI[6] = mac[4];
  devEUI[7] = mac[5];

  // Ausgabe der devEUI
  Serial.print("Die devEUI ist: ");
  for (int i = 0; i < 8; i++) {
    if (i != 0) Serial.print(":");
    Serial.print(devEUI[i], HEX);
  }
  Serial.println();
}

void loop() {
  // Der loop() bleibt leer, da alle relevanten Aktionen im setup() ausgeführt wurden
}
