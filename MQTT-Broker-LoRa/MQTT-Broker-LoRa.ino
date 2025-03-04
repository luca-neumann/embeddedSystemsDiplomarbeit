#include <WiFi.h>
#include "EmbeddedMqttBroker.h"
#include <LoRa.h>
#include <SPI.h> //Steuerung der für LoRa-benötigten Hardware

using namespace mqttBrokerName;

// WiFi-Konfiguration
const char* ssid = "LaMartinaEG";  //SSID
const char* password = "OLZFLFGXYMLPLLJN"; //Passwort

// LoRa-Konfiguration
#define ss 18 //Slave-Select
#define rst 14 //Reset
#define dio0 26 //Digital I/O 0

// MQTT-Broker-Einstellungen
uint16_t mqttPort = 8883;
MqttBroker broker(mqttPort);
String lorastring;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("MQTT Broker mit LoRa startet...");

  // WiFi initialisieren und verbinden
  Serial.print("Verbinden mit ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi verbunden");

  // LoRa initialisieren
  Serial.println("LoRa-Empfänger wird gestartet...");
  SPI.begin(5, 19, 27, 18);     //Pins für LoRa
  LoRa.setPins(ss, rst, dio0);  //Frequenz für LoRa

  if (!LoRa.begin(868E6)) {
    Serial.println("LoRa konnte nicht gestartet werden!");
    while (1);
    delay(1000);
  }
  Serial.println("LoRa gestartet");

  // MQTT-Broker starten
  broker.setMaxNumClients(9); // Max. Anzahl der Clients
  broker.startBroker();       // Broker starten
  Serial.println("MQTT-Broker gestartet!");

  // Verbindungsdaten ausgeben
  Serial.print("Verwende diese IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Verwende diesen Port: ");
  Serial.println(mqttPort);
  Serial.println("");
}

void loop() {
  // Überprüfen, ob ein LoRa-Paket empfangen wurde
  int paketlaenge = LoRa.parsePacket(); //LoRa.parsePacket() --> Wenn Paket empfangen wird, wird dessen Länge zurückgegeben
  //if-Verzweigung --> Wird dauernd duchgeloopt
  if (paketlaenge) {
    Serial.println("LoRa-Paket empfangen!");
    lora_paket();

    // Inhalt des Pakets ausgeben
    Serial.print("Inhalt des Pakets: ");
    Serial.println(lorastring);

    // LoRa-Inhalt als MQTT-Nachricht verteilen
    PublishMqttMessage publishMsg; //MQTT-Nachricht Objekt erzeigen
    publishMsg.setTopic("sensor/pm");  // Setze das Topic direkt als String --> Topic immer gleich
    publishMsg.setPayLoad(lorastring);  // Setze den empfangenen LoRa-String als Payload

    // Nachricht über den Broker veröffentlichen
    broker.publishMessage(&publishMsg);

    Serial.println("Nachricht über MQTT verteilt.");
  }
}

// Funktion, um den Inhalt des LoRa-Pakets zu lesen
void lora_paket() {
  lorastring = "";
  //Zusammensetzen des LoRa-Strings:
  while (LoRa.available()) {
    lorastring += (char)LoRa.read();
  }
}
