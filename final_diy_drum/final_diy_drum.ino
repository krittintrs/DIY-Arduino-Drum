#include <Control_Surface.h> // Include the Control Surface library

BEGIN_CS_NAMESPACE

/*
 * @brief   A class for reading from a piezo sensor and sending MIDI note events.
 *
 * The piezo sensor value is mapped to MIDI velocity.
 */
class PiezoMIDIOutput : public MIDIOutputElement {
 public:
  /*
   * @brief   Create a new PiezoMIDIOutput object on the given analog pin, with the 
   *          given address and MIDI channel.
   * 
   * @param   pin
   *          The analog input pin to read from.
   * @param   address
   *          The MIDI address to send to.
   * @param   threshold
   *          The MIDI note velocity [0, 127].
   */
  PiezoMIDIOutput(pin_t piezoPin, MIDIAddress address, pin_t ledPin)
    : piezoPin(piezoPin), address(address), ledPin(ledPin) {}

 public:
  // Initialize: nothing to do here
  // This method is called once by `Control_Surface.begin()`.
  void begin() final override {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
  }

  // Update: read from the piezo sensor and send MIDI messages.
  // This method is called continuously by `Control_Surface.loop()`.
  void update() final override {
    int piezoValue = analogRead(piezoPin);
    if (piezoValue > 80) {
      // Map piezo value to MIDI velocity
      uint8_t velocity = map(piezoValue, 0, 300, 64, 127);  
      if (velocity > 127) {
        velocity = 127;
      }

      // LED ON
      digitalWrite(ledPin, HIGH);
      
      // Send MIDI note on event
      Control_Surface.sendNoteOn(address, velocity);
      delay(50); 
      Control_Surface.sendNoteOff(address, 0);

    } else {
      // LED OFF
      digitalWrite(ledPin, LOW);
    }
  }

 private:
  pin_t piezoPin;
  const MIDIAddress address;
  pin_t ledPin;
};

END_CS_NAMESPACE

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

// // For Debug
// #include <MIDI_Interfaces/DebugMIDI_Interface.hpp>
// USBDebugMIDI_Interface midi;

// Instantiate MIDI over USB interface
USBMIDI_Interface midi;

// Instantiate CCPotentiometer object
CCPotentiometer potentiometer {
  A1,                                   // Analog pin connected to potentiometer
  {MIDI_CC::Channel_Volume, Channel_1}, // Channel volume of channel 1
};

// Instantiate PiezoMIDIOutput object
PiezoMIDIOutput piezoRight {
  A2,                       // Piezo sensor on analog pin A2
  {38, Channel_1},          // Note D1 - Snare center 
  5                         // LED pin 5   
};

PiezoMIDIOutput piezoMiddleTop {
  A3,                       // Piezo sensor on analog pin A3
  {49, Channel_1},          // Note Db2 - Crash Left
  4                         // LED pin 4   
};

PiezoMIDIOutput piezoMiddleBottom {
  A4,                       // Piezo sensor on analog pin A4
  {36, Channel_1},          // Note C1 - Kick    
  3                         // LED pin 3    
};

PiezoMIDIOutput piezoLeft {
  A5,                       // Piezo sensor on analog pin A5
  {42, Channel_1},          // Note Gb1 - Hi-hat foot close 
  2                         // LED pin 2   
};

void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface
}