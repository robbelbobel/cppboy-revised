#include "mmu.hpp"

MMU::MMU(PPU* ppu, Cartridge* cartridge, InputHandler* inputHandler){
    // Assign Private Class Pointers
    MMU::ppu            = ppu;
    MMU::cartridge      = cartridge;
    MMU::inputHandler   = inputHandler;

    // Initialize IO Registers
    MMU::write(0xFF00, 0xCF);   // JOYP
    MMU::write(0xFF01, 0x00);   // SB
    MMU::write(0xFF02, 0x7E);   // SC
    MMU::write(0xFF04, 0x18);   // DIV
    MMU::write(0xFF05, 0x00);   // TIMA
    MMU::write(0xFF06, 0x00);   // TMA
    MMU::write(0xFF07, 0xF8);   // TAC
    MMU::write(0xFF0F, 0xE1);   // IF
    MMU::write(0xFF10, 0x80);   // NR10
    MMU::write(0xFF11, 0xBF);   // NR11
    MMU::write(0xFF12, 0xF3);   // NR12
    MMU::write(0xFF13, 0xFF);   // NR13
    MMU::write(0xFF14, 0xBF);   // NR14
    MMU::write(0xFF16, 0x3F);   // NR21
    MMU::write(0xFF17, 0x00);   // NR22
    MMU::write(0xFF18, 0xFF);   // NR23
    MMU::write(0xFF19, 0xBF);   // NR24
    MMU::write(0xFF1A, 0x7F);   // NR30
    MMU::write(0xFF1B, 0xFF);   // NR31
    MMU::write(0xFF1C, 0x9F);   // NR32
    MMU::write(0xFF1D, 0xFF);   // NR33
    MMU::write(0xFF1E, 0xBF);   // NR34
    MMU::write(0xFF20, 0xFF);   // NR41
    MMU::write(0xFF21, 0x00);   // NR42
    MMU::write(0xFF22, 0x00);   // NR43
    MMU::write(0xFF23, 0xBF);   // NR44
    MMU::write(0xFF24, 0x77);   // NR50
    MMU::write(0xFF25, 0xF3);   // NR51
    MMU::write(0xFF26, 0xF1);   // NR52
    MMU::write(0xFF40, 0x91);   // LCDC
    MMU::write(0xFF41, 0x81);   // STAT
    MMU::write(0xFF42, 0x00);   // SCY
    MMU::write(0xFF43, 0x00);   // SCX
    MMU::write(0xFF45, 0x00);   // LYC
    MMU::write(0xFF46, 0xFF);   // DMA
    MMU::write(0xFF47, 0xFC);   // BGP
    MMU::write(0xFF48, 0xFF);   // OBP0
    MMU::write(0xFF49, 0xFF);   // OBP1
    MMU::write(0xFF4A, 0x00);   // WY
    MMU::write(0xFF4B, 0x00);   // WX
    MMU::write(0xFF4D, 0xFF);   // KEY1
    MMU::write(0xFF4F, 0xFF);   // VBK
    MMU::write(0xFF51, 0xFF);   // HDMA1
    MMU::write(0xFF52, 0xFF);   // HDMA2
    MMU::write(0xFF53, 0xFF);   // HDMA3
    MMU::write(0xFF54, 0xFF);   // HDMA4
    MMU::write(0xFF55, 0xFF);   // HDMA5
    MMU::write(0xFF56, 0xFF);   // RP
    MMU::write(0xFF68, 0xFF);   // BCPS
    MMU::write(0xFF69, 0xFF);   // BCPD
    MMU::write(0xFF6A, 0xFF);   // OCPS
    MMU::write(0xFF6B, 0xFF);   // OCPD
    MMU::write(0xFF70, 0xFF);   // SVBK
    MMU::write(0xFFFF, 0x00);   // IE
}

MMU::~MMU(){
    MMU::dump();
}

