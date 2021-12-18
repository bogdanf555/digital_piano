#include "Vector.h"

bool Vector::push_back(uint64_t item)
{

    if (_index == _size)
        return false;

    _arr[_index++] = item;
}

uint64_t Vector::at_index(uint16_t index)
{

    if (index >= _index || index < 0)
        return -1;

    return _arr[index];
}

void Vector::remove_helper(uint16_t i)
{

    for (int j = i + 1; j < _index; j++)
    {
        _arr[j - 1] = _arr[j];
    }

    _index--;
}

bool Vector::remove(uint64_t item)
{

    for (int i = 0; i < _index; i++)
    {
        if (item == _arr[i])
        {
            remove_helper(i);
            return true;
        }
    }

    return false;
}

void Vector::reset()
{
    _index = 0;
}

bool Vector::is_empty()
{
    return _index == 0;
}

int Vector::length()
{
    return _index;
}

bool Vector::allocation_failed()
{
    return _size == 0;
}

uint64_t Vector::first()
{
    return _arr[0];
}

uint64_t Vector::last()
{
    if (_index < 1)
        return _arr[0];

    return _arr[_index - 1];
}

void Vector::subtract_by(uint64_t by)
{

    for (int i = 0; i < _index; i++)
    {
        _arr[i] -= by;
    }
}