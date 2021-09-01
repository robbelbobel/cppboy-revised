#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include <iostream>

#include "mmu.hpp"
#include "cpu/cpu.hpp"

class Gameboy{
    private:
        // Private Class Pointers
        Cartridge* cartridge;
        MMU* mmu;
        CPU* cpu;
        PPU* ppu;

    public:
        // Public Functions
        Gameboy();
        ~Gameboy();
    
        void step();
        bool loadFile(char* path);
};

#endif