uint8_t MMU::ioRead(const uint16_t &address){
    switch(address){
        case 0xFF00:
            // JOYP
            MMU::inputHandler -> update();
            return MMU::inputHandler -> joyp;
            break;
        
        case 0xFF01:
            // SB
            break;
        
        case 0xFF02:
            // SC
            break;

        case 0xFF04:
            // DIV
            return MMU::div;
            break;
        
        case 0xFF05:
            // TIMA
            return MMU::tima;
            break;
        
        case 0xFF06:
            // TMA
            return MMU::tma;
            break;
        
        case 0xFF07:
            // TAC
            return MMU::tac;
            break;
        
        case 0xFF0F:{
            // IF
            uint8_t byte = 0;
            byte += MMU::ppu -> vBlankInt;
            return byte;
            break;
        }
        
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
        
        case 0xFF40:{
            // LCDC
            uint8_t byte = 0;
            byte += MMU::ppu -> bgWinEnable;
            byte += MMU::ppu -> objEnable   << 1;
            byte += MMU::ppu -> objSize     << 2;
            byte += MMU::ppu -> bgTMArea    << 3;
            byte += MMU::ppu -> bgWinTDArea << 4;
            byte += MMU::ppu -> winEnable   << 5;
            byte += MMU::ppu -> winTMArea   << 6;
            byte += MMU::ppu -> ppuEnable   << 7;
            return byte;
            break;
        }
        
        case 0xFF41:{
            // STAT
            uint8_t byte = 0;
            byte += MMU::ppu -> mode;
            byte += MMU::ppu -> lycFlag         << 2;
            byte += MMU::ppu -> hBlankIntSrc    << 3;
            byte += MMU::ppu -> vBlankIntSrc    << 4;
            byte += MMU::ppu -> oamIntSrc       << 5;
            byte += MMU::ppu -> lycIntSrc       << 6;
            return byte;
            break;
        }
        
        case 0xFF42:
            // SCY
            return MMU::ppu -> scy;
            break;
        
        case 0xFF43:
            // SCX
            return MMU::ppu -> scx;
            break;
        
        case 0xFF44:
            // LY
            return MMU::ppu -> ly;
            // return 0x90;
            break;
        
        case 0xFF45:
            // LYC
            return MMU::ppu -> lyc;
            break;
        
        case 0xFF46:
            // DMA
            break;
        
        case 0xFF47:
            // BGP
            return MMU::ppu -> bgp;
            break;
        
        case 0xFF48:
            // OBP0
            return MMU::ppu -> obp0;
            break;
        
        case 0xFF49:
            // OBP1
            return MMU::ppu -> obp1;
            break;
        
        case 0xFF4A:
            // WY
            return MMU::ppu -> wy;
            break;
        
        case 0xFF4B:
            // WX
            return MMU::ppu -> wx;
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

    // FOR DEV PURPOSES ONLY. SHOULD BE REMOVED AFTER IMPLEMENTING ALL IO READS
    return 0xFF;
}

void MMU::ioWrite(const uint16_t &address, const uint8_t &value){
    switch(address){
        case 0xFF00:{
            // JOYP
            MMU::inputHandler -> joyp &= ~(0b11 << 4);
            MMU::inputHandler -> joyp += (value & (0b11 << 4));
            break;
        }    
        
        case 0xFF01:{
            // SB
            break;
        }

        case 0xFF02:{
            // SC
            break;
        }

        case 0xFF04:{
            // DIV
            MMU::div = 0x00;
            break;
        }
        
        case 0xFF05:{
            // TIMA
            MMU::tima = value;
            break;
        }
        
        case 0xFF06:{
            // TMA
            MMU::tma = value;
            break;
        }
        
        case 0xFF07:{
            // TAC
            MMU::tac = value;
            break;
        }
        
        case 0xFF0F:{
            // IF
            MMU::ppu -> vBlankInt = value & 0b1;
            break;
        } 
        
        case 0xFF10:{
            // NR10
            break;
        }
        
        case 0xFF11:{
            // NR11
            break;
        }
        
        case 0xFF12:{
            // NR12
            break;
        }
        
        case 0xFF13:{
            // NR13
            break;
        }

        case 0xFF14:{
            // NR14
            break;
        }
        
        case 0xFF16:{
            // NR21
            break;
        }
        
        case 0xFF17:{
            // NR22
            break;
        }
        
        case 0xFF18:{
            // NR23
            break;
        }
        
        case 0xFF19:{
            // NR24
            break;
        }
        
        case 0xFF1A:{
            // NR30
            break;
        }

        case 0xFF1B:{
            // NR31
            break;
        }
        
        case 0xFF1C:{
            // NR32
            break;
        }
        
        case 0xFF1D:{
            // NR33
            break;
        }
        
        case 0xFF1E:{
            // NR34
            break;
        }
        
        case 0xFF20:{
            // NR41
            break;
        }
        
        case 0xFF21:{
            // NR42
            break;
        }
        
        case 0xFF22:{
            // NR43
            break;
        }
        
        case 0xFF23:{
            // NR44
            break;
        }
        
        case 0xFF24:{
            // NR50
            break;
        }
        
        case 0xFF25:{
            // NR51
            break;
        }
        
        case 0xFF26:{
            // NR52
            break;
        }
        
        case 0xFF40:{
            // LCDC
            MMU::ppu -> bgWinEnable = value & 0b1;
            MMU::ppu -> objEnable   = (value >> 1) & 0b1;
            MMU::ppu -> objSize     = (value >> 2) & 0b1;
            MMU::ppu -> bgTMArea    = (value >> 3) & 0b1;
            MMU::ppu -> bgWinTDArea = (value >> 4) & 0b1;
            MMU::ppu -> winEnable   = (value >> 5) & 0b1;
            MMU::ppu -> winTMArea   = (value >> 6) & 0b1;
            MMU::ppu -> ppuEnable   = (value >> 7) & 0b1;
            break;
        }
        
        case 0xFF41:{
            // STAT
            MMU::ppu -> hBlankIntSrc    = (value >> 3) & 0b1;
            MMU::ppu -> vBlankIntSrc    = (value >> 4) & 0b1;
            MMU::ppu -> oamIntSrc       = (value >> 5) & 0b1;
            MMU::ppu -> lycIntSrc       = (value >> 6) & 0b1;
            break;
        }
        
        case 0xFF42:{
            // SCY
            MMU::ppu -> scy = value;
            break;
        }
        
        case 0xFF43:{
            // SCX
            MMU::ppu -> scx = value;
            break;
        }
        
        case 0xFF45:{
            // LYC
            MMU::ppu -> lyc = value;
            break;
        }
        
        case 0xFF46:{
            // DMA
            uint16_t addr = value << 8;

            for(uint8_t i; i < 0xa0; i++){
                MMU::write(0xFE00 + i, MMU::read(addr + i));
            }

            break;
        }
        
        case 0xFF47:{
            // BGP
            MMU::ppu -> bgp = value;
            break;
        }
        
        case 0xFF48:{
            // OBP0
            MMU::ppu -> obp0 = value;
            break;
        }
        
        case 0xFF49:{
            // OBP1
            MMU::ppu -> obp1 = value;
            break;
        }
        
        case 0xFF4A:{
            // WY
            MMU::ppu -> wy = value;
            break;
        }
        
        case 0xFF4B:{
            // WX
            MMU::ppu -> wx = value;
            break;
        }
        
        case 0xFF4D:{
            // KEY1
            break;
        }
        
        case 0xFF4F:{
            // VBK
            break;
        }
        
        case 0xFF51:{
            // HDMA1
            break;
        }
        
        case 0xFF52:{
            // HDMA2
            break;
        }
        
        case 0xFF53:{
            // HDMA3
            break;
        }
        
        case 0xFF54:{
            // HDMA4
            break;
        }
        
        case 0xFF55:{
            // HDMA5
            break;
        }
        
        case 0xFF56:{
            // RP
            break;
        }
        
        case 0xFF68:{
            // BCPS
            break;
        }
        
        case 0xFF69:{
            // BCPD
            break;
        }
        
        case 0xFF6A:{
            // OCPS
            break;
        }
        
        case 0xFF6B:{
            // OCPD
            break;
        }
        
        case 0xFF70:{
            // SVBK
            break;
        }

        default:{
            break;
        }
    }
}

uint8_t MMU::read(const uint16_t &address){
    // Cartridge
    if(address < 0x8000){
        return MMU::cartridge -> read(address);
    }
    // VRAM
    if(address >= 0x8000 && address < 0xA000){
        return MMU::ppu -> vRAM[address - 0x8000];
    }
    // Ext. RAM
    if(address >= 0xA000 && address < 0xC000){
        return MMU::cartridge -> read(address);
    }
    // WRAM
    if(address >= 0xC000 && address < 0xE000){
        return MMU::wRAM[address - 0xC000];
    }
    // OAM
    if(address >= 0xFE00 && address < 0xFEA0){
        return MMU::ppu -> oam[address - 0xFE00];
    }
    // IO Regs
    if(address >= 0xFF00 && address < 0xFF80){
        return MMU::ioRead(address);
    }
    // HRAM
    if(address >= 0xFF80 && address < 0xFFFF){
        return MMU::hRAM[address - 0xFF80];
    }
    // IE
    if(address == 0xFFFF){
        return MMU::ie;
    }

    return 0xFF;
}

void MMU::write(const uint16_t &address, const uint8_t &value){
    // Cartridge
    if(address < 0x8000){
        MMU::cartridge -> write(address, value);
    }
    // VRAM
    if(address >= 0x8000 && address < 0xA000){
        MMU::ppu -> vRAM[address - 0x8000] = value;
    }
    // Ext. RAM
    if(address >= 0xA000 && address < 0xC000){
        MMU::cartridge -> write(address, value);
    }
    // WRAM
    if(address >= 0xC000 && address < 0xE000){
        MMU::wRAM[address - 0xC000] = value;
    }
    // OAM
    if(address >= 0xFE00 && address < 0xFEA0){
        MMU::ppu -> oam[address - 0xFE00] = value;
    }
    // IO Regs
    if(address >= 0xFF00 && address < 0xFF80){
        MMU::ioWrite(address, value);
    }
    // HRAM
    if(address >= 0xFF80 && address < 0xFFFF){
        MMU::hRAM[address - 0xFF80] = value;
    }
    // IE
    if(address == 0xFFFF){
        MMU::ie = value;
    }
}

void MMU::updateTimers(){
    // Increment DIV Register After 256 Steps
    if(MMU::divCounter == 256){
        MMU::div++;
        MMU::divCounter = 0;
    }

    // Increment divCounter
    MMU::divCounter++;

    // Increment TIMA If Timer Enable Is Set
    if((MMU::tac & 0b100) >> 2 != 1){
        return;
    }

    // Fetch Incrementation Speed
    uint16_t timerSpeed;

    switch(MMU::tac & 0b11){
        case 0x00:
            timerSpeed = 1024;
            break;
        
        case 0x01:
            timerSpeed = 16;
            break;
        
        case 0x10:  
            timerSpeed = 64;
            break;
        
        case 0x11:
            timerSpeed = 256;
            break;
        
        default:
            timerSpeed = 1024;
    }

    // Increment TIMA
    if(MMU::timaCounter == timerSpeed){
        MMU::tima++;
        MMU::timaCounter = 0;
    }

    // Increment TIMA Counter
    MMU::timaCounter++;
    
    // Check For TIMA Overflow
    if(MMU::tima == 0){
        MMU::tima = MMU::tma;
        // Trigger Interrupt Here
    }
}

void MMU::dump(){
    std::ofstream dumpStream;

    dumpStream.open("dump.bin", std::ios::binary | std::ios::out);

    dumpStream.seekp(0, std::ios::beg);

    char arr[0x10000];

    for(uint32_t i = 0; i <= 0xFFFF; i++){
        arr[i] = MMU::read(i);
    }

    dumpStream.write(arr, 0x10000);
    
    dumpStream.close();
}