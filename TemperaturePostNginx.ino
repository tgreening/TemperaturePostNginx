// Import required libraries
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <CircularBuffer.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// WiFi parameters
const char* ssid = "";
const char* password = "";
const String probe_name = "house";
const int delay_minutes = 15;
const String server = "";
float lastReading = 0;
float tolerance = 1.0F;
float desiredTemperature = 68;
HTTPClient http;
unsigned long readingTime = millis() + 60000UL;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(D4);
OneWire oneWire2(D3);

DallasTemperature sensors(&oneWire);
DallasTemperature sensor2(&oneWire2);

void setup(void)
{
  // Start Serial
  Serial.begin(115200);
  Wire.begin(D5, D6);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.display();

  // Connect to WiFi
  WiFi.hostname("basementtemp");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  sensors.begin();
  Serial.println("");
  Serial.println("WiFi connected");

  // Print the IP address
  Serial.println(WiFi.localIP());
  display.setTextSize(2);
  display.setTextColor(WHITE);
}

void loop() {

  if ((long)(millis() - readingTime) >= 0) {
    readingTime += 10000UL;
    lastReading = toFahrenheit(sensors.getTempCByIndex(0));
    if (abs(lastReading - desiredTemperature) > tolerance) {
       
    }
  }
sensors.requestTemperatures(); // Send the command to get temperatures
  sensor2.requestTemperatures(); // Send the command to get temperatures
  display.setCursor(0, 0);
  display.clearDisplay();
  display.print("I: ");
  display.println(toFahrenheit(sensors.getTempCByIndex(0)));
  display.setCursor(0, 30);
  display.print("E: ");
  display.println(toFahrenheit(sensor2.getTempCByIndex(0)));
  display.setCursor(0, 50);
  display.println("De: 68");
  display.display();
}

float toFahrenheit(float celsius) {
  return (celsius * 9 / 5) + 32;
}

