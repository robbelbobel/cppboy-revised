#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include <iostream>

#include "mmu.hpp"
#include "cpu/cpu.hpp"
#include "cartridge.hpp"

class Gameboy{
    private:
        // Private Class Pointers
        Cartridge* cartridge;
        MMU* mmu;
        CPU* cpu;

    public:
        // Public Functions
        Gameboy();
        ~Gameboy();
    
        void step();
        bool loadFile(char* path);
};

#endif