#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

void setup(void) {
  Serial.begin(9600);

  bool isInit = ads.begin();

  if (isInit) {
    Serial.println("Initialize ADS.");
  } else {
    Serial.println("Failed to initialize ADS.");
  }
  Serial.println("-----");
}

void loop(void) {
  int16_t adc0, adc1;

  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);

  Serial.print(adc0);
  Serial.print("\t");
  Serial.println(adc1);

  delay(500);
}