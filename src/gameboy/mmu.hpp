#ifndef MMU_HPP
#define MMU_HPP

#include <stdint.h>
#include <iostream>

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

        // Registers
        uint8_t div;
        uint8_t tima;
        uint8_t tma;
        uint8_t tac;

        uint8_t ie;

        // Timer Counters
        uint16_t divCounter;
        uint16_t timaCounter;

        // Private Class Functions
        uint8_t ioRead(const uint16_t &address);
        void ioWrite(const uint16_t &address, const uint8_t &value);

    public:
        // Public Class Functions
        MMU(PPU* ppu, Cartridge* cartridge, InputHandler* inputHandler);
        ~MMU();

        uint8_t read(const uint16_t &address);
        void write(const uint16_t &address, const uint8_t &value);

        void updateTimers();
        void dump();
};

#endif