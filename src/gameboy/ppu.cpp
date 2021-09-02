#include "ppu.hpp"

PPU::PPU(){
    // Initialize PPU Variables
    PPU::cycleCount = 0;
    // Initialize Read-Only Registers
    PPU::ly         = 0x91;
    PPU::mode       = 1;
    PPU::lycFlag    = 0;
}

PPU::~PPU(){

}

void PPU::draw(sf::RenderWindow &window){
    // Clear Window
    window.clear(sf::Color(0, 0, 0, 255));

    // Create Pixel Shape
    sf::RectangleShape pixelShape;

    // Calculate Pixel Size
    sf::Vector2u winSize = window.getSize();
    int pxSize = winSize.y / 144;

    // Set Pixel Size
    pixelShape.setSize(sf::Vector2f(pxSize, pxSize));
    
    for(unsigned int y = 0; y < 144; y++){
        for(unsigned int x = 0; x < 160; x++){
            pixelShape.setPosition(sf::Vector2f(x * pxSize, y * pxSize));
            if(PPU::pixelArray[y][x]){
                pixelShape.setFillColor(sf::Color(0, 0, 0, 255));
            }else{
                pixelShape.setFillColor(sf::Color(255, 255, 255, 255));
            }
            window.draw(pixelShape);
        }
    }

    window.display();
}

void PPU::hBlank(){
    PPU::cycleCount--;

    if(PPU::cycleCount == 0){
        PPU::ly++;

        if(PPU::ly >= 145){
            PPU::mode = PPU_VBLANK;
        }else{
            PPU::mode = PPU_OAM_SEARCH;
        }
    }
}

void PPU::vBlank(){
    if(PPU::cycleCount == 0){
        PPU::vBlankInt  = 1;
        PPU::cycleCount = 4560;
    }

    PPU::cycleCount--;

    if(PPU::cycleCount == 0){
        PPU::ly     = 0;
        PPU::mode   = PPU_OAM_SEARCH;
        return;
    }
    
    if(PPU::cycleCount % 456 == 0){
        PPU::ly++;
    }
}

void PPU::oamSearch(){
    if(PPU::cycleCount == 0){
        PPU::cycleCount = 456;
    }

    PPU::cycleCount--;

    if(PPU::cycleCount == 376){
        PPU::mode = PPU_PIXEL_TRANSFER;
    }
}

void PPU::pixelTransfer(){
    if(PPU::cycleCount == 376){
        PPU::pxPtr = 0;
        
        uint16_t bgTMAddr;

        if(PPU::bgTMArea){
            bgTMAddr = 0x1C00;
        }else{
            bgTMAddr = 0x1800;
        }

        uint16_t bgWinTDAddr;
        
        if(PPU::bgWinTDArea){
            bgWinTDAddr = 0x0;
        }else{
            bgWinTDAddr = 0x800;
        }

        for(uint8_t i = 0; i < 20; i++){
            uint8_t index = PPU::vRAM[bgTMAddr + i + (32 * (uint8_t)(PPU::ly / 8))];

            uint8_t byte1 = PPU::vRAM[bgWinTDAddr + index * 16 + ((PPU::ly % 8) * 2)];
            uint8_t byte2 = PPU::vRAM[bgWinTDAddr + index * 16 + ((PPU::ly % 8) * 2) + 1];

            pixelArray[PPU::ly][(i * 8) + 0] = (((byte2 >> 7) & 0b1) << 1) + ((byte1 >> 7) & 0b1);
            pixelArray[PPU::ly][(i * 8) + 1] = (((byte2 >> 6) & 0b1) << 1) + ((byte1 >> 6) & 0b1);
            pixelArray[PPU::ly][(i * 8) + 2] = (((byte2 >> 5) & 0b1) << 1) + ((byte1 >> 5) & 0b1);
            pixelArray[PPU::ly][(i * 8) + 3] = (((byte2 >> 4) & 0b1) << 1) + ((byte1 >> 4) & 0b1);
            pixelArray[PPU::ly][(i * 8) + 4] = (((byte2 >> 3) & 0b1) << 1) + ((byte1 >> 3) & 0b1);
            pixelArray[PPU::ly][(i * 8) + 5] = (((byte2 >> 2) & 0b1) << 1) + ((byte1 >> 2) & 0b1);
            pixelArray[PPU::ly][(i * 8) + 6] = (((byte2 >> 1) & 0b1) << 1) + ((byte1 >> 1) & 0b1);
            pixelArray[PPU::ly][(i * 8) + 7] = ((byte2 & 0b1) << 1) + (byte1 & 0b1);
        }
    }

    PPU::cycleCount--;
    
    if(PPU::cycleCount == 208){
        PPU::mode = PPU_HBLANK;
    }
}

void PPU::step(){
    if(!PPU::ppuEnable){
        return;
    }

    switch(PPU::mode){
        case PPU_HBLANK:
            PPU::hBlank();
            break;
        
        case PPU_VBLANK:
            PPU::vBlank();
            break;
        
        case PPU_OAM_SEARCH:
            PPU::oamSearch();
            break;
        
        case PPU_PIXEL_TRANSFER:
            PPU::pixelTransfer();
            break;
        
        default:    
            break;
    }
}