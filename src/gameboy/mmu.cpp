#include "mmu.hpp"

MMU::MMU(){
    ;
}

MMU::~MMU(){
    ;
}

uint8_t MMU::read(const uint16_t &address){
    // Cartridge
    if(address < 0x8000){
        return MMU::cartridge -> read(address);
    }
    // VRAM
    if(address >= 0x8000 && address < 0xA000){
        ;
    }
    // Ext. RAM
    if(address >= 0xA000 && address < 0xC000){
        ;
    }
    // WRAM
    if(address >= 0xC000 && address < 0xE000){
        ;
    }
    // OAM
    if(address >= 0xFE00 && address < 0xFEA0){
        ;
    }
    // IO Regs
    if(address >= 0xFF00 && address < 0xFF80){
        ;
    }
    // HRAM
    if(address >= 0xFF80 && address < 0xFFFF){
        ;
    }
    // IE
    if(address == 0xFFFF){
        ;
    }

    return 0xFF;
}

void MMU::write(const uint16_t &address, const uint8_t &value){
    // VRAM
    if(address >= 0x8000 && address < 0xA000){
        ;
    }
    // Ext. RAM
    if(address >= 0xA000 && address < 0xC000){
        ;
    }
    // WRAM
    if(address >= 0xC000 && address < 0xE000){
        ;
    }
    // OAM
    if(address >= 0xFE00 && address < 0xFEA0){
        ;
    }
    // IO Regs
    if(address >= 0xFF00 && address < 0xFF80){
        ;
    }
    // HRAM
    if(address >= 0xFF80 && address < 0xFFFF){
        ;
    }
    // IE
    if(address == 0xFFFF){
        ;
    }
}