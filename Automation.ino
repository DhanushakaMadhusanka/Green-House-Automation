#include <WiFi.h>
#include "time.h"

// Pins for sensors and valves
const int temperatureSensorPin = 34;
const int humiditySensorPin = 35;
const int valve1Pin = 26;
const int valve2Pin = 27;
const int valve3Pin = 25;
const int motorPin = 2; // CHANGE TO 14

// Testing values (DELETE THIS OR UNCOMMENT)
const int CONSTANT_HUMIDITY = 80; // Constant humidity value

// WiFi credentials
const char* ssid = "DIALOG-4G_DHANUSH";
const char* password = "Dhanu@98";

// Days for valve 3 to open (0 = Sunday, 1 = Monday, ..., 6 = Saturday)
const int valve3Days[] = {1, 3, 5}; // Example: Monday, Wednesday, Friday

void setup() {
  Serial.begin(115200);
  
  // Initialize pins
  pinMode(temperatureSensorPin, INPUT);
  pinMode(humiditySensorPin, INPUT);
  pinMode(valve1Pin, OUTPUT);
  pinMode(valve2Pin, OUTPUT);
  pinMode(valve3Pin, OUTPUT);
  pinMode(motorPin, OUTPUT);

  // Initialize WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();

  // Initialize time
  if (WiFi.status() == WL_CONNECTED) {
    configTime(5 * 3600, 30 * 60, "pool.ntp.org"); // Set time from NTP server

    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      Serial.println(&timeinfo, "Time synchronized: %A, %B %d %Y %H:%M:%S");
    } else {
      Serial.println("Failed to obtain time from NTP server");
    }
  } else {
    Serial.println("WiFi not connected. Using internal clock.");
  }
}

bool isValve3Day(int dayOfWeek) {
  for (int i = 0; i < sizeof(valve3Days) / sizeof(valve3Days[0]); i++) {
    if (valve3Days[i] == dayOfWeek) {
      return true;
    }
  }
  return false;
}

void loop() {
  // Get current time
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    delay(1000); // Retry after a delay
    return;
  }
  
  int hour = timeinfo.tm_hour;
  int minute = timeinfo.tm_min;
  int dayOfWeek = timeinfo.tm_wday;

  // Read sensor values
  int temperature = analogRead(temperatureSensorPin);
  int humidity = analogRead(humiditySensorPin);

  Serial.print("Temperature Pin: ");
  Serial.println(temperature);
  Serial.print("Humidity Pin: ");
  Serial.println(humidity);

  // Convert analog readings to actual values (customize based on your sensors)
  temperature = map(temperature, 0, 4095, -40, 125); // Assuming temperature range -40 to 125°C
  //humidity = map(humidity, 0, 4095, 0, 100); // Assuming humidity range 0 to 100%

  humidity = CONSTANT_HUMIDITY; // (DELETE THIS) Set current humidity to the constant value


  Serial.print("Temperature : ");
  Serial.println(temperature);
  Serial.print("Humidity : ");
  Serial.println(humidity);

  // Control valve1
  if (temperature > 40 || humidity < 70) {
    digitalWrite(valve1Pin, HIGH);
    digitalWrite(motorPin, HIGH);
  } else {
    digitalWrite(valve1Pin, LOW);
  }

  // Control valve2 and valve3
  if ((hour == 16 && minute < 5 && isValve3Day(dayOfWeek)) || (hour == 16 && minute < 5)) {     // water supply 
    digitalWrite(valve2Pin, HIGH);
    digitalWrite(motorPin, HIGH);

    if (hour == 16 && minute < 5 && isValve3Day(dayOfWeek)) {   // fertilizer supply
      digitalWrite(valve3Pin, HIGH);
    } else {
      digitalWrite(valve3Pin, LOW);
    }

  } else {
    digitalWrite(valve2Pin, LOW);
    digitalWrite(valve3Pin, LOW);
  }
  
  // Stop motor if all valves are closed
  if (digitalRead(valve1Pin) == LOW && digitalRead(valve2Pin) == LOW && digitalRead(valve3Pin) == LOW) {
    digitalWrite(motorPin, LOW);
  }

  // Small delay to avoid excessive processing
  delay(5000);
}
