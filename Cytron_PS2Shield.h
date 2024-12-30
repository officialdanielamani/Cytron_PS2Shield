/*
Original written by:
            Cytron Technologies

Modified:
  29/06/2015  Idris, Cytron Technologies    - Point to IDE SoftwareSerial
                                            - Restructure the code style to follow standard Arduino library
Updated:
  30/12/2024  Daniel Amani, ProjectEDNA     - Add support for multiple hardware serial ports
                                            - Avoid issues with Arduino Mega R3
*/

#ifndef Cytron_PS2Shield_h
#define Cytron_PS2Shield_h

#include "Arduino.h"
#include <SoftwareSerial.h>

// Define PS2 button to number
enum {
  // Digital button
  PS2_SELECT,
  PS2_JOYSTICK_LEFT,
  PS2_JOYSTICK_RIGHT,
  PS2_START,
  PS2_UP,
  PS2_RIGHT,
  PS2_DOWN,
  PS2_LEFT,
  PS2_LEFT_2,
  PS2_RIGHT_2,
  PS2_LEFT_1,
  PS2_RIGHT_1,
  PS2_TRIANGLE,
  PS2_CIRCLE,
  PS2_CROSS,
  PS2_SQUARE,
  // Analog button
  PS2_JOYSTICK_LEFT_X_AXIS,
  PS2_JOYSTICK_LEFT_Y_AXIS,
  PS2_JOYSTICK_RIGHT_X_AXIS,
  PS2_JOYSTICK_RIGHT_Y_AXIS,
  PS2_JOYSTICK_LEFT_UP,
  PS2_JOYSTICK_LEFT_DOWN,
  PS2_JOYSTICK_LEFT_LEFT,
  PS2_JOYSTICK_LEFT_RIGHT,
  PS2_JOYSTICK_RIGHT_UP,
  PS2_JOYSTICK_RIGHT_DOWN,
  PS2_JOYSTICK_RIGHT_LEFT,
  PS2_JOYSTICK_RIGHT_RIGHT,
  // Check connection status
  PS2_CONNECTION_STATUS,
  // Control motor vibration
  PS2_MOTOR_1,
  PS2_MOTOR_2,
  // Read all button
  PS2_BUTTON_JOYSTICK
};

class Cytron_PS2Shield {
  public:
    boolean SERIAL_ERR;
    uint8_t ps_data[6];
    uint8_t _txpin, _rxpin;

    // Constructors
    Cytron_PS2Shield(uint8_t rxpin, uint8_t txpin); // SoftwareSerial
    Cytron_PS2Shield(HardwareSerial& hwSerial);    // HardwareSerial

    // Functions
    void begin(uint32_t baudrate);
    uint8_t readButton(uint8_t key);
    boolean readAllButton();
    void vibrate(uint8_t motor, uint8_t value);
    void reset(uint8_t reset);

  protected:
    boolean hardwareSerial;
    HardwareSerial* hwSerial;     // Pointer to HardwareSerial instance
    SoftwareSerial* PS2Serial;    // Pointer to SoftwareSerial instance

    void write(uint8_t data);
    uint8_t read(void);
};

#endif
