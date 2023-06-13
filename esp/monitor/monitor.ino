int pin = A0;

void setup() {
  Serial.begin(115200);
  Serial.println("Setting up");
}


void loop() {
  int value = fakeAnalogRead();
  Serial.println(value);
  delay(10);
}

// https://ecgwaves.com/topic/ecg-normal-p-wave-qrs-complex-st-segment-t-wave-j-point/
// When no finger is placed, the value is zero
// When a finger is placed the value is around 2300 +- 100
// When a pulse is detected the value is around 2800 +- 100
unsigned long nextInterval = 0;

const int rrIntervall = 880;
const int qrsDuration = 120;

const int noFingerValue = 0;
const int fingerBaseValue = 2300;
const int fingerPulseAdditionValue = 500;
const float sensorVariance = 0.03;

// Percentage from which the timings differ
const float pulseVariance = 0.10;

bool isPeak = false;
bool hasFinger = true;

int fakeAnalogRead() {
  if (!hasFinger) {
    return 0;
  }

  unsigned long currentMillis = millis();
  int sensorValue = random(fingerBaseValue - (fingerBaseValue * sensorVariance), fingerBaseValue + (fingerBaseValue * sensorVariance));
  if (isPeak) {
    if (currentMillis >= nextInterval) {
      nextInterval = currentMillis + random(rrIntervall - (rrIntervall * pulseVariance), rrIntervall + (rrIntervall * pulseVariance));
      isPeak = false;
    }
    sensorValue = sensorValue + random(fingerPulseAdditionValue - (fingerPulseAdditionValue * sensorVariance), fingerPulseAdditionValue + (fingerPulseAdditionValue * sensorVariance));
  } else {
    if (currentMillis >= nextInterval) {
      nextInterval = currentMillis + random(qrsDuration - (qrsDuration * pulseVariance), qrsDuration + (qrsDuration * pulseVariance));
      isPeak = true;
    }
  }
  return sensorValue;
}