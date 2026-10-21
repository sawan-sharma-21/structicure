#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_PIN 15

const float DATUM_C = 0.0;
OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature sensors(&oneWire);

unsigned long previousMs = 0;
float previousTempC = 0;
double maturityCelsiusHours = 0;
bool havePreviousReading = false;

void setup() {
  Serial.begin(115200);
  sensors.begin();
  Serial.println("elapsed_seconds,temp_C,maturity_C_h");
}

void loop() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  if (tempC < -55 || tempC > 125) {
    Serial.println("Sensor reading error; check the wiring.");
    delay(2000);
    return;
  }

  unsigned long now = millis();

  if (havePreviousReading) {
    float averageTempC = (previousTempC + tempC) / 2.0;
    float elapsedHours = (now - previousMs) / 3600000.0;
    maturityCelsiusHours += max(0.0f, averageTempC - DATUM_C) * elapsedHours;
  }

  previousMs = now;
  previousTempC = tempC;
  havePreviousReading = true;

Serial.print(millis() / 1000.0, 1);
Serial.print(',');
Serial.print(tempC, 2);
Serial.print(',');
Serial.println(maturityCelsiusHours, 4);

  delay(2000);
}
