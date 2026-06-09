#include <Adafruit_ADS1X15.h>

#define PORT_SPEED 115200

#define INIT_DELAY 5000
#define POLLING_INTERVAL 50

#define MODULE_1_ADDRESS 0x48
#define CHANNEL_1 0
#define CHANNEL_2 1
#define CHANNEL_3 2
#define CHANNEL_4 3

Adafruit_ADS1115 converterModule;

const byte FRAME_LEN = 22;  // byte
byte frame[FRAME_LEN];

uint16_t sensValue1;
uint16_t sensValue2;
uint16_t sensValue3;
uint16_t sensValue4;

uint32_t lastSensPollTime;
uint32_t currentTime;

void setup() {
  lastSensPollTime = millis();

  Serial.begin(PORT_SPEED);

  converterModule.setDataRate(RATE_ADS1115_250SPS);  // 250 samples per second - 860 max, see <Adafruit_ADS1X15.h>
  converterModule.begin();

  delay(INIT_DELAY);
}

void loop() {
  currentTime = millis();

  if (currentTime - lastSensPollTime >= POLLING_INTERVAL) {
    lastSensPollTime = currentTime;

    sensValue1 = converterModule.readADC_SingleEnded(CHANNEL_1);
    sensValue2 = converterModule.readADC_SingleEnded(CHANNEL_2);
    sensValue3 = 0;
    sensValue4 = 0;

    // Init bytes
    frame[0] = 0;
    frame[1] = 0;
    frame[2] = 0;

    frame[3] = MODULE_1_ADDRESS;

    // Control bytes
    frame[4] = 1;
    frame[7] = 2;
    frame[10] = 3;
    frame[13] = 4;
    frame[16] = 5;
    frame[19] = 6;

    frame[9] = lastSensPollTime & 0xff;
    frame[8] = (lastSensPollTime >> 8) & 0xff;
    frame[6] = (lastSensPollTime >> 16) & 0xff;
    frame[5] = (lastSensPollTime >> 24) & 0xff;

    frame[11] = (sensValue1 >> 8) & 0xff;
    frame[12] = sensValue1 & 0xff;
    frame[14] = (sensValue2 >> 8) & 0xff;
    frame[15] = sensValue2 & 0xff;
    frame[17] = (sensValue3 >> 8) & 0xff;
    frame[18] = sensValue3 & 0xff;
    frame[20] = (sensValue4 >> 8) & 0xff;
    frame[21] = sensValue4 & 0xff;

    Serial.write(frame, FRAME_LEN);
  }
}