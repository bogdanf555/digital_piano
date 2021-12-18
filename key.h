#include <stdint.h>

class Key
{
private:
    bool _state;
    uint8_t _pin;

public:
    Key(uint8_t pin) : _pin(pin)
    {
    }
    void begin();
    int isToggled();
};