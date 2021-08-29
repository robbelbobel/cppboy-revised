#ifndef CPU_HPP
#define CPU_HPP

#include "../mmu.hpp"

class CPU{
    private:
        // Private Class Pointers
        MMU* mmu;

        // Registers
        uint8_t A;
        uint8_t F;
        uint8_t B;
        uint8_t C;
        uint8_t D;
        uint8_t E;
        uint8_t H;
        uint8_t L;

        uint16_t SP;
        uint16_t PC;

    public:
        // Public Class Functions
        CPU(MMU* mmu);
        ~CPU();
        
        void reset();
        void step();
};

#endif