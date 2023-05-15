#pragma once


#include <cstdint>


inline uint32_t reverse_bytes(uint32_t value)
{
    uint8_t *ptr = reinterpret_cast<uint8_t *> (&value);
    uint8_t temp = *ptr;
    *ptr = *(ptr + 3);
    *(ptr + 3) = temp;

    temp = *(ptr + 1);
    *(ptr + 1) = *(ptr + 2);
    *(ptr + 2) = temp;

    return value;
}