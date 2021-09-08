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
    
    // Draw Pixel Array To Window
    for(unsigned int y = 0; y < 144; y++){
        for(unsigned int x = 0; x < 160; x++){
            // Calculate Position Of Pixel
            pixelShape.setPosition(sf::Vector2f(x * pxSize, y * pxSize));
            
            // Determine Color Of Pixel
            pixelShape.setFillColor(PPU::pixelColors[(PPU::bgp >> (PPU::pixelArray[16 + y][8 + x] * 2)) & 0b11]);

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

            // Transfer Visible Tiles To Pixel Array
            for(uint8_t i = 0; i < 20; i++){
                // Calculate Index Of Tile
                uint8_t index = PPU::vRAM[bgTMAddr + i + (32 * (uint8_t)(PPU::ly / 8))];

                // Fetch Bytes From Tile Data
                uint8_t byte1 = PPU::vRAM[bgWinTDAddr + index * 16 + ((PPU::ly % 8) * 2)];
                uint8_t byte2 = PPU::vRAM[bgWinTDAddr + index * 16 + ((PPU::ly % 8) * 2) + 1];

                // Store Pixel Color ID In Pixel Array
                PPU::pixelArray[16 + PPU::ly][8 + (i * 8) + 0] = (((byte2 >> 7) & 0b1) << 1) + ((byte1 >> 7) & 0b1);
                PPU::pixelArray[16 + PPU::ly][8 + (i * 8) + 1] = (((byte2 >> 6) & 0b1) << 1) + ((byte1 >> 6) & 0b1);
                PPU::pixelArray[16 + PPU::ly][8 + (i * 8) + 2] = (((byte2 >> 5) & 0b1) << 1) + ((byte1 >> 5) & 0b1);
                PPU::pixelArray[16 + PPU::ly][8 + (i * 8) + 3] = (((byte2 >> 4) & 0b1) << 1) + ((byte1 >> 4) & 0b1);
                PPU::pixelArray[16 + PPU::ly][8 + (i * 8) + 4] = (((byte2 >> 3) & 0b1) << 1) + ((byte1 >> 3) & 0b1);
                PPU::pixelArray[16 + PPU::ly][8 + (i * 8) + 5] = (((byte2 >> 2) & 0b1) << 1) + ((byte1 >> 2) & 0b1);
                PPU::pixelArray[16 + PPU::ly][8 + (i * 8) + 6] = (((byte2 >> 1) & 0b1) << 1) + ((byte1 >> 1) & 0b1);
                PPU::pixelArray[16 + PPU::ly][8 + (i * 8) + 7] = ((byte2 & 0b1) << 1) + (byte1 & 0b1);
            }
        }else{
            // Fill Background With White Pixels
            for(uint8_t x = 0; x < 160; x++){
                for(uint8_t y = 0; y < 144; y++){
                    PPU::pixelArray[y][x] = 0;
                }
            }
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

                // Store Object Pixel Color ID In Pixel Array
                PPU::pixelArray[16 + PPU::ly][PPU::oam[i * 4 + 1]] = (((byte2 >> 7) & 0b1) << 1) + ((byte1 >> 7) & 0b1);
                PPU::pixelArray[16 + PPU::ly][PPU::oam[i * 4 + 1] + 1] = (((byte2 >> 6) & 0b1) << 1) + ((byte1 >> 6) & 0b1);
                PPU::pixelArray[16 + PPU::ly][PPU::oam[i * 4 + 1] + 2] = (((byte2 >> 5) & 0b1) << 1) + ((byte1 >> 5) & 0b1);
                PPU::pixelArray[16 + PPU::ly][PPU::oam[i * 4 + 1] + 3] = (((byte2 >> 4) & 0b1) << 1) + ((byte1 >> 4) & 0b1);
                PPU::pixelArray[16 + PPU::ly][PPU::oam[i * 4 + 1] + 4] = (((byte2 >> 3) & 0b1) << 1) + ((byte1 >> 3) & 0b1);
                PPU::pixelArray[16 + PPU::ly][PPU::oam[i * 4 + 1] + 5] = (((byte2 >> 2) & 0b1) << 1) + ((byte1 >> 2) & 0b1);
                PPU::pixelArray[16 + PPU::ly][PPU::oam[i * 4 + 1] + 6] = (((byte2 >> 1) & 0b1) << 1) + ((byte1 >> 1) & 0b1);
                PPU::pixelArray[16 + PPU::ly][PPU::oam[i * 4 + 1] + 7] = ((byte2 & 0b1) << 1) + (byte1 & 0b1);
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