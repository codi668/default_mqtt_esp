# ESP32 MQTT Handle - Grundstruktur

Dies ist eine Grundstruktur für die Integration eines ESP32 mit einem MQTT-Broker. Der Code verbindet sich mit einem WLAN, stellt eine MQTT-Verbindung her und ermöglicht die Veröffentlichung und den Empfang von Nachrichten über MQTT-Themen.

## Funktionen
- Verbindung zum WLAN herstellen.
- Verbindung zu einem MQTT-Broker aufbauen.
- Empfang und Verarbeitung von MQTT-Nachrichten.
- Veröffentlichung von Nachrichten an ein MQTT-Thema.

## Verwendete Bibliotheken
- `WiFi.h`: Ermöglicht die WLAN-Verbindung auf dem ESP32.
- `Arduino.h`: Grundlegende Arduino-Funktionen.
- `PubSubClient.h`: MQTT-Client-Bibliothek für die Kommunikation mit MQTT-Brokern.

## Hardwareanforderungen
- **ESP32**
- Optional: **DHT11-Sensor** (im Code erwähnt, jedoch nicht vollständig implementiert).

## Konfiguration
Im Code sind mehrere Konfigurationsparameter definiert. Diese sollten an deine Anforderungen angepasst werden:

### WLAN-Einstellungen
```cpp
const char* ssid = "speeed";       // Dein WLAN-SSID (Netzwerkname)
const char* password = "The_Integtal_is_Real";   // Dein WLAN-Passwort
const char* hostname = "Thomas_ESP";  // Dein Hostname
