#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define SERIAL_BAUD 115200

// Replace with your network credentials
const char* ssid = "YOUR MODEM SSID";
const char* password = "ENTER YOUR WIFI PASSWORD HERE";

// Desired static IP configuration
IPAddress staticIP(192, 168, 0, 208);    // Replace with your preferred static IP
IPAddress gateway(192, 168, 0, 1);       // Replace with your gateway (usually your router's IP)
IPAddress subnet(255, 255, 255, 0);      // Subnet mask

// Create an instance of the web server
ESP8266WebServer server(80);

// Max connection attempts before reboot
int maxAttempts = 5;

void setup() {
  // Start serial communication
  Serial.begin(SERIAL_BAUD);

  // Attempt to connect to WiFi with static IP
  connectToWiFi();

  // Define web server routes for motor control
  server.on("/", handleRoot);  // Webpage route

  server.on("/M1_FORWARD", []() {
    Serial.println("M1_FORWARD");
    server.send(200, "text/plain", "Motor 1 Forward");
  });
  server.on("/M1_STOP", []() {
    Serial.println("M1_STOP");
    server.send(200, "text/plain", "Motor 1 Stop");
  });
  server.on("/M2_FORWARD", []() {
    Serial.println("M2_FORWARD");
    server.send(200, "text/plain", "Motor 2 Forward");
  });
  server.on("/M2_STOP", []() {
    Serial.println("M2_STOP");
    server.send(200, "text/plain", "Motor 2 Stop");
  });
  server.on("/M3_FORWARD", []() {
    Serial.println("M3_FORWARD");
    server.send(200, "text/plain", "Motor 3 Forward");
  });
  server.on("/M3_STOP", []() {
    Serial.println("M3_STOP");
    server.send(200, "text/plain", "Motor 3 Stop");
  });
  server.on("/M4_FORWARD", []() {
    Serial.println("M4_FORWARD");
    server.send(200, "text/plain", "Motor 4 Forward");
  });
  server.on("/M4_STOP", []() {
    Serial.println("M4_STOP");
    server.send(200, "text/plain", "Motor 4 Stop");
  });

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  // Handle client requests
  server.handleClient();

  // Check if the ESP is still connected to WiFi
  checkWiFi();
}

// Function to connect to WiFi with increased timeout and static IP
void connectToWiFi() {
  WiFi.config(staticIP, gateway, subnet); // Set static IP configuration
  WiFi.begin(ssid, password);
  
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 30000) { // Timeout of 30 seconds
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi.");
  } else {
    Serial.println("WiFi connected.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
}

// Function to check and reconnect to WiFi if disconnected
void checkWiFi() {
  static int connectionAttempts = 0;

  if (WiFi.status() != WL_CONNECTED) {
    connectionAttempts++;
    Serial.println("WiFi disconnected, attempting to reconnect...");
    
    WiFi.begin(ssid, password);
    unsigned long startAttemptTime = millis();
    
    // Try to reconnect to WiFi
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) { // 10-second timeout per attempt
      delay(500);
      Serial.print(".");
    }

    // If still not connected after max attempts, reboot the ESP
    if (WiFi.status() != WL_CONNECTED && connectionAttempts >= maxAttempts) {
      Serial.println("Failed to reconnect to WiFi. Rebooting...");
      ESP.restart();
    } else if (WiFi.status() == WL_CONNECTED) {
      connectionAttempts = 0; // Reset attempts count on successful reconnect
      Serial.println("WiFi reconnected.");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
    }
  }
}

// Function to handle the root webpage
void handleRoot() {
  String html = "<html><body>"
                "<h1>Motor Control</h1>"
                "<p>Motor 1: <button onclick=\"sendCommand('M1_FORWARD')\">Forward</button>"
                "<button onclick=\"sendCommand('M1_STOP')\">Stop</button></p>"
                "<p>Motor 2: <button onclick=\"sendCommand('M2_FORWARD')\">Forward</button>"
                "<button onclick=\"sendCommand('M2_STOP')\">Stop</button></p>"
                "<p>Motor 3: <button onclick=\"sendCommand('M3_FORWARD')\">Forward</button>"
                "<button onclick=\"sendCommand('M3_STOP')\">Stop</button></p>"
                "<p>Motor 4: <button onclick=\"sendCommand('M4_FORWARD')\">Forward</button>"
                "<button onclick=\"sendCommand('M4_STOP')\">Stop</button></p>"
                "<script>"
                "function sendCommand(command) {"
                "  var xhttp = new XMLHttpRequest();"
                "  xhttp.open('GET', '/' + command, true);"
                "  xhttp.send();"
                "}"
                "</script>"
                "</body></html>";
  server.send(200, "text/html", html);
}
