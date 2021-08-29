#ifndef MMU_HPP
#define MMU_HPP

#include <stdint.h>

#include "cartridge.hpp"

class MMU{
    private:
        // Private Class Pointers
        Cartridge* cartridge;

    public:
        // Public Class Functions
        MMU();
        ~MMU();

        uint8_t read(const uint16_t &address);
        void write(const uint16_t &address, const uint8_t &value);
};

#endif