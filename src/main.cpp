#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <WiFi.h>  // Für ESP32
#include <Arduino.h>
#include <PubSubClient.h>

#define BMP280_I2C_ADDRESS 0x76 // Adresse für den BMP280

// WLAN- und MQTT-Konfiguration
const char* ssid = "EL_IoT";       // Dein WLAN-SSID (Netzwerkname)
const char* password = "R1ck:and:M0rty";   // Dein WLAN-Passwort
const char* hostname = "ESP32_Grubinger";   // Hostname für den ESP32

WiFiClient esp32thomas;
PubSubClient client(esp32thomas);
Adafruit_BMP280 bmp; // Erstelle ein Objekt für den BMP280

void initWifi(){
  Serial0.begin(115200);
  WiFi.setHostname(hostname);  // Setze den Hostnamen des ESP32
  WiFi.mode(WIFI_STA);  // Setze den ESP32 in den Station-Modus (Client-Modus)
  
  // Beginne mit dem Versuch, eine Verbindung zum WLAN herzustellen
  Serial0.println("Verbindung zum WLAN wird hergestellt...");
  WiFi.begin(ssid, password);

  // Warte, bis die Verbindung hergestellt ist
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial0.println("Verbindung wird hergestellt...");
  }

  // Wenn die Verbindung erfolgreich ist:
  Serial0.println("WLAN verbunden!");
  Serial0.print("IP-Adresse: ");
  Serial0.println(WiFi.localIP());  // Ausgabe der IP-Adresse
}

void reconnect(){
  // Wenn die Verbindung zum MQTT-Broker verloren geht, wird versucht, sie wiederherzustellen
  while (!client.connected()) {
    Serial0.println("Verbindung zum MQTT-Broker wird hergestellt...");
    if (client.connect("ESP32_Grubinger","mqttuser","mqttpass")) {
      Serial0.println("Verbunden mit MQTT-Broker!");
      client.subscribe("W014/akt/LED1/thomas");
    } else {
      Serial0.print("Fehler beim Verbinden mit MQTT-Broker, rc=");
      Serial0.print(client.state());
      Serial0.println(" Warte 5 Sekunden, bevor der erneute Verbindungsversuch unternommen wird...");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial0.print("Nachricht empfangen [");
  Serial0.print(topic);
  Serial0.print("] ");
  for (int i = 0; i < length; i++) {
    Serial0.print((char)payload[i]);
  }
  Serial0.println();

  if(String(topic) == "W014/akt/LED1/thomas"){
    if((char)payload[0] == '1'){
      Serial0.println("LED EIN");
      digitalWrite(2, HIGH);
    }else{
      Serial0.println("LED AUS");
      digitalWrite(2, LOW);
    }
  }
}

void setup() {
  pinMode(2, OUTPUT);
  Serial0.begin(115200);

  client.setServer("10.245.0.204", 1883);
  client.setCallback(callback);
  
  // Initialisiere I2C mit benutzerdefinierten Pins
  Wire.begin(21, 20); // SDA: 21, SCL: 20
  if (!bmp.begin(BMP280_I2C_ADDRESS)) {
    Serial0.println("Konnte den BMP280 nicht finden. Überprüfe die Verkabelung!");
    while (1);
  }
    initWifi();
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();

  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F;

  Serial0.print("Temperatur: ");
  Serial0.print(temperature);
  Serial0.println(" °C");
  
  Serial0.print("Luftdruck: ");
  Serial0.print(pressure);
  Serial0.println(" hPa");

  // Veröffentliche die Temperatur und den Luftdruck über MQTT
  client.publish("W014/temp/thomas", String(temperature).c_str());
  client.publish("W014/press/thomas", String(pressure).c_str());
  Serial0.println("Daten veröffentlicht!");
        digitalWrite(2, HIGH);


  delay(5000); // Warte 5 Sekunden zwischen den Messungen und Veröffentlichungen
}
