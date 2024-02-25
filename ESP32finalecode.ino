#ifdef ESP32
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#endif

#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

// Set to true to define Relay as Normally Open (NO)
#define RELAY_NO    true
#define ONE_WIRE_BUS 4
// Set number of relays
#define NUM_RELAYS  1

// Assign each GPIO to a relay
int relayGPIOs[NUM_RELAYS] = {5};
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
String messageStatic1 = "Battery Temperature is";

// Variables to store temperature values
String temperatureF = "";
String temperatureC = "";
unsigned long lastTime = 0;
unsigned long timerDelay = 2000; // Refresh every 2 seconds

// Replace with your network credentials
const char* ssid = "Nisarg";
const char* password = "Nkp@2005";

const char* PARAM_INPUT_1 = "relay";
const char* PARAM_INPUT_2 = "state";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Function to read temperature in Celsius
String readDSTemperatureC() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  if (tempC == -127.00) {
    Serial.println("Failed to read from DS18B20 sensor");
    return "--";
  } else {
    Serial.print("Temperature Celsius: ");
    Serial.println(tempC);
    return String(tempC);
  }
}

// Function to read temperature in Fahrenheit
String readDSTemperatureF() {
  sensors.requestTemperatures();
  float tempF = sensors.getTempFByIndex(0);

  if (int(tempF) == -196) {
    Serial.println("Failed to read from DS18B20 sensor");
    return "--";
  } else {
    Serial.print("Temperature Fahrenheit: ");
    Serial.println(tempF);
    return String(tempF);
  }
}

// HTML content for the web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center; background-color: #ADD8E6;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px; background-color: rgba(255, 255, 255, 0.5);}
  </style>
  <script>
    setInterval(function() {
      // Update temperature values and relay status every 2 seconds using AJAX
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("temperaturec").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "/temperaturec", true);
      xhttp.send();

      var xhttp2 = new XMLHttpRequest();
      xhttp2.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("temperaturef").innerHTML = this.responseText;
        }
      };
      xhttp2.open("GET", "/temperaturef", true);
      xhttp2.send();

      var xhttp3 = new XMLHttpRequest();
      xhttp3.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("relaystatus").innerHTML = this.responseText;
        }
      };
      xhttp3.open("GET", "/relaystatus", true);
      xhttp3.send();
    }, 2000); // Update every 2 seconds
  </script>
</head>
<body>
  <h2>EV Battery Blast Protection Monitoring System </h2>
  <h3>DC Motor</h3>
  <p>Motor Status: <span id="relaystatus">%RELAYSTATUS%</span></p>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
    <span class="ds-labels">Temperature Celsius</span>
    <span id="temperaturec">%TEMPERATUREC%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
    <span class="ds-labels">Temperature Fahrenheit</span>
    <span id="temperaturef">%TEMPERATUREF%</span>
    <sup class="units">&deg;F</sup>
  </p>
</body>
</html>
)rawliteral";

// Function to get relay status (ON or OFF)
String relayState(int numRelay) {
  if (RELAY_NO) {
    if (digitalRead(relayGPIOs[numRelay - 1])) {
      return "ON";
    } else {
      return "OFF";
    }
  } else {
    if (digitalRead(relayGPIOs[numRelay - 1])) {
      return "OFF";
    } else {
      return "ON";
    }
  }
  return "ON";
}

// Custom processor function to replace placeholders in HTML with actual values
String processor(const String& var) {
  if (var == "RELAYSTATUS") {
    return relayState(1);
  } else if (var == "RELAYGPIO") {
    return String(relayGPIOs[0]);
  } else if (var == "TEMPERATUREC") {
    return temperatureC;
  } else if (var == "TEMPERATUREF") {
    return temperatureF;
  }
  return String();
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  sensors.begin();

  temperatureC = readDSTemperatureC();
  temperatureF = readDSTemperatureF();

  for (int i = 0; i < NUM_RELAYS; i++) {
    pinMode(relayGPIOs[i], OUTPUT);
    if (RELAY_NO) {
      digitalWrite(relayGPIOs[i], HIGH);
    } else {
      digitalWrite(relayGPIOs[i], LOW);
    }
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  // Setup routes for different HTTP requests
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/relaystatus", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", relayState(1).c_str());
  });

  server.on("/temperaturec", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", temperatureC.c_str());
  });

  server.on("/temperaturef", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", temperatureF.c_str());
  });

  server.begin();
  lcd.init();
  lcd.backlight();
  pinMode(5, OUTPUT);
}

void loop() {
  sensors.requestTemperatures();

  // Update temperature values every 2 seconds
  if ((millis() - lastTime) > timerDelay) {
    temperatureC = readDSTemperatureC();
    temperatureF = readDSTemperatureF();
    lastTime = millis();
  }

  lcd.setCursor(0, 0);
  lcd.print(messageStatic1);
  lcd.setCursor(0, 1);
  lcd.print(temperatureC);

  // Control relay based on temperature
  if (temperatureC.toFloat() >= 32.00) {
    digitalWrite(5, LOW); // Turn off relay
  } else {
    digitalWrite(5, HIGH); // Turn on relay
  }
}
