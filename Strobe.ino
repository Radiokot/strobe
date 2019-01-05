#include "state.h"

#define CONTROL_PIN LED_BUILTIN 

unsigned char state;
unsigned long flashingPeriodMs;
unsigned long lastFlashTimeMs;

void setup() {
  pinMode(CONTROL_PIN, OUTPUT);
  digitalWrite(CONTROL_PIN, LOW);

  state = STATE_REQUIRES_SINGLE_FLASH;
}

void loop() {
  operate();
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

    case STATE_CONTINOUS_FLASHING:
      flashingTick();
      break;
  }
}

/**
 * Sets up continous flashing for given BPM value
 */
void startFlashingBpm(unsigned short bpm) {
  startFlashingHz(round(bpm / 60.0));
}

/**
 * Sets up continous flashing for given Hz value
 */
void startFlashingHz(unsigned char hz) {
  flashingPeriodMs = round(1000.0 / hz);
  state = STATE_CONTINOUS_FLASHING;
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
  digitalWrite(CONTROL_PIN, HIGH);
  delay(1);
  digitalWrite(CONTROL_PIN, LOW);
}
