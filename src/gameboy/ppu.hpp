#ifndef PPU_HPP
#define PPU_HPP

#include <stdint.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

enum PPU_MODES{
    PPU_HBLANK,
    PPU_VBLANK,
    PPU_OAM_SEARCH,
    PPU_PIXEL_TRANSFER
};

class PPU{
private:
    // Private Class Functions
    void hBlank();
    void vBlank();
    void oamSearch();
    void pixelTransfer();

    // Private Class Variables
    sf::Color pixelColors[4];

public:
    // Public Class Functions
    PPU();
    ~PPU();

    void step();
    void draw(sf::RenderWindow &window);

    // Public Class Variables
    uint16_t cycleCount;
    bool objArray[40];
    sf::Color pixelArray[176][176];
    // Memory Arrays
    uint8_t vRAM[0x2000];
    uint8_t oam[0xA0];
    // Interrupt
    bool vBlankInt;
    // Registers
    // Pallette Data
    uint8_t bgp;
    uint8_t obp0;
    uint8_t obp1;
    // LY
    uint8_t ly;
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