#include <Control_Surface.h> 
#include <MIDI_Interfaces/DebugMIDI_Interface.hpp>

#define DRUM_NUM 4

const int ledPin[DRUM_NUM] = {2, 3, 4, 5};
const int piezoPin[DRUM_NUM] = {A1, A2, A3, A4};
const int drumNote[DRUM_NUM] = {36, // Kick
                                38, // Snare center
                                49, // Crash
                                42  // Hi-hat foot close 
                                };

const int threshold = 60;
const int maxThreshold = 300;
const int baseVelocity = 32;
const int maxVelocity = 127;

// Instantiate a MIDI over USB interface.
// USBDebugMIDI_Interface midi; // For Debugging
USBMIDI_Interface midi;

// // Analog pin for the potentiometer
// const int potentiometerPin = A5;

// Instantiate a CCPotentiometer object
CCPotentiometer potentiometer {
  A5,                                   // Analog pin connected to potentiometer
  {MIDI_CC::Channel_Volume, Channel_1}, // Channel volume of channel 1
};

void setup() {
  for (int i=0; i<DRUM_NUM; i++) {
    pinMode(ledPin[i], OUTPUT);
    digitalWrite(ledPin[i], LOW);
  }
  Control_Surface.begin(); // Initialize Control Surface
  Serial.begin(115200);
}

void loop() {
  // Loop through all drum elements
  for (int i = 0; i < DRUM_NUM; i++) {
    // Read the analog value from the piezo element
    int piezoValue = analogRead(piezoPin[i]);

    // If the piezo value exceeds the threshold, trigger a MIDI note
    if (piezoValue > threshold) {
      // Velocity
      int velocity = map(piezoValue, threshold, maxThreshold, baseVelocity, maxVelocity);
      if (velocity > maxVelocity) {
        velocity = maxVelocity;
      }

      // Debugging
      Serial.print("DRUM_NUM: ");
      Serial.print(i);
      Serial.print(" Note: ");
      Serial.print(drumNote[i]);
      Serial.print(" Piezo Value: ");
      Serial.print(piezoValue);
      Serial.print(" Velocity: ");
      Serial.println(velocity);

      // LED ON
      digitalWrite(ledPin[i], HIGH);

      // // Read the potentiometer value
      // int potentiometerValue = analogRead(potentiometerPin);
  
      // // Send MIDI volume message based on potentiometer value
      // midi.sendCC(MIDI_CC::Channel_Volume, map(potentiometerValue, 0, 1023, 0, 127));

      // Trigger MIDI note 
      midi.sendNoteOn(drumNote[i], 127);
      delay(50); // Add a small delay to prevent rapid triggering
      midi.sendNoteOff(drumNote[i], 0); // Turn off the note after a short delay
    } else {
      // LED OFF
      digitalWrite(ledPin[i], LOW);
    }
  }

  // Update Control-Surface
  Control_Surface.loop();
  // Serial.println(potentiometer.getRawValue());
}