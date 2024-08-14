
//WICHTIG
//Der Code funktioniert vermutlich nicht richtig und Verwendet alte Bibliotheken (<hal/hal.h>)

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

// Funktion zum Auslesen der Device EUI
void printDevEUI() {
    uint8_t devEUI[8];
    os_getDevEui(devEUI);

    Serial.print("Device EUI: ");
    for (int i = 0; i < 8; i++) {
        if (devEUI[i] < 0x10) {
            Serial.print(F("0"));
        }
        Serial.print(devEUI[i], HEX);
        if (i < 7) {
            Serial.print("-");
        }
    }
    Serial.println();
}

void setup() {
    Serial.begin(115200);

    // LMIC init
    os_init();

    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    // DevEUI auslesen und anzeigen
    printDevEUI();
}

void loop() {
    // Hier könnte der Rest deines LoRaWAN-Codes sein
}

