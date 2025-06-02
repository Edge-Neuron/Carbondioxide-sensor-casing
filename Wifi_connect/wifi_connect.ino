#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32_CO2_AP";     // Wi-Fi SSID
const char* password = "12345678";     // Wi-Fi Password

WebServer server(80);                  // Create web server on port 80

const int sensorPin = 34;              // Analog pin connected to MG-811

void setup() {
  Serial.begin(115200);

  // Start Wi-Fi Access Point
  WiFi.softAP(ssid, password);
  Serial.print("Access Point started. Connect to: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Serve root page
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", getHTMLPage());
  });

  // Serve CO2 data endpoint
  server.on("/co2", HTTP_GET, []() {
    int raw = analogRead(sensorPin);
    float voltage = raw * (3.3 / 4095.0);          // Convert ADC reading to voltage (0 - 3.3V)
    
    // Simple estimation for MG-811 (can be calibrated)
    float ppm = (voltage - 0.4) * 1000.0;          // Rough ppm value
    float percent = (ppm / 10000.0) * 100.0;       // Scale to 0-100%

    // Clamp percentage
    percent = constrain(percent, 0.0, 100.0);

    server.send(200, "text/plain", String(percent, 2));
  });

  server.begin();
  Serial.println("HTTP server started.");
}

void loop() {
  server.handleClient();    // Handle incoming client requests
}

// Function to return HTML page with JavaScript for live data
String getHTMLPage() {
  return R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>CO₂ Sensor Monitor</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; padding-top: 50px; background-color: #f5f5f5; }
    .value { font-size: 48px; color: #2e8b57; margin: 20px 0; }
    .label { font-size: 24px; color: #333; }
  </style>
</head>
<body>
  <h1>Real-Time CO₂ Level</h1>
  <div class="label">Concentration:</div>
  <div class="value" id="co2Value">-- %</div>

  <script>
    setInterval(() => {
      fetch("/co2")
        .then(response => response.text())
        .then(data => {
          document.getElementById("co2Value").innerText = data + " %";
        })
        .catch(err => {
          console.error("Failed to fetch CO₂ value:", err);
        });
    }, 1000);
  </script>
</body>
</html>
)rawliteral";
}
