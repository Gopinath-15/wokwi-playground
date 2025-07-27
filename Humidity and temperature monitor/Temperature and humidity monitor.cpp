#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const int relayPin = 3;
const float tempThreshold = 30.0; // Adjust as needed

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(relayPin, OUTPUT);
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("DHT22 Monitor");
  display.display();
  delay(2000);
}

void loop() {
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  display.clearDisplay();
  display.setCursor(0, 0);
  
  if (isnan(temp) || isnan(humidity)) {
    display.println("Sensor Error");
    Serial.println("DHT22 read error");
  } else {
    display.print("Temp: ");
    display.print(temp);
    display.println(" C");

    display.print("Humidity: ");
    display.print(humidity);
    display.println(" %");

    if (temp > tempThreshold) {
      digitalWrite(relayPin, HIGH);
      display.println("Relay: ON");
    } else {
      digitalWrite(relayPin, LOW);
      display.println("Relay: OFF");
    }
  }

  display.display();
  delay(2000);
}
