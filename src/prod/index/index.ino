#include <Adafruit_ADS1X15.h>

#define PORT_SPEED 115200

#define INIT_DELAY 4978
#define POLLING_INTERVAL 100

#define MODULE_1_ADDRESS 0x48
#define CHANNEL_1 0
#define CHANNEL_2 1
#define CHANNEL_3 2
#define CHANNEL_4 3

Adafruit_ADS1115 ada;

const byte FRAME_LEN = 22;  // byte
byte frame[FRAME_LEN];

uint16_t adc1;
uint16_t adc2;
uint16_t adc3;
uint16_t adc4;

unsigned long sensPollTimeStamp;
unsigned long currentTimeStamp;

void setup() {
  sensPollTimeStamp = millis();

  Serial.begin(PORT_SPEED);

  ada.setDataRate(RATE_ADS1115_250SPS);  // 250 samples per second - 860 max, see <Adafruit_ADS1X15.h>
  ada.begin();

  delay(INIT_DELAY);
}

void loop() {
  currentTimeStamp = millis();

  if (currentTimeStamp - sensPollTimeStamp >= POLLING_INTERVAL) {
    sensPollTimeStamp = currentTimeStamp;

    adc1 = ada.readADC_SingleEnded(CHANNEL_1);
    adc2 = ada.readADC_SingleEnded(CHANNEL_2);
    adc3 = ada.readADC_SingleEnded(CHANNEL_3);
    adc4 = ada.readADC_SingleEnded(CHANNEL_4);

    frame[0] = 0;
    frame[1] = 0;
    frame[2] = 0;
    frame[3] = MODULE_1_ADDRESS;

    frame[9] = lowByte(currentTimeStamp);
    currentTimeStamp >>= 8;
    frame[8] = lowByte(currentTimeStamp);
    currentTimeStamp >>= 8;
    frame[7] = 2;
    frame[6] = lowByte(currentTimeStamp);
    currentTimeStamp >>= 8;
    frame[5] = lowByte(currentTimeStamp);
    currentTimeStamp >>= 8;
    frame[4] = 1;

    frame[10] = 3;
    frame[11] = highByte(adc1);
    frame[12] = lowByte(adc1);
    frame[13] = 4;
    frame[14] = highByte(adc2);
    frame[15] = lowByte(adc2);
    frame[16] = 5;
    frame[17] = highByte(adc3);
    frame[18] = lowByte(adc3);
    frame[19] = 6;
    frame[20] = highByte(adc4);
    frame[21] = lowByte(adc4);

    Serial.write(frame, FRAME_LEN);
  }
}