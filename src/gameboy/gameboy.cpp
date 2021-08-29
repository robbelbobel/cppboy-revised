#include "gameboy.hpp"

Gameboy::Gameboy(){
    // Assign Private Class Pointers
    Gameboy::cartridge = new Cartridge;
    Gameboy::mmu = new MMU;
    Gameboy::cpu = new CPU(Gameboy::mmu);
}

Gameboy::~Gameboy(){
    // Delete All Dynamically Allocated Variables
    delete Gameboy::cpu;
}

void Gameboy::step(){
    
}

bool Gameboy::loadFile(char* path){
    return Gameboy::cartridge -> loadFile(path);
}