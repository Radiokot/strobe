#include "state.h"
#include "command.h"

#define CONTROL_PIN 8 

unsigned char state;
unsigned long flashingPeriodMs;
unsigned long lastFlashTimeMs;

void setup() {
  pinMode(CONTROL_PIN, OUTPUT);
  digitalWrite(CONTROL_PIN, LOW);

  Serial.begin(9600);
  Serial.setTimeout(10);
}

void loop() {
  readCommand();
  operate();
}

void readCommand() {
  if (Serial.available()) {
    unsigned char buffer[64];
    size_t length = Serial.readBytes(buffer, 64);
    parseCommand(buffer, length);
  }
}

void parseCommand(unsigned char *data, unsigned short length) {
  if (length == 0) {
    return;
  }

  unsigned char command = *data;
  switch (command) {
    case COMMAND_SINGLE_FLASH:
      state = STATE_REQUIRES_SINGLE_FLASH;
      return;
    case COMMAND_STOP_ALL:
      state = STATE_IDLE;
      return;
  }
  
  unsigned short payloadLength = length - 1;
  
  switch (command) {  
   case COMMAND_FLASHING_HZ:
      parseFlashingHzCommand(data + 1, payloadLength);
      break;
    case COMMAND_FLASHING_BPM:
      parseFlashingBpmCommand(data + 1, payloadLength);
      break;
  }
}

void parseFlashingHzCommand(unsigned char *payload, unsigned short length) {
  if (length != 1) {
    return;
  }

  startFlashingHz(*payload);
}

void parseFlashingBpmCommand(unsigned char *payload, unsigned short length) {
  if (length != 2) {
    return;
  }
  
  unsigned short bpm = *((unsigned short*) payload); 
  startFlashingBpm(bpm);
}

/**
 * Performs action according to the state
 */
void operate() {
  switch(state) {
    case STATE_REQUIRES_SINGLE_FLASH:
      singleFlash();
      state = STATE_IDLE;
      break;

    case STATE_CONTINUOUS_FLASHING:
      flashingTick();
      break;
  }
}

/**
 * Sets up continous flashing for given BPM value
 */
void startFlashingBpm(unsigned short bpm) {
  flashingPeriodMs = round(1000.0 / (bpm / 60.0));
  state = STATE_CONTINUOUS_FLASHING;
}

/**
 * Sets up continous flashing for given Hz value
 */
void startFlashingHz(unsigned char hz) {
  flashingPeriodMs = round(1000.0 / hz);
  state = STATE_CONTINUOUS_FLASHING;
}

/**
 * Performs single flash when it's needed
 * according to the flashing period
 */
void flashingTick() {
  unsigned long currentTimeMs = millis();
  unsigned long delta = currentTimeMs - lastFlashTimeMs;
  
  if (delta < 0) {
    delta = flashingPeriodMs;
  }
  
  if (delta >= flashingPeriodMs) {
    singleFlash(currentTimeMs);
  }
}

/**
 * Performs single flash,
 * sets last flash time to the current
 */
void singleFlash() {
  singleFlash(millis());
}

/**
 * Performs single flash,
 * sets last flash time to the given
 */
void singleFlash(unsigned long currentTimeMs) {
  lastFlashTimeMs = currentTimeMs;
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(CONTROL_PIN, HIGH);
  delay(20);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(CONTROL_PIN, LOW);
  delay(10);
}
