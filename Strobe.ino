#define CONTROL_PIN LED_BUILTIN 

unsigned long flashingPeriodMs;
unsigned long lastFlashTimeMs;

void setup() {
  pinMode(CONTROL_PIN, OUTPUT);
  digitalWrite(CONTROL_PIN, LOW);

  startFlashingHz(10);
}

void loop() {
  flashingTick();
}

/**
 * Sets up flashing period for given BMP value
 */
void startFlashingBpm(unsigned short bmp) {
  flashingPeriodMs = round(1000.0 / (bmp / 60.0));
}

/**
 * Sets up flashing period for given Hz value
 */
void startFlashingHz(unsigned char hz) {
  flashingPeriodMs = round(1000.0 / hz);
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
