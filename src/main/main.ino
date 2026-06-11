#include <Adafruit_ADS1X15.h>

#define PORT_SPEED 115200
#define INIT_DELAY 5000

// number of points for averaging
#define ITEMS_NUM_TO_SEND 4
#define POLL_INTERVAL 25

#define MODULE_1_ADDRESS 0x48
#define CHANNEL_1 0
#define CHANNEL_2 1
#define CHANNEL_3 2
#define CHANNEL_4 3

Adafruit_ADS1115 converterModule;

const uint8_t FRAME_LEN = 22;
uint8_t frame[FRAME_LEN];

uint32_t channel1ValueSum = 0;
uint32_t channel2ValueSum = 0;
uint32_t channel3ValueSum = 0;

uint16_t channel1AvgValue = 0;
uint16_t channel2AvgValue = 0;
uint16_t channel3AvgValue = 0;
uint16_t channel4AvgValue = 0;

uint32_t lastSensPollTime;
uint32_t currentTime;

uint8_t itemsNum = 0;

void setup() {
  lastSensPollTime = millis();

  Serial.begin(PORT_SPEED);

  converterModule.begin();
  converterModule.setDataRate(RATE_ADS1115_250SPS);

  delay(INIT_DELAY);
}

void loop() {
  currentTime = millis();

  if (currentTime - lastSensPollTime >= POLL_INTERVAL) {
    lastSensPollTime = currentTime;

    channel1ValueSum += converterModule.readADC_SingleEnded(CHANNEL_1);
    channel2ValueSum += converterModule.readADC_SingleEnded(CHANNEL_2);
    channel3ValueSum += converterModule.readADC_SingleEnded(CHANNEL_3);

    itemsNum += 1;

    if (itemsNum >= ITEMS_NUM_TO_SEND) {
      channel1AvgValue = channel1ValueSum / ITEMS_NUM_TO_SEND;
      channel2AvgValue = channel2ValueSum / ITEMS_NUM_TO_SEND;
      channel3AvgValue = channel3ValueSum / ITEMS_NUM_TO_SEND;
      channel4AvgValue = 0xffff; // channel 4 not used, but retained for future implementations
      
      channel1ValueSum = 0;
      channel2ValueSum = 0;
      channel3ValueSum = 0;
      
      itemsNum = 0;
      
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
      
      frame[11] = (channel1AvgValue >> 8) & 0xff;
      frame[12] = channel1AvgValue & 0xff;
      frame[14] = (channel2AvgValue >> 8) & 0xff;
      frame[15] = channel2AvgValue & 0xff;
      frame[17] = (channel3AvgValue >> 8) & 0xff;
      frame[18] = channel3AvgValue & 0xff;
      frame[20] = (channel4AvgValue >> 8) & 0xff;
      frame[21] = channel4AvgValue & 0xff;
      
      Serial.write(frame, FRAME_LEN);
    }
  }
}