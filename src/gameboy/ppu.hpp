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
    void render(sf::RenderWindow &window);

    void hBlank();
    void vBlank(sf::RenderWindow &window);
    void oamSearch();
    void pixelTransfer();

public:
    // Public Class Functions
    PPU();
    ~PPU();

    void step(sf::RenderWindow &window);

    // Public Class Variables
    uint16_t cycleCount;
    uint8_t pxPtr;
    uint8_t pixelArray[144][160];
    // Memory Arrays
    uint8_t vRAM[0x2000];
    uint8_t oam[0xA0];
    // Interrupt
    bool vBlankInt;
    // Registers
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