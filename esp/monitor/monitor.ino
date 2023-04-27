////////////////////////////////////////////////////////////////////////
/// Copyright (c)2015 Dan Truong
/// Permission is granted to use this software under the MIT
/// licence, with my name and copyright kept in source code
/// http://http://opensource.org/licenses/MIT
///
/// KY039 Arduino Heartrate Monitor V1.0 (April 02, 2015)
////////////////////////////////////////////////////////////////////////

// German Comments by Joy-IT

int rawValue;


bool heartbeatDetected(int IRSensorPin, int delay) {
  static int maxValue = 0;
  static bool isPeak = false;


  bool result = false;

  rawValue = analogRead(IRSensorPin);
  // Hier wird der aktuelle Spannungswert am Fototransistor ausgelesen und in der rawValueVariable zwischengespeichert
  rawValue *= (1000 / delay);

  // Sollte der aktuelle Wert vom letzten maximalen Wert zu weit abweichen
  // (z.B. da der Finger neu aufgesetzt oder weggenommen wurde)
  // So wird der MaxValue resetiert, um eine neue Basis zu erhalten.
  if (rawValue * 4L < maxValue) { maxValue = rawValue * 0.8; }  // Detect new peak
  if (rawValue > maxValue - (1000 / delay)) {
    // Hier wird der eigentliche Peak detektiert. Sollte ein neuer RawValue groeßer sein
    // als der letzte maximale Wert, so wird das als Spitze der aufgezeichnten Daten erkannt.
    if (rawValue > maxValue) {
      maxValue = rawValue;
    }
    // Zum erkannten Peak soll nur ein Herzschlag zugewiesen werden
    if (isPeak == false) {
      result = true;
    }
    isPeak = true;
  } else if (rawValue < maxValue - (3000 / delay)) {
    isPeak = false;
    // Hierbei wird der maximale Wert bei jeden Durchlauf
    // etwas wieder herabgesetzt. Dies hat den Grund, dass
    // nicht nur der Wert sonst immer stabil bei jedem Schlag
    // gleich oder kleiner werden wuerde, sondern auch,
    // falls der Finger sich minimal bewegen sollte und somit
    // das Signal generell schwaecher werden wuerde.
    maxValue -= (1000 / delay);
  }
  return result;
}
int ledPin = 13;
int analogPin = 0;

void setup() {
  // Die eingebaute Arduino LED (Digital 13), wird hier zur Ausgabe genutzt
  pinMode(ledPin, OUTPUT);

  // Serielle Ausgabe Initialisierung
  Serial.begin(9600);
  Serial.println("Heartbeat Detektion Beispielcode.");
}

const int delayMsec = 60;  // 100msec per sample

// Das Hauptprogramm hat zwei Aufgaben:
// - Wird ein Herzschlag erkannt, so blinkt die LED kurz aufgesetzt
// - Der Puls wird errechnet und auf der serriellen Ausgabe ausgegeben.

void loop() {
  // static int beatMsec = 0;
  // int heartRateBPM = 0;
 if (heartbeatDetected(analogPin, delayMsec)) {
    // heartRateBPM = 60000 / beatMsec;
    // LED-Ausgabe bei Herzschlag
    //digitalWrite(ledPin, 1);

    // Serielle Datenausgabe
    Serial.print("Puls erkannt: ");
    // Serial.println(heartRateBPM);

    // beatMsec = 0;
  } else {
    //digitalWrite(ledPin, 0);
  }
  //delay(delayMsec);
  //beatMsec += delayMsec;
}