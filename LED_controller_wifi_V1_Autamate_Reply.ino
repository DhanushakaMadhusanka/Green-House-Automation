#include <WiFi.h>
#include <ThingESP.h>

ThingESP32 thing("Dhanushka98150", "Bulb", "Esp 32");

// Define pins for outputs and inputs
int OUTPUT1 = 23;
int OUTPUT2 = 22;
int OUTPUT3 = 21;
int OUTPUT4 = 19;
int ANALOG_OUTPUT1 = 25; // Analog output pin
int ANALOG_OUTPUT2 = 26; // Analog output pin
int INPUT1 = 34; // Analog input pin
int INPUT2 = 35; // Analog input pin
int INPUT3 = 32; // Analog input pin
int INPUT4 = 33; // Analog input pin

void setup()
{
  Serial.begin(115200);
  pinMode(OUTPUT1, OUTPUT);
  pinMode(OUTPUT2, OUTPUT);
  pinMode(OUTPUT3, OUTPUT);
  pinMode(OUTPUT4, OUTPUT);
  pinMode(ANALOG_OUTPUT1, OUTPUT);
  pinMode(ANALOG_OUTPUT2, OUTPUT);

  // Set initial state for outputs to LOW (off)
  digitalWrite(OUTPUT1, LOW);
  digitalWrite(OUTPUT2, LOW);
  digitalWrite(OUTPUT3, LOW);
  digitalWrite(OUTPUT4, LOW);
  analogWrite(ANALOG_OUTPUT1, 0);
  analogWrite(ANALOG_OUTPUT2, 0);

  thing.SetWiFi("DIALOG-4G_DHANUSH", "Dhanu@98");
  thing.initDevice();

  // Send available queries and pinout details to client at startup
  String queries = getAvailableQueries();
  Serial.println(queries); // Using Serial for sending available queries

  String pinoutDetails = getPinoutDetails();
  Serial.println(pinoutDetails); // Using Serial for sending pinout details
}

String getAvailableQueries() {
  String queries = "*Available queries:*\n\n";
  queries += "output1 on\n";
  queries += "output1 off\n";
  queries += "output2 on\n";
  queries += "output2 off\n";
  queries += "output3 on\n";
  queries += "output3 off\n";
  queries += "output4 on\n";
  queries += "output4 off\n";
  queries += "analogoutput1 set <value>\n";
  queries += "analogoutput2 set <value>\n";
  queries += "input1 status\n";
  queries += "input2 status\n";
  queries += "input3 status\n";
  queries += "input4 status\n";
  queries += "details\n";
  queries += "auto reply on\n";
  queries += "auto reply off\n";
  return queries;
}

String getPinoutDetails() {
  String pinoutDetails = "*Pinout details:*\n\n";
  pinoutDetails += "OUTPUT1: " + String(OUTPUT1) + " - " + (digitalRead(OUTPUT1) ? "HIGH" : "LOW") + "\n";
  pinoutDetails += "OUTPUT2: " + String(OUTPUT2) + " - " + (digitalRead(OUTPUT2) ? "HIGH" : "LOW") + "\n";
  pinoutDetails += "OUTPUT3: " + String(OUTPUT3) + " - " + (digitalRead(OUTPUT3) ? "HIGH" : "LOW") + "\n";
  pinoutDetails += "OUTPUT4: " + String(OUTPUT4) + " - " + (digitalRead(OUTPUT4) ? "HIGH" : "LOW") + "\n";
  pinoutDetails += "ANALOG_OUTPUT1: " + String(ANALOG_OUTPUT1) + " - " + String(analogRead(ANALOG_OUTPUT1)) + "\n";
  pinoutDetails += "ANALOG_OUTPUT2: " + String(ANALOG_OUTPUT2) + " - " + String(analogRead(ANALOG_OUTPUT2)) + "\n";
  pinoutDetails += "INPUT1: " + String(INPUT1) + " - " + String(analogRead(INPUT1)) + "\n";
  pinoutDetails += "INPUT2: " + String(INPUT2) + " - " + String(analogRead(INPUT2)) + "\n";
  pinoutDetails += "INPUT3: " + String(INPUT3) + " - " + String(analogRead(INPUT3)) + "\n";
  pinoutDetails += "INPUT4: " + String(INPUT4) + " - " + String(analogRead(INPUT4)) + "\n";
  return pinoutDetails;
}

String HandleResponse(String query)
{
  if (query == "output1 on") {
    digitalWrite(OUTPUT1, HIGH);
    return "Done: OUTPUT1 Turned ON";
  } else if (query == "output1 off") {
    digitalWrite(OUTPUT1, LOW);
    return "Done: OUTPUT1 Turned OFF";
  } else if (query == "output2 on") {
    digitalWrite(OUTPUT2, HIGH);
    return "Done: OUTPUT2 Turned ON";
  } else if (query == "output2 off") {
    digitalWrite(OUTPUT2, LOW);
    return "Done: OUTPUT2 Turned OFF";
  } else if (query == "output3 on") {
    digitalWrite(OUTPUT3, HIGH);
    return "Done: OUTPUT3 Turned ON";
  } else if (query == "output3 off") {
    digitalWrite(OUTPUT3, LOW);
    return "Done: OUTPUT3 Turned OFF";
  } else if (query == "output4 on") {
    digitalWrite(OUTPUT4, HIGH);
    return "Done: OUTPUT4 Turned ON";
  } else if (query == "output4 off") {
    digitalWrite(OUTPUT4, LOW);
    return "Done: OUTPUT4 Turned OFF";
  } else if (query.startsWith("analogoutput1 set")) {
    int value = query.substring(17).toInt();
    analogWrite(ANALOG_OUTPUT1, value);
    return "Done: ANALOG_OUTPUT1 set to " + String(value);
  } else if (query.startsWith("analogoutput2 set")) {
    int value = query.substring(17).toInt();
    analogWrite(ANALOG_OUTPUT2, value);
    return "Done: ANALOG_OUTPUT2 set to " + String(value);
  } else if (query == "input1 status") {
    int value = analogRead(INPUT1);
    return "INPUT1 value: " + String(value);
  } else if (query == "input2 status") {
    int value = analogRead(INPUT2);
    return "INPUT2 value: " + String(value);
  } else if (query == "input3 status") {
    int value = analogRead(INPUT3);
    return "INPUT3 value: " + String(value);
  } else if (query == "input4 status") {
    int value = analogRead(INPUT4);
    return "INPUT4 value: " + String(value);
  } else if (query == "details") {
    return getPinoutDetails();
  } else if (query == "queries") {
    return getAvailableQueries();
  } else if (query == "auto reply on") {
    return "Auto reply functionality not implemented.";
  } else if (query == "auto reply off") {
    return "Auto reply functionality not implemented.";
  } else {
    return "Your query was invalid.";
  }
}

void loop()
{
  thing.Handle();
}
