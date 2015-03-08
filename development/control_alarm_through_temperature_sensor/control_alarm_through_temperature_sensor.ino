/*
 *  Control Alarm Through Temperature Sensor.
 *
 *  Copyright (C) 2010 Efstathios Chatzikyriakidis (contact@efxa.org)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// include float limits for DBL_EPSILON.
#include <float.h>

// include notes' frequencies.
#include "pitches.h"

const int ledPin = 13;   // the led pin of the alarm.
const int piezoPin = 9;  // the piezo pin of the alarm.
const int sensorPin = 0; // the sensor pin of the alarm.

// sensor threshold value for the alarm (Celsius).
const double sensorThreshold = 41.5;

// delay time before next fetching value from sensor occur.
const int delayTime = 1000;

// notes in the alarm melody.
const int notesMelody[] = { NOTE_G4, NOTE_C4, NOTE_G3 };

// calculate the number of the notes in the melody in the array.
const int NUM_NOTES = (int) (sizeof (notesMelody) / sizeof (const int));

// note durations: 4 = quarter note, 8 = eighth note, etc.
const int noteDurations[] = { 4, 8, 4 };

// startup point entry (runs once).
void setup() {
  // set the input sensor as input.
  pinMode(sensorPin, INPUT);

  // set the led and piezo as outputs.
  pinMode(ledPin, OUTPUT);
  pinMode(piezoPin, OUTPUT);
}

// loop the main sketch.
void loop() {
  // get sensor value and convert it to Celsius temperature.
  double sensorValue = getCelsius(getVoltage(sensorPin));

  // if sensor value is less than the threshold.
  if (isLessThan(sensorValue, sensorThreshold)) {
    // dark the led and do nothing.
    digitalWrite(ledPin, LOW);

    // delay some time (temperature changes slowly).
    delay(delayTime);
  }
  else {
    // first light the led.
    digitalWrite(ledPin, HIGH);

    // trigger the alarm melody.
    playMelody();
  }
}

// play a melody and return immediately.
void playMelody () {
  // iterate over the notes of the melody.
  for (int thisNote = 0; thisNote < NUM_NOTES; thisNote++) {
    // to calculate the note duration, take one second divided by the note type.
    // e.g. quarter note = 1000/4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];

    // play the tone.
    tone(piezoPin, notesMelody[thisNote], noteDuration);

    // to distinguish notes, set a minimum time between them.
    // the note's duration plus 30% seems to work well enough.
    int pauseBetweenNotes = noteDuration * 1.30;

    // delay some time.
    delay(pauseBetweenNotes);
  }
}

// get the voltage on the analog input pin.
double getVoltage (const int pin) {
  // converting from a 0 to 1024 digital range to
  // 0 to 5 volts (each 1 reading equals ~ 5 mv).
  return (analogRead(pin) * .004882814);
}

// convert the voltage to Celsius temperature.
double getCelsius (const double voltage) {
  return (voltage - .5) * 100;
}

// check if double a is less than double b.
bool isLessThan(const double a, const double b) {
  return (b - a) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * DBL_EPSILON);
}
