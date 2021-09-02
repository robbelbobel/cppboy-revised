#include "gameboy.hpp"

Gameboy::Gameboy(){
    Gameboy::ppu        = new PPU;
    Gameboy::cartridge  = new Cartridge;
    Gameboy::mmu        = new MMU(Gameboy::ppu, Gameboy::cartridge);
    Gameboy::cpu        = new CPU(Gameboy::mmu);
}

Gameboy::~Gameboy(){
    delete Gameboy::cpu;
    delete Gameboy::mmu;
    delete Gameboy::cartridge;
}

void Gameboy::step(sf::RenderWindow &window){
    Gameboy::cpu -> step();
    Gameboy::ppu -> step(window);
}

bool Gameboy::loadFile(char* path){
    return Gameboy::cartridge -> loadFile(path);
}