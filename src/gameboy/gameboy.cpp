#include "gameboy.hpp"

Gameboy::Gameboy(){
    Gameboy::cartridge = new Cartridge;
    Gameboy::mmu = new MMU(Gameboy::cartridge);
    Gameboy::cpu = new CPU(Gameboy::mmu);
}

Gameboy::~Gameboy(){
    delete Gameboy::cpu;
    delete Gameboy::mmu;
    delete Gameboy::cartridge;
}

void Gameboy::step(){
    Gameboy::cpu -> step();
}

bool Gameboy::loadFile(char* path){
    return Gameboy::cartridge -> loadFile(path);
}