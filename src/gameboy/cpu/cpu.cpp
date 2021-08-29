#include "cpu.hpp"

CPU::CPU(MMU* mmu){
    // Assign Private Class Pointers
    CPU::mmu = mmu;

    // Reset CPU
    CPU::reset();

}

CPU::~CPU(){
    ;
}

void CPU::reset(){
    // Initialize CPU Flags
    CPU::A = 0x01;
    CPU::F = 0x00;
    CPU::B = 0xFF;
    CPU::C = 0x13;
    CPU::D = 0x00;
    CPU::E = 0xC1;
    CPU::H = 0x84;
    CPU::L = 0x03;

    CPU::PC = 0x0100;
    CPU::SP = 0xFFFE;
}

void CPU::step(){

}