#include <SPI.h>

class MCP41xxx {
    public:
        MCP41xxx(uint8_t csPin, uint8_t fbPin) {
            _csPin = csPin;
            _fbPin = fbPin;
        }
        
        void begin() {
            digitalWrite(_csPin, HIGH);
            pinMode(_csPin, OUTPUT);
            SPI.begin();

            _wiper = 128;
            _fbIndex = 0;
            for (int i = 0; i < 10; i++) {
                _fbMax[i] = 0;
                _fbMin[i] = 1023;
            }

            setWiper();
            _lastAdjustment = millis();
        };

        void loop() {
            uint16_t fb = analogRead(_fbPin);
            _fbMax[_fbIndex] = max(fb, _fbMax[_fbIndex]);
            _fbMin[_fbIndex] = min(fb, _fbMin[_fbIndex]);

            if (millis() > _lastAdjustment + 500) {
                uint16_t fbMin = 1023;
                uint16_t fbMax = 0;

                for (int i = 0; i < 10; i++) {
                    if (_fbMin[i] < _fbMax[i]) {
                        fbMin = min(fbMin, _fbMin[i]);
                        fbMax = max(fbMax, _fbMax[i]);
                    }
                }

                uint16_t diff = fbMax - fbMin;
                bool change = false;
                
                // 0 is wiper to signal
                // 255 is wiper to ground
                if (diff < 20 && _wiper > 0) {
                    _wiper--;
                    change = true;
                } else if (diff > 23 && _wiper < 255) {
                    _wiper++;
                    change = true;
                }

                char buf[128];
                sprintf(buf, "Wiper: %3d, Diff: %3d", _wiper, diff);
                Serial.println(buf);

                _fbIndex++;
                if (_fbIndex == 10) {
                    _fbIndex = 0;
                }
                _fbMin[_fbIndex] = 1023;
                _fbMax[_fbIndex] = 0;

                _lastAdjustment = millis();
                if (change) {
                    setWiper();
                }
            }
        }

        void set(uint8_t val) {
            _wiper = val;
            setWiper();
        }

    private:
        void setWiper() {
            digitalWrite(_csPin, LOW);
            SPI.transfer(0x17);
            SPI.transfer(_wiper);
            digitalWrite(_csPin, HIGH);
        }

        uint8_t _csPin;
        uint8_t _fbPin;

        uint16_t _fbMax[10];
        uint16_t _fbMin[10];
        uint8_t _fbIndex;
        uint32_t _lastAdjustment;

        uint8_t _wiper;
};