#include "Adafruit_VL53L0X.h"

#define Buzzer 52

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

int distance = 0;

void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }

  pinMode(Buzzer, OUTPUT);

}


void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print(" Distance (mm): "); Serial.println(measure.RangeMilliMeter);
    distance = measure.RangeMilliMeter;
  } else {
    Serial.println(" out of range ");
  }

  if(distance < 60)
  {
    digitalWrite(Buzzer, HIGH); Serial.print(" Buzzer ON");
  }
  else
  {
    digitalWrite(Buzzer, LOW); Serial.print(" Buzzer OFF");
  }

}
