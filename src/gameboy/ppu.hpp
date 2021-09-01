#ifndef PPU_HPP
#define PPU_HPP

#include <stdint.h>

class PPU{
public:
    // Public Class Functions
    PPU();
    ~PPU();

    // Public Class Variables
    // Memory Arrays
    uint8_t vRAM[0x2000];
    uint8_t oam[0xA0];
    // LCD Control Register (LCDC)
    bool ppuEnable;
    bool winTMArea;
    bool winEnable;
    bool bgWinTDArea;
    bool bgTMArea;
    bool objSize;
    bool objEnable;
    bool bgWinEnable;
    // LCD Status Register (STAT)
    bool lycIntSrc;
    bool oamIntSrc;
    bool vBlankIntSrc;
    bool hBlankIntSrc;
    bool lycFlag;
    uint8_t mode;
};  

#endif