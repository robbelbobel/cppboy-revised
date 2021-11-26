#include "cpu.hpp"

CPU::CPU(MMU* mmu){
    // Assign Private Class Variables
    CPU::mmu = mmu;
    CPU::IME = 0;

    // Reset CPU
    CPU::reset();

}

CPU::~CPU(){
    ;
}

void CPU::checkInterrupts(){
    uint8_t intFlag     = CPU::mmu -> read(0xFF0F);
    uint8_t intEnable   = CPU::mmu -> read(0xFFFF);

    uint8_t res = intFlag & intEnable;

    if((res & 0b1) == 1){
        // VBlank Interrupt
        CPU::IME = 0;
        intFlag &= ~0b1;
        CPU::mmu -> write(0xFF0F, intFlag);
        CPU::INTCALL(0x40);
        return;
    }
    if(((res >> 1) & 0b1) == 1){
        // STAT Interrupt
    }
    if(((res >> 2) & 0b1) == 1){
        // Timer Interrupt
    }
    if(((res >> 3) & 0b1) == 1){
        // Serial Interrupt
    }
    if(((res >> 4) & 0b1) == 1){
        // Joypad Interrupt
    }
}

void CPU::reset(){
    // Initialize CPU Flags
    CPU::A = 0x01;
    CPU::F = 0xB0;
    CPU::B = 0xFF;
    CPU::C = 0x13;
    CPU::D = 0x00;
    CPU::E = 0xC1;
    CPU::H = 0x84;
    CPU::L = 0x03;

    CPU::PC = 0x0100;
    CPU::SP = 0xFFFE;

    // Reset Class Variables
    CPU::cycleCount = 0;
}

void CPU::step(){
    // Fetch Interrupt If IME Is Set
    if(CPU::IME){
        CPU::checkInterrupts();
    }

    // Update Timer Registers
    CPU::mmu -> updateTimers();

    // Execute Instruction If Cycle Count Is 0
    if(CPU::cycleCount == 0){
        CPU::execute(CPU::mmu -> read(CPU::PC));
        // std::cout << std::hex << std::setfill('0') << std::uppercase << "A: " << std::setw(2) << (int) CPU::A << " F: " << std::setw(2) << (int) CPU::F << " B: " << std::setw(2) << (int) CPU::B  << " C: " << std::setw(2) << (int) CPU::C << " D: " << std::setw(2) << (int) CPU::D << " E: " << std::setw(2) << (int) CPU::E << " H: " << std::setw(2) << (int) CPU::H << " L: " << std::setw(2) << (int) CPU::L << " SP: " << std::setw(4) << (int) CPU::SP << " PC: 00:" << std::setw(4) << (int) CPU::PC <<  " (" << std::setw(2) << (int) CPU::mmu -> read(CPU::PC) << " " << std::setw(2) << (int) CPU::mmu -> read(CPU::PC + 1) << " " << std::setw(2) << (int) CPU::mmu -> read(CPU::PC + 2) << " " << std::setw(2) << (int) CPU::mmu -> read(CPU::PC + 3) << ')' << std::endl;
    }

    // Decrement Cycle Count
    CPU::cycleCount--;
}