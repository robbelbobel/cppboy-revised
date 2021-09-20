#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include "mmu.hpp"
#include "cpu/cpu.hpp"

class Gameboy{
    private:
        // Private Class Pointers
        Cartridge* cartridge;
        MMU* mmu;
        CPU* cpu;
        PPU* ppu;
        InputHandler* inputHandler;

    public:
        // Public Functions
        Gameboy();
        ~Gameboy();
    
        void step();
        void draw(sf::RenderWindow &window);

        bool loadFile(char* path);
};

#endif