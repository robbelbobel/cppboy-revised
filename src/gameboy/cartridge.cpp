#include "cartridge.hpp"

Cartridge::Cartridge(){
    // for(uint16_t i = 0; i < 0x8000; i++){
    //     Cartridge::data[i] = 0;
    // }
}

Cartridge::~Cartridge(){

}

bool Cartridge::loadFile(char* path){
    // Create File Stream
    std::ifstream gameStream;
    
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

    gameStream.close();

    return true;
}

uint8_t Cartridge::read(const uint16_t &address){
    return Cartridge::data[address];
}