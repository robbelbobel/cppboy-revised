#ifndef MMU_HPP
#define MMU_HPP

#include <stdint.h>

#include "cartridge.hpp"
#include "ppu.hpp"

class MMU{
    private:
        // Private Class Pointers
        PPU* ppu;
        Cartridge* cartridge;

        uint8_t ioRead(const uint16_t &address);
        void ioWrite(const uint16_t &address, const uint8_t &value);

    public:
        // Public Class Functions
        MMU(PPU* ppu, Cartridge* cartridge);
        ~MMU();

        uint8_t read(const uint16_t &address);
        void write(const uint16_t &address, const uint8_t &value);
};

#endif