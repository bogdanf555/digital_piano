#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>

class Vector
{

private:
    uint16_t _size;
    uint16_t _index;
    uint64_t *_arr;
    void remove_helper(uint16_t);

public:
    Vector(uint16_t size) : _size(size), _index(0)
    {
        _arr = (uint64_t *)malloc(size * sizeof(uint64_t));

        if (_arr == NULL)
        {
            _size = 0;
        }
    }
    bool push_back(uint64_t);
    uint64_t at_index(uint16_t);
    bool remove(uint64_t);
    bool is_empty();
    int length();
    bool allocation_failed();
    void reset();
    uint64_t first();
    uint64_t last();
    void subtract_by(uint64_t);
};
#endif