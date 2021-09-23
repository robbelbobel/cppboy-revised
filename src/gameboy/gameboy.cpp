#include "gameboy.hpp"

Gameboy::Gameboy(){
    Gameboy::ppu            = new PPU;
    Gameboy::cartridge      = new Cartridge;
    Gameboy::inputHandler   = new InputHandler;
    Gameboy::mmu            = new MMU(Gameboy::ppu, Gameboy::cartridge, Gameboy::inputHandler);
    Gameboy::cpu            = new CPU(Gameboy::mmu);
}

Gameboy::~Gameboy(){
    delete Gameboy::cpu;
    delete Gameboy::mmu;
    delete Gameboy::cartridge;
}

void Gameboy::step(){
    Gameboy::cpu -> step();
    Gameboy::ppu -> step();
}

void Gameboy::draw(sf::RenderWindow &window){
    Gameboy::ppu -> draw(window);
}

bool Gameboy::loadFile(char* path){
    return Gameboy::cartridge -> loadFile(path);
}

void Gameboy::setWindowTitle(sf::RenderWindow &window){
    window.setTitle(std::string("CPPBOY: ") + Gameboy::cartridge -> title);
}