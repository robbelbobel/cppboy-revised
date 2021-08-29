#include "cartridge.hpp"

Cartridge::Cartridge(){

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
    gameStream.read((char*) Cartridge::data, end - begin);

    gameStream.close();

    return true;
}