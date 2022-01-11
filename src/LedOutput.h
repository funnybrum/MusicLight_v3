#include "Arduino.h"

class LedOutput {
    public:
        LedOutput(uint8_t pin, uint8_t threshold=80, uint8_t upperThreshold=200) {
            _pin = pin;
            _threshold = threshold;
            _upperThreshold = upperThreshold;
        }

        void begin() {
            _value = LOW;
            setOutput();
            pinMode(_pin, OUTPUT);
        }

        void loop(uint8_t value) {
            // uint8_t target;
            // if (value < _threshold) {
            //     target = 0;
            // } else {
            //     target = map(value, _threshold, _upperThreshold, 0, 255);
            // }

            if (value > _threshold) {
                _value += 16;
            } else if (_value > 0) {
                _value--;
            }

            // if (target > _value) {
            //     _value = min(_value + 4, target);
            // } else {
            //     _value = max(_value - 1, target);
            // }

            setOutput();
        }

    private:
        void setOutput() {
            digitalWrite(_pin, _value);
        }

        uint8_t _pin;
        uint8_t _threshold;
        uint8_t _upperThreshold;
        uint8_t _value;
};