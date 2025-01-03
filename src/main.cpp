
#include <WiFi.h>  // Für ESP32
#include <Arduino.h>
#include <PubSubClient.h>

// Definiere den DHT11-Typ
#define DHTTYPE DHT11
#define DHTPIN 14  // GPIO-Pin, an den der DHT11-Datenpin angeschlossen ist

// WLAN und MQTT Konfiguration
const char* ssid = "****";       // Dein WLAN-SSID (Netzwerkname)
const char* password = "*******";   // Dein WLAN-Passwort
const char* hostname = "Thomas_ESP";  // Dein Hostname
const char* mqtt_server = "192.168.50.223";  // MQTT-Broker IP-Adresse
const int mqtt_port = 1883;
const char* mqtt_user = "****";   // MQTT-Benutzername
const char* mqtt_pass = "******";   // MQTT-Passwort

// MQTT-Client
WiFiClient esp32thomas;
PubSubClient client(esp32thomas);

// DHT-Sensor Objekt
// Funktionen zur Initialisierung von WLAN und MQTT
void initWifi() {
  Serial.begin(115200);
  WiFi.setHostname(hostname);
  WiFi.mode(WIFI_STA);
  Serial.println("Verbindung zum WLAN wird hergestellt...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Verbindung wird hergestellt...");
  }

  Serial.println("WLAN verbunden!");
  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Verbindung zum MQTT-Broker wird hergestellt...");
    if (client.connect("ESP32_Grubinger", mqtt_user, mqtt_pass)) {
      Serial.println("Verbunden mit MQTT-Broker!");
    } else {
      Serial.print("Fehler beim Verbinden mit MQTT-Broker, rc=");
      Serial.print(client.state());
      Serial.println(" Warte 5 Sekunden...");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Nachricht empfangen [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  // WLAN- und MQTT-Verbindung initialisieren
  initWifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // Initialisiere den DHT-Sensor
  reconnect();
}

void loop() {
  // Stelle sicher, dass der Client verbunden bleibt
  if (!client.connected()) {
    reconnect();
  }
  client.loop();



  client.publish("thomas", "Hallo Welt!");

  // Warte 5 Sekunden, bevor eine neue Messung durchgeführt wird
  delay(5000);
}
