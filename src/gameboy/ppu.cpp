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