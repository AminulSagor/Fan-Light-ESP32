#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "TP-Link_BFCA";
const char* password = "35855712";

// Output pins for fan and light
const int fanPin = 2;
const int lightPin = 4;

WebServer server(80);

// Fan handlers
void handleFanOn() {
  digitalWrite(fanPin, HIGH);
  server.send(200, "text/plain", "Fan ON");
  Serial.println("✅ Fan ON");
}

void handleFanOff() {
  digitalWrite(fanPin, LOW);
  server.send(200, "text/plain", "Fan OFF");
  Serial.println("✅ Fan OFF");
}

// Light handlers
void handleLightOn() {
  digitalWrite(lightPin, HIGH);
  server.send(200, "text/plain", "Light ON");
  Serial.println("✅ Light ON");
}

void handleLightOff() {
  digitalWrite(lightPin, LOW);
  server.send(200, "text/plain", "Light OFF");
  Serial.println("✅ Light OFF");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Booting...");

  pinMode(fanPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  digitalWrite(fanPin, LOW);
  digitalWrite(lightPin, LOW);

  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ WiFi Connected!");
  Serial.print("📶 IP address: ");
  Serial.println(WiFi.localIP());

  // Start mDNS
  if (!MDNS.begin("esp32")) {
    Serial.println("❌ Error starting mDNS");
    return;
  }
  Serial.println("🌐 mDNS responder started: http://esp32.local");

  // Advertise HTTP service
  MDNS.addService("http", "tcp", 80);

  // Register routes
  server.on("/fan/on", handleFanOn);
  server.on("/fan/off", handleFanOff);
  server.on("/light/on", handleLightOn);
  server.on("/light/off", handleLightOff);

  server.begin();
  Serial.println("🌍 HTTP server started");
}

void loop() {
  server.handleClient();
}
