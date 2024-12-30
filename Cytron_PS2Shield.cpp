/*
Original written by:
            Cytron Technologies

Modified:
  29/06/15  Idris, Cytron Technologies    - Point to IDE SoftwareSerial
                                          - Restructure the code style to follow standard Arduino library
Updated:
  [Your Date]  [Your Name]                - Add support for multiple hardware serial ports
                                          - Avoid issues with Arduino Mega R3
*/

#include "HardwareSerial.h"
#include "Cytron_PS2Shield.h"

Cytron_PS2Shield::Cytron_PS2Shield(uint8_t rxpin, uint8_t txpin) {
  _txpin = txpin;
  _rxpin = rxpin;
  hardwareSerial = false;
  hwSerial = nullptr;
  PS2Serial = nullptr;
}

Cytron_PS2Shield::Cytron_PS2Shield(HardwareSerial& hwSerialRef) {
  hardwareSerial = true;
  hwSerial = &hwSerialRef;
  PS2Serial = nullptr;
}

void Cytron_PS2Shield::begin(uint32_t baudrate) {
  if (hardwareSerial && hwSerial) {
    hwSerial->begin(baudrate);
    while (!*hwSerial);
  } else {
    pinMode(_rxpin, INPUT);
    pinMode(_txpin, OUTPUT);
    PS2Serial = new SoftwareSerial(_rxpin, _txpin);
    PS2Serial->begin(baudrate);
  }
  pinMode(A1, OUTPUT);
  digitalWrite(A1, HIGH);
}

void Cytron_PS2Shield::write(uint8_t data) {
  if (hardwareSerial && hwSerial) {
    while (hwSerial->available() > 0) {
      hwSerial->read();
    }
    hwSerial->write(data);
    hwSerial->flush();
  } else if (PS2Serial) {
    while (PS2Serial->available() > 0) {
      PS2Serial->read();
    }
    PS2Serial->write(data);
  }
}

uint8_t Cytron_PS2Shield::read(void) {
  uint8_t rec_data;
  long waitcount = 0;

  if (hardwareSerial && hwSerial) {
    while (true) {
      if (hwSerial->available() > 0) {
        rec_data = hwSerial->read();
        SERIAL_ERR = false;
        return rec_data;
      }
      waitcount++;
      if (waitcount > 50000) {
        SERIAL_ERR = true;
        return 0xFF;
      }
    }
  } else if (PS2Serial) {
    while (true) {
      if (PS2Serial->available() > 0) {
        rec_data = PS2Serial->read();
        SERIAL_ERR = false;
        return rec_data;
      }
      waitcount++;
      if (waitcount > 50000) {
        SERIAL_ERR = true;
        return 0xFF;
      }
    }
  }
  return 0xFF; // Default return if no valid Serial
}

uint8_t Cytron_PS2Shield::readButton(uint8_t key) {
  if (!hardwareSerial && PS2Serial) PS2Serial->listen();
  write(key);
  return read();
}

boolean Cytron_PS2Shield::readAllButton() {
  uint8_t nbyte;
  uint32_t waitcount;

  write(PS2_BUTTON_JOYSTICK);

  if (hardwareSerial && hwSerial) {
    nbyte = hwSerial->readBytes(ps_data, 6);
    return (nbyte == 6);
  } else if (PS2Serial) {
    waitcount = 0;
    while (PS2Serial->available() < 6) {
      waitcount++;
      if (waitcount > 50000) {
        return false;
      }
    }
    for (int i = 0; i < 6; i++) {
      ps_data[i] = PS2Serial->read();
    }
    return true;
  }
  return false;
}

void Cytron_PS2Shield::vibrate(uint8_t motor, uint8_t value) {
  uint8_t _motor = (motor == 1) ? PS2_MOTOR_1 : PS2_MOTOR_2;
  write(_motor);
  write(value);
}

void Cytron_PS2Shield::reset(uint8_t reset) {
  digitalWrite(A1, reset == 1 ? LOW : HIGH);
}
