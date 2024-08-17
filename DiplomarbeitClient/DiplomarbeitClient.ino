#include <WiFi.h>
#include <PubSubClient.h>

// WLAN-Zugangsdaten
const char* ssid = "LaMartinaEG";         //SSID
const char* password = "OLZFLFGXYMLPLLJN";    //Passwort

// IP des ESP32-Brokers
const char* mqtt_server = "192.168.1.29";  // Die IP-Adresse des ESP32-Brokers
const char* topic = "sensor/pm";

WiFiClient espClient;
PubSubClient client(espClient);

const int rxPin = 16;
const int txPin = 17;
HardwareSerial pmsSerial(2);
unsigned char data[32];

void setup() {
  Serial.begin(115200);
  pmsSerial.begin(9600, SERIAL_8N1, rxPin, txPin);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Verbinde mit ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi verbunden");
  Serial.println("IP-Adresse: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Verbinde mit MQTT Broker...");
    if (client.connect("ESP32Client")) {
      Serial.println("Verbunden");
    } else {
      Serial.print("Fehler, rc=");
      Serial.print(client.state());
      Serial.println(" Versuche erneut in 5 Sekunden");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (pmsSerial.available()) {
    if (pmsSerial.read() == 0x42) {  // Header-Byte 1
      if (pmsSerial.read() == 0x4d) {  // Header-Byte 2
        for (int i = 0; i < 30; i++) {
          data[i] = pmsSerial.read();
        }

        unsigned int PM1_0 = ((data[10] << 8) + data[11]);
        unsigned int PM2_5 = ((data[12] << 8) + data[13]);
        unsigned int PM10  = ((data[14] << 8) + data[15]);

        String payload = String("{\"PM1_0\":") + String(PM1_0) + 
                         String(",\"PM2_5\":") + String(PM2_5) +
                         String(",\"PM10\":") + String(PM10) + String("}");

        Serial.print("Veröffentliche Nachricht: ");
        Serial.println(payload);

        client.publish(topic, (char*) payload.c_str());

        delay(1000);  // Warte eine Sekunde, bevor du erneut liest
      }
    }
  }
}
