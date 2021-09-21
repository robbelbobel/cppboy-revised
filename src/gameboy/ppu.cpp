#include "ppu.hpp"

PPU::PPU(){
    // Initialize PPU Variables
    PPU::cycleCount = 0;
    // Intialize Pixel Colors
    PPU::pixelColors[0] = sf::Color(255, 255, 255);
    PPU::pixelColors[1] = sf::Color(169,169,169);
    PPU::pixelColors[2] = sf::Color(128, 128, 128);
    PPU::pixelColors[3] = sf::Color(0, 0, 0);
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
    
    // Draw Layers To Window
    for(unsigned int y = 0; y < 144; y++){
        for(unsigned int x = 0; x < 160; x++){
            // Calculate Position Of Pixel
            pixelShape.setPosition(sf::Vector2f(x * pxSize, y * pxSize));
            
            // Determine Color Of Pixel
            if(PPU::objLayer[16 + y][8 + x] != 4){
                pixelShape.setFillColor(PPU::pixelColors[PPU::objLayer[16 + y][8 + x]]);
            }else{
                pixelShape.setFillColor(PPU::pixelColors[PPU::bgLayer[y][x]]);
            }

            // Draw Pixel To Window
            window.draw(pixelShape);
        }
    }

    // Display Pixels
    window.display();
}

void PPU::hBlank(){
    // Decrement Cycle Count
    PPU::cycleCount--;

    if(PPU::cycleCount == 0){
        // Increment LY
        PPU::ly++;

        // Update PPU Mode
        PPU::mode = PPU::ly >= 145 ? PPU_VBLANK : PPU_OAM_SEARCH;
    }
}

void PPU::vBlank(){
    // Enable VBlank Interrupt And Initialize Cycle Count
    if(PPU::cycleCount == 0){
        PPU::vBlankInt  = 1;
        PPU::cycleCount = 4560;
    }

    // Decrement Cycle Count
    PPU::cycleCount--;

    // Update PPU Mode
    if(PPU::cycleCount == 0){
        PPU::ly     = 0;
        PPU::mode   = PPU_OAM_SEARCH;
        return;
    }
    
    // Update Line Every 456 Cycles
    if(PPU::cycleCount % 456 == 0){
        PPU::ly++;
    }
}

void PPU::oamSearch(){
    // Initialize Cycle Count
    if(PPU::cycleCount == 0){
        PPU::cycleCount = 456;

        // Check For Visible Object
        for(uint8_t i = 0; i < 40; i++){
            // Check If Object Is Present On Current Line
            PPU::objArray[i] = (PPU::oam[i * 4] - 16) <= PPU::ly && (PPU::oam[i * 4] - 16 + (8 * (1 + PPU::objSize))) > PPU::ly;
        }
    }

    // Decrement Cycle Count
    PPU::cycleCount--;

    // Update PPU Mode
    if(PPU::cycleCount == 376){
        PPU::mode = PPU_PIXEL_TRANSFER;
    }
}

void PPU::pixelTransfer(){
    // Transfer Pixels For Current Line
    if(PPU::cycleCount == 376){
        // Transfer Background And Window
        if(PPU::bgWinEnable){
            // Get Background Tile Map Address
            uint16_t bgTMAddr = PPU::bgTMArea ? 0x1C00 : 0x1800;

            // Get Background And Window Tile Data Address
            uint16_t bgWinTDAddr = PPU::bgWinTDArea ? 0x0 : 0x800;

            // Transfer Tiles To Background Layer
            for(uint8_t i = 0; i < 21; i++){
                // Calculate Index Of Tile
                uint8_t index = PPU::vRAM[bgTMAddr + ((uint8_t)(i + PPU::scx) + (32 * (uint8_t)((PPU::ly - PPU::scy) / 8)))];

                // Fetch Bytes From Tile Data
                uint8_t byte1 = PPU::vRAM[bgWinTDAddr + index * 16 + ((PPU::ly % 8) * 2)];
                uint8_t byte2 = PPU::vRAM[bgWinTDAddr + index * 16 + ((PPU::ly % 8) * 2) + 1];

                // Store Pixel Color ID In Background Layer
                for(uint8_t j = 0; j < 8; j++){
                    PPU::bgLayer[PPU::ly - PPU::scy][(uint8_t) ((i * 8) + j + PPU::scx)] = (PPU::bgp >> (((((byte2 >> (7 - j)) & 0b1) << 1) + ((byte1 >> (7 - j)) & 0b1)) * 2)) & 0b11;
                }
            }
        }else{
            // Fill Background With White Pixels
            for(uint16_t x = 0; x < 256; x++){
                for(uint16_t y = 0; y < 256; y++){
                    PPU::bgLayer[y][x] = 0;
                }
            }
        }

        // Clear Current Line Of The Object Layer
        for(uint16_t x = 0; x < 176; x++){
            PPU::objLayer[16 + PPU::ly][x] = 4; 
        }

        // Transfer Objects
        if(PPU::objEnable){
            // Loop Through Objects
            for(uint8_t i = 0; i < 40; i++){
                // Check If Object Is Visible
                if(!PPU::objArray[i]){
                    continue;
                }

                // Fetch Object Tile Index
                uint8_t index = PPU::oam[i * 4 + 2];

                // Fetch Bytes From Tile Data
                uint8_t byte1 = PPU::vRAM[index * 16 + ((PPU::ly - (PPU::oam[i * 4] - 16)) * 2)];
                uint8_t byte2 = PPU::vRAM[index * 16 + ((PPU::ly - (PPU::oam[i * 4] - 16)) * 2) + 1];
                
                // Determine Object Palette
                uint8_t palette = PPU::obp1;
                if(((PPU::oam[(i * 4) + 3] >> 4) & 0b1) == 0){
                    palette = PPU::obp0;
                }
                
                // Draw Visible Pixels
                for(uint8_t j = 0; j < 8; j++){
                    // Determine Color Index
                    uint8_t colorIndex = ((((byte2 >> (7 - j)) & 0b1) << 1) + ((byte1 >> (7 - j)) & 0b1));

                    // Store Color Index In Object Layer
                    if(colorIndex != 0){
                        PPU::objLayer[16 + PPU::ly][PPU::oam[(i * 4) + 1] + j] = (palette >> (2 * colorIndex)) & 0b11;
                    }
                }
            }
        }
    }

    // Decrement Pixel Counter
    PPU::cycleCount--;
    
    // Update PPU Mode
    if(PPU::cycleCount == 208){
        PPU::mode = PPU_HBLANK;
    }
}

void PPU::step(){
    // Continue If PPU Is Enabled
    if(!PPU::ppuEnable){
        return;
    }

    // Execute PPU Function
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