#ifndef MMU_HPP
#define MMU_HPP

#include <stdint.h>

#include "ppu.hpp"
#include "cartridge.hpp"
#include "inputHandler.hpp"

class MMU{
    private:
        // Private Class Variables
        PPU* ppu;
        Cartridge* cartridge;
        InputHandler* inputHandler;
        // Memory Arrays
        uint8_t wRAM[0x2000];
        uint8_t hRAM[0x80];
        
        uint8_t IE;

        // Private Class Functions
        uint8_t ioRead(const uint16_t &address);
        void ioWrite(const uint16_t &address, const uint8_t &value);

    public:
        // Public Class Functions
        MMU(PPU* ppu, Cartridge* cartridge, InputHandler* inputHandler);
        ~MMU();

        uint8_t read(const uint16_t &address);
        void write(const uint16_t &address, const uint8_t &value);
        void dump();
};

#endif