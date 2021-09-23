#include "cartridge.hpp"

Cartridge::Cartridge(){
    // Initialize Class Variables
    Cartridge::ramEnable    = 0x00;
    Cartridge::romNumber    = 0x01;
    Cartridge::ramNumber    = 0x00;
    Cartridge::bankMode     = 0x00;
}

Cartridge::~Cartridge(){

}

void Cartridge::loadHeader(){
    // Load Title
    for(uint8_t i = 0; i < 16; i++){
        Cartridge::title[i] = Cartridge::data[0x134 + i];
    }
    
    // Load Manufacturer Code
    for(uint8_t i = 0; i < 4; i++){
        Cartridge::manCode[i] = Cartridge::data[0x13F + i];
    }

    // Load Type
    Cartridge::type = Cartridge::data[0x147];

    // Load ROM Size
    Cartridge::romSize = Cartridge::data[0x148];

    // Load RAM Size
    Cartridge::ramSize = Cartridge::data[0x149];
}

void Cartridge::updateROMBank(){
    // ROM Number Can't Be 0
    if(Cartridge::romNumber == 0){
        return;
    }

    for(uint16_t i = 0; i < 0x4000; i++){
        Cartridge::romBank1[i] = Cartridge::data[(Cartridge::romNumber * 0x4000) + i];
    }
}

void Cartridge::updateRAMBank(){
    // Copy Current Ram Bank In Its Ram Slot
    for(uint16_t i; i < 0x2000; i++){
        Cartridge::ramSlots[Cartridge::ramNumber][i] = Cartridge::ramBank[i]; 
    }
    
    // Copy Current Ram Slot To Ram Bank
    for(uint16_t i; i < 0x2000; i++){
        Cartridge::ramBank[i] = Cartridge::ramSlots[Cartridge::ramNumber][i]; 
    }
}

bool Cartridge::loadFile(char* path){
    // Create File Stream
    std::ifstream gameStream;
    
    // Open File
    gameStream.open(path, std::ios::binary | std::ios::in);

    // Check If File Loading Succeeded
    if(gameStream.fail()){
        return false;
    }

    // Calculate File Length
    gameStream.seekg(0, std::ios::end);
    int end = gameStream.tellg();

    gameStream.seekg(0, std::ios::beg);
    int begin = gameStream.tellg();

    // Read File
    gameStream.read(Cartridge::data, end - begin);

    // Close File
    gameStream.close();

    // Load Header From Cartridge Data
    Cartridge::loadHeader();

    // Initialize ROM Bank 0
    for(uint16_t i = 0; i < 0x4000; i++){
        Cartridge::romBank0[i] = Cartridge::data[i];
    }
    
    // Initialize ROM Bank 1
    Cartridge::updateROMBank();

    return true;
}

uint8_t Cartridge::read(const uint16_t &address){
    // ROM Bank 0 Read
    if(address < 0x4000){
        return Cartridge::romBank0[address];
    }
    // ROM Bank 1 Read
    if(address >= 0x4000 && address < 0x8000){
        return Cartridge::romBank1[address - 0x4000];
    }
    // RAM Bank Read
    if(address >= 0xA000 && address < 0xC000){
        return Cartridge::ramBank[address - 0xA000];
    }

    return 0x00;
}

void Cartridge::write(const uint16_t &address, const uint8_t &value){
    std::cout << "Cartridge write" << std::endl;

    // RAM Enable Register
    if(address < 0x2000){
        Cartridge::ramEnable = value;
        return;
    }
    
    // ROM Bank Number Register
    if(address >= 0x2000 && address < 0x4000){
        // Mask Only Relevant Bits
        Cartridge::romNumber = ((value & 0b11111) & ((0b1 << (Cartridge::romSize + 1)) - 1));
        
        return;
    }

    // RAM Bank Number Or Upper Bits For ROM Bank Number Register
    if(address >= 0x4000 && address < 0x6000){
        // Bank Mode 0
        if(Cartridge::bankMode == 0){
            if(Cartridge::romSize > 4){
                // Use This Register For Bit 6 & 7 Of ROM Bank Number Register
                Cartridge::romNumber += (value & 0b11) << 5;
            }
        }
        // Bank Mode 1
        if(Cartridge::bankMode == 1){
            if(Cartridge::romSize > 4){
                // Use This Register For Bit 6 & 7 Of ROM Bank Number Register
                Cartridge::romNumber += (value & 0b11) << 5;
            }else{
                // Use This Register For RAM Bank Number Register
                Cartridge::ramNumber = value & 0b11;
            }
        }
        return;
    }
    
    // Bank Mode Select
    if(address >= 0x6000 && address < 0x8000){
        Cartridge::bankMode = value & 0b1;
    } 

    // Update ROM/RAM Banks
    Cartridge::updateROMBank();
    Cartridge::updateRAMBank();
}