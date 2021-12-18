#include "key.h"
#include <Arduino.h>

void Key::begin()
{
    pinMode(_pin, INPUT_PULLUP);
    _state = digitalRead(_pin);
}

int Key::isToggled()
{
    bool v = digitalRead(_pin);
    if (v != _state)
    {
        _state = v;
        if (_state)
        {
            return 2;
        }
        return 1;
    }
    return 0;
}