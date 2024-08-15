#include <WiFi.h>
#include "heltec.h"

// WiFi-Konfigurationsdaten
//WICHTIG: WLAN muss auf 2,4 GHz laufen
const char* ssid = "XX";         //SSID
const char* password = "XX";    //Passwort

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

}

void loop() {
  // Der loop() bleibt leer, da alle relevanten Aktionen im setup() ausgeführt wurden
}
