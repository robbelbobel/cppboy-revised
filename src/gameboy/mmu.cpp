#include "mmu.hpp"

MMU::MMU(PPU* ppu, Cartridge* cartridge){
    MMU::ppu        = ppu;
    MMU::cartridge  = cartridge;
}

MMU::~MMU(){
    ;
}

uint8_t MMU::ioRead(const uint16_t &address){
    switch(address){
        case 0xFF00:
            // JOYP
            break;
        
        case 0xFF01:
            // SB
            break;
        
        case 0xFF02:
            // SC
            break;

        case 0xFF04:
            // DIV
            break;
        
        case 0xFF05:
            // TIMA
            break;
        
        case 0xFF06:
            // TMA
            break;
        
        case 0xFF07:
            // TAC
            break;
        
        case 0xFF08:
            // IF
            break;
        
        case 0xFF10:
            // NR10
            break;
        
        case 0xFF11:
            // NR11
            break;
        
        case 0xFF12:
            // NR12
            break;

        case 0xFF14:
            // NR14
            break;
        
        case 0xFF16:
            // NR21
            break;
        
        case 0xFF17:
            // NR22
            break;
        
        case 0xFF19:
            // NR24
            break;
        
        case 0xFF1A:
            // NR30
            break;
        
        case 0xFF1C:
            // NR32
            break;
        
        case 0xFF1E:
            // NR34
            break;
        
        case 0xFF21:
            // NR42
            break;
        
        case 0xFF22:
            // NR43
            break;
        
        case 0xFF23:
            // NR44
            break;
        
        case 0xFF24:
            // NR50
            break;
        
        case 0xFF25:
            // NR51
            break;
        
        case 0xFF26:
            // NR52
            break;
        
        case 0xFF40:
            // LCDC
            break;
        
        case 0xFF41:
            // STAT
            break;
        
        case 0xFF42:
            // SCY
            break;
        
        case 0xFF43:
            // SCX
            break;
        
        case 0xFF44:
            // LY
            break;
        
        case 0xFF45:
            // LYC
            break;
        
        case 0xFF46:
            // DMA
            break;
        
        case 0xFF47:
            // BGP
            break;
        
        case 0xFF48:
            // OBP0
            break;
        
        case 0xFF49:
            // OBP1
            break;
        
        case 0xFF4A:
            // WY
            break;
        
        case 0xFF4B:
            // WX
            break;
        
        case 0xFF4D:
            // KEY1
            break;
        
        case 0xFF4F:
            // VBK
            break;
        
        case 0xFF56:
            // RP
            break;
        
        case 0xFF68:
            // BCPS
            break;
        
        case 0xFF69:
            // BCPD
            break;
        
        case 0xFF6A:
            // OCPS
            break;
        
        case 0xFF6B:
            // OCPD
            break;
        
        case 0xFF70:
            // SVBK
            break;

        default:
            return 0xFF;
            break;
    }

    // FOR DEV PURPOSES ONLY. SHOULD BE REMOVED
    return 0xFF;
}

void MMU::ioWrite(const uint16_t &address, const uint8_t &value){
    switch(address){
        case 0xFF00:
            // JOYP
            break;
        
        case 0xFF01:
            // SB
            break;
        
        case 0xFF02:
            // SC
            break;

        case 0xFF04:
            // DIV
            break;
        
        case 0xFF05:
            // TIMA
            break;
        
        case 0xFF06:
            // TMA
            break;
        
        case 0xFF07:
            // TAC
            break;
        
        case 0xFF08:
            // IF
            break;
        
        case 0xFF10:
            // NR10
            break;
        
        case 0xFF11:
            // NR11
            break;
        
        case 0xFF12:
            // NR12
            break;
        
        case 0xFF13:
            // NR13
            break;

        case 0xFF14:
            // NR14
            break;
        
        case 0xFF16:
            // NR21
            break;
        
        case 0xFF17:
            // NR22
            break;
        
        case 0xFF18:
            // NR23
            break;
        
        case 0xFF19:
            // NR24
            break;
        
        case 0xFF1A:
            // NR30
            break;
        
        case 0xFF1B:
            // NR31
            break;
        
        case 0xFF1C:
            // NR32
            break;
        
        case 0xFF1D:
            // NR33
            break;
        
        case 0xFF1E:
            // NR34
            break;
        
        case 0xFF20:
            // NR41
            break;
        
        case 0xFF21:
            // NR42
            break;
        
        case 0xFF22:
            // NR43
            break;
        
        case 0xFF23:
            // NR44
            break;
        
        case 0xFF24:
            // NR50
            break;
        
        case 0xFF25:
            // NR51
            break;
        
        case 0xFF26:
            // NR52
            break;
        
        case 0xFF40:
            // LCDC
            break;
        
        case 0xFF41:
            // STAT
            break;
        
        case 0xFF42:
            // SCY
            break;
        
        case 0xFF43:
            // SCX
            break;
        
        case 0xFF45:
            // LYC
            break;
        
        case 0xFF46:
            // DMA
            break;
        
        case 0xFF47:
            // BGP
            break;
        
        case 0xFF48:
            // OBP0
            break;
        
        case 0xFF49:
            // OBP1
            break;
        
        case 0xFF4A:
            // WY
            break;
        
        case 0xFF4B:
            // WX
            break;
        
        case 0xFF4D:
            // KEY1
            break;
        
        case 0xFF4F:
            // VBK
            break;
        
        case 0xFF51:
            // HDMA1
            break;
        
        case 0xFF52:
            // HDMA2
            break;
        
        case 0xFF53:
            // HDMA3
            break;
        
        case 0xFF54:
            // HDMA4
            break;
        
        case 0xFF55:
            // HDMA5
            break;
        
        case 0xFF56:
            // RP
            break;
        
        case 0xFF68:
            // BCPS
            break;
        
        case 0xFF69:
            // BCPD
            break;
        
        case 0xFF6A:
            // OCPS
            break;
        
        case 0xFF6B:
            // OCPD
            break;
        
        case 0xFF70:
            // SVBK
            break;

        default:
            break;
    }
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