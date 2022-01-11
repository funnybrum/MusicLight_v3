#pragma once

#include "Arduino.h"

class MSGEQ7 {
    public:
        MSGEQ7(uint8_t resetPin, uint8_t strobePin, uint8_t analogPin) {
            _resetPin = resetPin;
            _strobePin = strobePin;
            _analogPin = analogPin;
        }

        void begin() {
            pinMode(_resetPin, OUTPUT);
            pinMode(_strobePin, OUTPUT);

            digitalWrite(_resetPin, LOW);
            digitalWrite(_strobePin, HIGH);

            digitalWrite(_resetPin, HIGH);
            digitalWrite(_resetPin, LOW);
            _counter = 0;
            for (int i = 0; i < 7; i++) {
                _noiseFloor[i] = 255;
            }
        }

        void read() {
            if (_counter > 10000) {
                // Empty cycle to reset the MSGEQ7
                digitalWrite(_resetPin, HIGH);
                digitalWrite(_resetPin, LOW);

                for (uint8_t band = 0; band < 7; band++) {
                    digitalWrite(_strobePin, LOW); 
                    delayMicroseconds(30);
                    digitalWrite(_strobePin, HIGH);
                    delayMicroseconds(10);
                }
                _counter = 0;
                return;
            }

            memmove(_data[0], _data[1], 9 * sizeof(_data[0]));

            for(uint8_t band = 0; band < 7; band++ ) {
                digitalWrite(_strobePin, LOW); 
                delayMicroseconds(30);
                _data[9][band] = analogRead(_analogPin);
                digitalWrite(_strobePin, HIGH);
                if (_data[9][band] > 30 && _noiseFloor[band] > _data[9][band]) {
                    _noiseFloor[band] = _data[9][band];
                }
            }

            // Introduce some jitter to avoid issues with certain frequencies.
            uint16_t r = random(1000);
            delayMicroseconds(r);
        }

        uint8_t getBandValue(uint8_t band) {
            uint8_t value = 0;
            for (int i = 0; i < 9; i++) {
                value = max(value, _data[i][band]);
            }

            if (value > _noiseFloor[band]) {
                return map(value, _noiseFloor[band], 255, 0, 255);
            } else {
                return 0;
            }
        }

    private:
        uint8_t _resetPin;
        uint8_t _strobePin;
        uint8_t _analogPin;
        uint8_t _data[10][7];
        uint8_t _noiseFloor[7];
        uint16_t _counter;
};