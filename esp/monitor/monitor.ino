#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  Serial.println("Setting up");

  SerialBT.begin("Heart Rate Monitor");  //Bluetooth device name
}

const int threshHoldFinger = 2000;
const int threshHoldBeat = 2700;

bool fingerDetected = false;
const int beatsArrayLength = 4;
int lastBeatsArray[beatsArrayLength];
unsigned long lastBeatTime = 0;

void loop() {
  delay(10);
  int value = fakeAnalogRead();

  if (value < threshHoldFinger) {
    fingerDetected = false;
    Serial.println("Please place the finger on the sensor");
    SerialBT.println("Please place the finger on the sensor");
  } else if (value > threshHoldFinger && value < threshHoldBeat) {
    fingerDetected = true;
  } else if (value > threshHoldBeat) {
    fingerDetected = true;

    // Wait until peak is over
    while (fakeAnalogRead() > threshHoldBeat) {
      delay(10);
    }

    // Shift array to left
    for (int i = 0; i < beatsArrayLength - 1; i++) {
      lastBeatsArray[i] = lastBeatsArray[i + 1];
    }

    // Push current timestamp to array
    lastBeatsArray[beatsArrayLength - 1] = millis() - lastBeatTime;

    lastBeatTime = millis();

    // Check if enough variables to calculate the BPM are avaiable
    bool canCalculate = true;
    for (int i = 0; i < beatsArrayLength; i++) {
      if (lastBeatsArray[i] < 1) {
        canCalculate = false;
        break;
      }
    }

    if (canCalculate) {
      float val = 0;
      for (int i = 0; i < beatsArrayLength; i++) {
        val += lastBeatsArray[i];
      }

      val = (val / (beatsArrayLength - 1)) / 1000 * 60;
      String bpm = "Current BPM: " + (String) val;
      Serial.println(bpm);
      SerialBT.println(bpm);
    } else {
      Serial.println("Waiting for enough data to calculate BPM");
      SerialBT.println("Waiting for enough data to calculate BPM");
    }
  }
}