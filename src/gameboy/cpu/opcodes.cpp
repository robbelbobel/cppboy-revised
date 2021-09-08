#include "cpu.hpp"

// Flag Functions
void CPU::setFlag(uint8_t flag){
    CPU::F |= (0b1 << flag);                                                // Set Flag
}

void CPU::clearFlag(uint8_t flag){
    CPU::F &= ~(0b1 << flag);                                               // Clear Flag
}

bool CPU::getFlag(uint8_t flag){
    return((CPU::F >> flag) & 0b1);                                         // Return Flag
}

// LD Operations
void CPU::LD(uint8_t *reg1a, uint8_t *reg1b, uint8_t d16a, uint8_t d16b){
    *reg1a = d16a;                                                          // Assign Reg1a To d16a
    *reg1b = d16b;                                                          // Assign Reg1b to d16b


    CPU::cycleCount = 12;                                                   // Set CycleCount
}

void CPU::LD(uint8_t *reg, uint8_t d8){
    *reg = d8;                                                              // Assign Reg To d8

    CPU::cycleCount = 8;                                                    // Set CycleCount
}

void CPU::LD(uint8_t addr1a, uint8_t addr1b, uint8_t d8){
    CPU::mmu -> write((((uint16_t) addr1a) << 8) + addr1b, d8);             // Store d8 At Address Addr

    CPU::cycleCount = 8;                                                    // Set CycleCount
}

void CPU::LD(uint8_t *reg, uint8_t addr1a, uint8_t addr1b){
    *reg = CPU::mmu -> read((((uint16_t) addr1a) << 8) + addr1b);           // Assign Reg To Byte At Address Addr

    CPU::cycleCount = 8;                                                    // Set CycleCount
} 

void CPU::LD(uint16_t *SP, uint8_t d16a, uint8_t d16b){
    *SP = (d16a << 8) + d16b;                                               // Assign Stack Pointer To d16

    CPU::cycleCount = 12;                                                   // Set CycleCount
}

void CPU::LD(uint8_t a8, uint8_t reg){
    CPU::mmu -> write(0xFF00 + a8, reg);                                    // Write Reg To Address Addr

    CPU::cycleCount = 8;                                                    // Set CycleCount
}

void CPU::LDH(uint8_t a8, uint8_t reg){
    CPU::mmu -> write(0xFF00 + a8, reg);                                    // Write Reg To Address 0xFF00 + a8

    CPU::cycleCount = 12;                                                   // Set CycleCount
}

void CPU::LDH(uint8_t *reg, uint8_t a8){
    *reg = CPU::mmu -> read(0xFF00 + a8);                                   // Assign Reg To The Byte At Address 0xFF00 + a8

    CPU::cycleCount = 12;                                                   // Set CycleCount
}

// INC Operations
void CPU::INC(uint8_t *reg){
    CPU::clearFlag(FLAG_N);                                                 // Clear Substract Flag

    if((((((*reg &0xF) + 1) & 0xF0) >> 4) & 0b1) == 0b1){
        CPU::setFlag(FLAG_H);                                               // Set H Flag If A Half Carry Occured
    }else{
        CPU::clearFlag(FLAG_H);                                             // Else Clear The H Flag
    }

    *reg += 1;                                                              // Increment Reg

    if(*reg == 0){
        CPU::setFlag(FLAG_Z);                                               // Set Zero Flag If Result Is 0
    }else{
        CPU::clearFlag(FLAG_Z);                                             // Else Clear Zero Flag
    }

    CPU::cycleCount = 4;                                                    // Set CycleCount
}

void CPU::INC(uint8_t *reg1a, uint8_t *reg1b){
    uint16_t reg = (((uint16_t) *reg1a) << 8) + *reg1b;                     // Merge The Bytes
    reg++;                                                                  // Increment Reg
    *reg1a = (reg & 0xFF00) >> 8;                                           // Assign Reg1a
    *reg1b = reg & 0xFF;                                                    // Assign Reg1b

    CPU::cycleCount = 8;                                                    // Set CycleCount
}

void CPU::INC(uint8_t addr1a, uint8_t addr1b){
    uint8_t num = CPU::mmu -> read((((uint16_t) addr1a) << 8) + addr1b);    // Read The Byte From Memory

    CPU::clearFlag(FLAG_N);                                                 // Clear Substract Flag

    if((((((num &0xF) + 1) & 0xF0) >> 4) & 0b1) == 0b1){
        CPU::setFlag(FLAG_H);                                               // Set H Flag If A Half Carry Occured
    }else{
        CPU::clearFlag(FLAG_H);                                             // Else Clear The H Flag
    }

    num++;                                                                  // Increment The Byte
    
    if(num == 0){
        CPU::setFlag(FLAG_Z);                                               // Set Zero Flag If Result Is Zero
    }else{
        CPU::clearFlag(FLAG_Z);                                             // Else Clear Flag
    }

    CPU::mmu -> write((((uint16_t) addr1a) << 8) + addr1b, num);            // Write Num To Memory

    CPU::cycleCount = 12;                                                   // Set CycleCount
}

void CPU::INC(uint16_t* SP){
    *SP += 1;                                                               // Increment Stack Pointer

    CPU::cycleCount = 8;                                                    // Set CycleCount
}

// DEC Operations
void CPU::DEC(uint8_t *reg){
    CPU::setFlag(FLAG_N);                                                   // Set Substract Flag

    if((((((*reg &0xF) - 1) & 0xF0) >> 4) & 0b1) == 0b1){
        CPU::setFlag(FLAG_H);                                               // Set H Flag If A Half Carry Occured
    }else{
        CPU::clearFlag(FLAG_H);                                             // Else Clear The H Flag
    }

    *reg -= 1;                                                              // Decrement Reg

    if(*reg == 0){
        CPU::setFlag(FLAG_Z);                                               // Set Zero Flag If Result Is Zero
    }else{  
        CPU::clearFlag(FLAG_Z);                                             // Else Clear Zero FLag
    }

    CPU::cycleCount = 4;                                                    // Set CycleCount
}

void CPU::DEC(uint8_t *reg1a, uint8_t *reg1b){
    uint16_t reg = (((uint16_t) *reg1a) << 8) + *reg1b;                     // Merge The Bytes

    reg--;                                                                  // Decrement Reg

    *reg1a = (reg & 0xFF00) >> 8;                                           // Assign Reg1a
    *reg1b = reg & 0xFF;                                                    // Assign Reg1b

    CPU::cycleCount = 8;                                                    // Set CycleCount
}

void CPU::DEC(uint8_t addr1a, uint8_t addr1b){
    uint8_t num = CPU::mmu -> read((((uint16_t) addr1a) << 8) + addr1b); // Read The Byte From Memory

    CPU::setFlag(FLAG_N);                                                   // Set Substract Flag

    if((((((num &0xF) - 1) & 0xF0) >> 4) & 0b1) == 0b1){
        CPU::setFlag(H);                                                    // Set H Flag If A Half Carry Occurs
    }else{
        CPU::clearFlag(H);                                                  // Else Clear H Flag
    }

    num--;                                                                  // Decrement Num

    if(num == 0){
        CPU::setFlag(FLAG_Z);                                               // Set Zero Flag If Result is Zero
    }else{
        CPU::clearFlag(FLAG_Z);                                             // Else Clear Zero Flag
    }

    CPU::mmu -> write((((uint16_t) addr1a) << 8) + addr1b, num);            // Write Num To Address Addr

    CPU::cycleCount = 12;                                                    // Set CycleCount
}

void CPU::DEC(uint16_t *SP){
    *SP -= 1;                                                               // Decrement Stack Pointer

    CPU::cycleCount = 8;                                                    // Set CycleCount
}

// ADD Operations
void CPU::ADD(uint8_t *reg1a, uint8_t *reg1b, uint8_t reg2a, uint8_t reg2b){
    CPU::clearFlag(FLAG_N);                                                 // Clear Substract Flag

    uint16_t reg1 = (((uint16_t) *reg1a) << 8) + *reg1b;                    // Merge Reg1
    uint16_t reg2 = (((uint16_t) reg2a) << 8) + reg2b;                      // Merge Reg2

    if(((((reg1 & 0xFF) + (reg2 & 0xFF)) >> 8) & 0b1) == 0b1){
        CPU::setFlag(FLAG_H);                                               // Set Half Carry Flag If Half Carry Occurs
    }else{
        CPU::clearFlag(FLAG_H);                                             // Else Clear Half Carry Flag
    }

    if((((((uint32_t) reg1) + reg2) >> 16) & 0b1) == 0b1){
        CPU::setFlag(FLAG_C);                                               // Set Carry Flag If Carry Occurs
    }else{
        CPU::clearFlag(FLAG_C);                                             // Else Clear Carry Flag
    }

    reg1 += reg2;                                                           // Add Reg2 To Reg1

    *reg1a = (reg1 & 0xFF00) >> 8;                                          // Assign Reg1a
    *reg1b = reg1 & 0xFF;                                                   // Assign Reg1b

    CPU::cycleCount = 8;                                                    // Set CycleCount
}

void CPU::ADD(uint8_t *reg, uint8_t d8){
    CPU::clearFlag(FLAG_N);                                                 // Substract Clear Flag

    if(((((((uint16_t) *reg) &0xF) + d8) >> 4) & 0b1) == 0b1){ 
        CPU::setFlag(FLAG_H);                                               // Set Half Carry Flag If Half Carry Occured
    }else{
        CPU::clearFlag(FLAG_H);                                             // Else Clear Half Carry Flag
    }
    
    if((((((uint16_t) *reg) + d8) >> 8) & 0b1) == 0b1){ 
        CPU::setFlag(FLAG_C);                                               // Set Carry Flag If Carry Occured
    }else{
        CPU::clearFlag(FLAG_C);                                             // Else Clear Carry Flag
    }

    *reg += d8;                                                             // Add d8 To Reg

    if(*reg == 0){
        CPU::setFlag(FLAG_Z);                                               // Set Zero Flag If Result Is Zero
    }else{
        CPU::clearFlag(FLAG_Z);                                             // Else Clear Zero Flag
    }

    CPU::cycleCount = 4;                                                    // Set CycleCount
}

void CPU::ADD(uint8_t *reg, uint8_t addr1a, uint8_t addr1b){
    CPU::clearFlag(FLAG_N);                                                 // Clear Substraction Flag

    uint16_t addr = ((uint16_t) addr1a << 8) + addr1b;                      // Merge Addr
    
    uint8_t num = CPU::mmu -> read(addr);                                   // Fetch Num From Memory

    if(((((((uint16_t) *reg) &0xF) + num) >> 4) & 0b1) == 0b1){ 
        CPU::setFlag(FLAG_H);                                               // Set Half Carry Flag If Half Carry Occured
    }else{
        CPU::clearFlag(FLAG_H);                                             // Else Clear Half Carry Flag
    }

    if((((((uint16_t) *reg) + num) >> 8) & 0b1) == 0b1){ 
        CPU::setFlag(FLAG_C);                                               // Set Carry Flag If Carry Occured
    }else{
        CPU::clearFlag(FLAG_C);                                             // Else Clear Carry Flag
    }

    *reg += num;                                                            // Add Num To Reg

    if(*reg == 0){
        CPU::setFlag(FLAG_Z);                                               // Set Zero Flag If Result Is 0
    }else{
        CPU::clearFlag(FLAG_Z);                                             // Else Clear Zero Flag
    }

    CPU::cycleCount = 8;                                                    // Set CycleCount
}

void CPU::ADD(uint8_t *reg1a, uint8_t *reg1b, uint16_t SP){
    uint16_t reg1 = ((uint16_t) *reg1a << 8) + *reg1b;                      // Merge Reg1

    if(((((reg1 &0xFF) + SP) >> 8) & 0b1) == 0b1){ 
        CPU::setFlag(FLAG_H);                                               // Set Half Carry Flag If Half Carry Occured
    }else{
        CPU::clearFlag(FLAG_H);                                             // Else Clear Half Carry Flag
    }

    if((((((uint32_t) reg1) + SP) >> 16) & 0b1) == 0b1){ 
        CPU::setFlag(FLAG_C);                                               // Set Carry Flag If Carry Occured
    }else{
        CPU::clearFlag(FLAG_C);                                             // Else Clear Carry Flag
    }

    CPU::clearFlag(FLAG_N);                                                 // Clear Substract Flag

    uint16_t res = reg1 + SP;                                               // Add SP To Reg1

    *reg1a = (res >> 8);                                                    // Assign Reg1a
    *reg1b = res & 0xFF;                                                    // Assign Reg1b

    CPU::cycleCount = 8;                                                    // Set CycleCount
}

void CPU::ADD(uint16_t* SP, int8_t s8){
    CPU::clearFlag(FLAG_Z);                                                 // Clear Zero Flag 
    CPU::clearFlag(FLAG_N);                                                 // Clear Substract Flag

    if(((((*SP & 0xFF) + s8) >> 8) & 0b1) == 0b1){ 
        CPU::setFlag(FLAG_H);                                               // Set Half Carry Flag If Half Carry Occured
    }else{
        CPU::clearFlag(FLAG_H);                                             // Else Clear Half Carry Flag
    }

    if((((((uint32_t) *SP) + s8) >> 16) & 0b1) == 0b1){ 
        CPU::setFlag(FLAG_C);                                               // Set Carry Flag If Carry Occured
    }else{
        CPU::clearFlag(FLAG_C);                                             // Else Clear Carry Flag
    }

    *SP += s8;                                                              // Add s8 To SP

    CPU::cycleCount = 16;
}

void CPU::ADC(uint8_t reg){
    
}

void CPU::ADC(uint8_t addr1a, uint8_t addr1b){

}

// SUB Operations
void CPU::SUB(uint8_t reg){

}

void CPU::SUB(uint8_t addr1a, uint8_t addr1b){
    
}

void CPU::SBC(uint8_t reg){

}

void CPU::SBC(uint8_t addr1a, uint8_t addr1b){

}

// AND Operations
void CPU::AND(uint8_t reg){
    CPU::A &= reg;                                                          // Logic AND A and Reg And Store Result In A

    if(CPU::A == 0){
        CPU::setFlag(FLAG_Z);                                               // Set Zero Flag if Result Is 0
    }else{
        CPU::clearFlag(FLAG_Z);                                             // Else Clear Zero Flag
    }

    CPU::setFlag(FLAG_H);                                                   // Set Half Carry Flag
    CPU::clearFlag(FLAG_C);                                                 // Clear Carry Flag
    CPU::clearFlag(FLAG_N);                                                 // Clear Substract FLag

    CPU::cycleCount = 4;                                                    // Set CycleCount
}

void CPU::AND(uint8_t addr1a, uint8_t addr1b){
    uint16_t addr = ((uint16_t) addr1a << 8) + addr1b;                      // Merge Addr

    CPU::A &= CPU::mmu -> read(addr);                                       // Logic AND A And Byte At Addr And Store Result In A

    if(CPU::A == 0){
        CPU::setFlag(FLAG_Z);                                               // Set Zero Flag If Result Is 0
    }else{
        CPU::clearFlag(FLAG_Z);                                             // Else Clear Zero Flag
    }

    CPU::setFlag(FLAG_H);                                                   // Set Half Carry Flag
    CPU::clearFlag(FLAG_C);                                                 // Clear Carry Flag
    CPU::clearFlag(FLAG_N);                                                 // Clear Substract Flag

    CPU::cycleCount = 8;                                                    // Set CycleCount
}

// XOR Operations
void CPU::XOR(uint8_t reg){
    CPU::A ^= reg;                                                          // Logic XOR A And Reg And Store The Result In A

    if(CPU::A == 0){
        CPU::setFlag(FLAG_Z);                                               // Set Zero Flag If Result Is 0
    }else{
        CPU::clearFlag(FLAG_Z);                                             // Else Clear Zero Flag
    }

    CPU::clearFlag(FLAG_H);                                                 // Clear Half Carry Flag
    CPU::clearFlag(FLAG_C);                                                 // Clear Carry Flag
    CPU::clearFlag(FLAG_N);                                                 // Clear Substract Flag

    CPU::cycleCount = 4;                                                    // Set CycleCount
}

void CPU::XOR(uint8_t addr1a, uint8_t addr1b){
    uint16_t addr = ((uint16_t) addr1a << 8) + addr1b;                      // Merge Addr

    CPU::A ^= CPU::mmu -> read(addr);                                       // Logic XOR A And Reg And Store The Result In A

    if(CPU::A == 0){
        CPU::setFlag(FLAG_Z);                                               // Set Zero Flag If Result Is 0
    }else{
        CPU::clearFlag(FLAG_Z);                                             // Else Clear Zero Flag
    }

    CPU::clearFlag(FLAG_H);                                                 // Clear Half Carry Flag
    CPU::clearFlag(FLAG_C);                                                 // Clear Carry Flag
    CPU::clearFlag(FLAG_N);                                                 // Clear Substract Flag

    CPU::cycleCount = 8;                                                    // Set CycleCount
}

// OR Operations
void CPU::OR(uint8_t reg){
    CPU::A |= reg;                                                          // Logic OR A And Reg And Store The Result In A

    if(CPU::A == 0){
        CPU::setFlag(FLAG_Z);                                               // Set Zero Flag If Result Is 0
    }else{
        CPU::clearFlag(FLAG_Z);                                             // Else Clear Zero Flag
    }

    CPU::clearFlag(FLAG_H);                                                 // Clear Half Carry Flag
    CPU::clearFlag(FLAG_C);                                                 // Clear Carry Flag
    CPU::clearFlag(FLAG_N);                                                 // Clear Substract Flag

    CPU::cycleCount = 4;                                                    // Set CycleCount
}

void CPU::OR(uint8_t addr1a, uint8_t addr1b){
    uint16_t addr = ((uint16_t) addr1a << 8) + addr1b;                      // Merge Addr

    CPU::A |= CPU::mmu -> read(addr);                                       // Logic OR A And Reg And Store The Result In A

    if(CPU::A == 0){
        CPU::setFlag(FLAG_Z);                                               // Set Zero Flag If Result Is 0
    }else{
        CPU::clearFlag(FLAG_Z);                                             // Else Clear Zero Flag
    }

    CPU::clearFlag(FLAG_H);                                                 // Clear Half Carry Flag
    CPU::clearFlag(FLAG_C);                                                 // Clear Carry Flag
    CPU::clearFlag(FLAG_N);                                                 // Clear Substract Flag

    CPU::cycleCount = 8;                                                    // Set CycleCount
}

// CP Operations
void CPU::CP(uint8_t reg){
    CPU::setFlag(FLAG_N);                                                   // Set Substract Flag
    
    if((CPU::A - reg) == 0){
        CPU::setFlag(FLAG_Z);                                               // Set Z Flag If Reg Is Equal To A
    }else{
        CPU::clearFlag(FLAG_Z);                                             // Else Clear Z Flag
    }

    if((((CPU::A & 0xF) - (reg & 0xF)) >> 4) != 0b0){
        CPU::setFlag(FLAG_H);                                               // Set H Flag If No Half Carry Ocurred
    }else{
        CPU::clearFlag(FLAG_H);                                             // Else Clear H Flag
    }

    if(CPU::A < reg){
        CPU::setFlag(FLAG_C);                                               // Set Carry Flag If A Is Less Than Reg
    }else{
        CPU::clearFlag(FLAG_C);                                             // Else Clear Carry Flag
    }

    CPU::cycleCount = 4;
}

void CPU::CP(uint8_t addr1a, uint8_t addr1b){
    CPU::setFlag(FLAG_N);                                                   // Set Substract Flag
    
    uint16_t addr = ((uint16_t) addr1a << 8) + addr1b;                      // Merge Addr

    if((CPU::A - CPU::mmu -> read(addr)) == 0){
        CPU::setFlag(FLAG_Z);                                               // Set Z Flag If Byte At Addr Is Equal To A
    }else{
        CPU::clearFlag(FLAG_Z);                                             // Else Clear Z Flag
    }

    if((((CPU::A & 0xF) - (CPU::mmu -> read(addr) & 0xF)) >> 4) != 0b0){
        CPU::setFlag(FLAG_H);                                               // Set H Flag If No Half Carry Ocurred
    }else{
        CPU::clearFlag(FLAG_H);                                             // Else Clear H Flag
    }

    if(CPU::A < CPU::mmu -> read(addr)){
        CPU::setFlag(FLAG_C);                                               // Set Carry Flag If A Is Less Than Reg
    }else{
        CPU::clearFlag(FLAG_C);                                             // Else Clear Carry Flag
    }

    CPU::cycleCount = 8;
}

// JP Operations
void CPU::JP(uint8_t a16a, uint8_t a16b){
    uint16_t addr = ((uint16_t) a16a << 8) + a16b;                          // Merge Addr

    CPU::PC = addr;                                                         // Set Program Counter Equal To Addr

    CPU::cycleCount = 4;                                                    // Set CycleCount
}

void CPU::JP(uint8_t flag, uint8_t a16a, uint8_t a16b){
    if(CPU::getFlag(flag)){
        uint16_t addr = ((uint16_t) a16a << 8) + a16b;                      // Merge Addr

        CPU::PC = addr;                                                     // Set Program Counter Equal To Addr

        CPU::cycleCount = 16;                                               // Set CycleCount

    }else{
        CPU::PC += 3;                                                       // Increment Program Counter

        CPU::cycleCount = 12;                                               // Set CycleCount
    }
}

void CPU::JPN(uint8_t flag, uint8_t a16a, uint8_t a16b){
    if(!CPU::getFlag(flag)){
        uint16_t addr = ((uint16_t) a16a << 8) + a16b;                      // Merge Addr

        CPU::PC = addr;                                                     // Set Program Counter Equal To Addr

        CPU::cycleCount = 16;                                               // Set CycleCount

    }else{
        CPU::PC += 3;                                                       // Increment Program Counter

        CPU::cycleCount = 12;                                               // Set CycleCount
    }
}

// JR Operations
void CPU::JR(int8_t s8){
    CPU::PC += s8;                                                          // Add s8 To Program Counter

    CPU::cycleCount = 4;                                                    // Set CycleCount
}

void CPU::JR(uint8_t flag, int8_t s8){
    if(CPU::getFlag(flag)){
        CPU::PC += s8;                                                      // Add s8 To Program Counter

        CPU::cycleCount = 16;                                               // Set CycleCount
    }else{
        CPU::cycleCount = 12;                                               // Set CycleCount
    }
}

void CPU::JRN(uint8_t flag, int8_t s8){
    if(!CPU::getFlag(flag)){
        CPU::PC += s8;                                                      // Add s8 To Program Counter

        CPU::cycleCount = 16;                                               // Set CycleCount
    }else{
        CPU::cycleCount = 12;                                               // Set CycleCount
    }
}

// CALL Operations
void CPU::INTCALL(uint8_t a8){
    CPU::SP -= 2;                                                           // Decrement Stack Pointer

    CPU::mmu -> write(CPU::SP, (CPU::PC) & 0xFF);                           // Write Least Significant Byte
    CPU::mmu -> write(CPU::SP + 1, ((CPU::PC) >> 8) & 0xFF);                // Write Most Significant Byte

    CPU::PC = a8;                                                           // Set Program Counter To Addr

    CPU::cycleCount = 12;                                                   // Set CycleCount
}

void CPU::CALL(uint8_t a16a, uint8_t a16b){
    uint16_t addr = ((uint16_t) a16a << 8) + a16b;                          // Merge Addr

    CPU::SP -= 2;                                                           // Decrement Stack Pointer

    CPU::PC += 3;                                                           // Set PC To Next Instruction

    CPU::mmu -> write(CPU::SP, (CPU::PC) & 0xFF);                           // Write Least Significant Byte
    CPU::mmu -> write(CPU::SP + 1, ((CPU::PC) >> 8) & 0xFF);                // Write Most Significant Byte

    CPU::PC = addr;                                                         // Set Program Counter To Addr

    CPU::cycleCount = 12;                                                   // Set CycleCount
}

void CPU::CALL(uint8_t flag, uint8_t a16a, uint8_t a16b){
    if(CPU::getFlag(flag)){
        uint16_t addr = ((uint16_t) a16a << 8) + a16b;                      // Merge Addr
        
        CPU::SP -= 2;                                                       // Decrement Stack Pointer
        
        CPU::PC += 3;                                                       // Set PC To Next Instruction

        CPU::mmu -> write(CPU::SP, (CPU::PC) & 0xFF);                       // Write Least Significant Byte
        CPU::mmu -> write(CPU::SP + 1, ((CPU::PC) >> 8) & 0xFF);            // Write Most Significant Byte

        CPU::PC = addr;                                                     // Set Program Counter To Addr

        CPU::cycleCount = 24;                                               // Set CycleCount
    
    }else{
        CPU::PC += 3;                                                       // Add To Program Counter

        CPU::cycleCount = 12;
    }
}

void CPU::CALLN(uint8_t flag, uint8_t a16a, uint8_t a16b){
    if(!CPU::getFlag(flag)){
        uint16_t addr = ((uint16_t) a16a << 8) + a16b;                      // Merge Addr

        CPU::SP -= 2;                                                       // Decrement Stack Pointer
        
        CPU::PC += 3;                                                       // Set PC To Next Instruction

        CPU::mmu -> write(CPU::SP, (CPU::PC) & 0xFF);                       // Write Least Significant Byte
        CPU::mmu -> write(CPU::SP + 1, ((CPU::PC) >> 8) & 0xFF);            // Write Most Significant Byte

        CPU::PC = addr;                                                     // Set Program Counter To Addr

        CPU::cycleCount = 24;                                               // Set CycleCount
    
    }else{
        CPU::PC += 3;                                                       // Add To Program Counter

        CPU::cycleCount = 12;                                               // Set CycleCount
    }
}

// RET Operations
void CPU::RET(){
    // Fetch New Address From Memory
    uint16_t address = (((uint16_t) CPU::mmu -> read(CPU::SP + 1)) << 8) + CPU::mmu -> read(CPU::SP);

    CPU::SP += 2;                                                           // Increment Stack Pointer

    CPU::PC = address;                                                      // Set Program Counter To Address

    CPU::cycleCount = 4;                                                    // Set CycleCount
}

void CPU::RET(uint8_t flag){
    if(CPU::getFlag(flag)){
        // Fetch New Address From Memory
        uint16_t address = (((uint16_t) CPU::mmu -> read(CPU::SP + 1)) << 8) + CPU::mmu -> read(CPU::SP);

        CPU::SP += 2;                                                       // Increment Stack Pointer

        CPU::PC = address;                                                  // Set Program Counter To Address

        CPU::cycleCount = 20;                                               // Set CycleCount
    }else{
        CPU::PC += 1;                                                       // Increment Program Counter

        CPU::cycleCount = 8;                                                // Set CycleCount
    }
}

void CPU::RETN(uint8_t flag){
    if(!CPU::getFlag(flag)){
        // Fetch New Address From Memory
        uint16_t address = (((uint16_t) CPU::mmu -> read(CPU::SP + 1)) << 8) + CPU::mmu -> read(CPU::SP);

        CPU::SP += 2;                                                       // Increment Stack Pointer

        CPU::PC = address;                                                  // Set Program Counter To Address

        CPU::cycleCount = 20;                                               // Set CycleCount
    }else{
        CPU::PC += 1;                                                       // Increment Program Counter

        CPU::cycleCount = 8;                                                // Set CycleCount
    }
}

void CPU::RETI(){
    CPU::IME = 1;

    // Fetch New Address From Memory
    uint16_t address = (((uint16_t) CPU::mmu -> read(CPU::SP + 1)) << 8) + CPU::mmu -> read(CPU::SP);

    CPU::SP += 2;                                                           // Increment Stack Pointer

    CPU::PC = address;                                                      // Set Program Counter To Address


    CPU::cycleCount = 16;
}

// Stack Related Operations
void CPU::POP(uint8_t* reg1a, uint8_t* reg1b){
    *reg1a = CPU::mmu -> read(CPU::SP + 1);                                 // Load Most Significant Byte
    *reg1b = CPU::mmu -> read(CPU::SP);                                     // Load Less Significant Byte

    CPU::SP += 2;                                                           // Increment Stack Pointer

    CPU::cycleCount = 12;                                                   // Set CycleCount
}

void CPU::PUSH(uint8_t reg1a, uint8_t reg1b){
    CPU::SP -= 2;
    
    CPU::mmu -> write(CPU::SP, reg1b);                                      // Write Least Significant Byte
    CPU::mmu -> write(CPU::SP + 1, reg1a);                                  // Write Most Significant Byte

    CPU::cycleCount = 16;                                                   // Set CycleCount
}

// Shift Operations
void CPU::SLA(uint8_t *reg){
    if((*reg >> 7) == 0b1){
        CPU::setFlag(FLAG_C);                                               // Set Carry Flag If Bit 7 Is 1
    }else{
        CPU::clearFlag(FLAG_C);                                             // Else Clear Carry Flag
    }

    *reg = *reg << 1;                                                       // Bit Shift Reg To The Left

    if(*reg == 0){
        CPU::setFlag(FLAG_Z);                                               // Set Zero Flag If Result Is 0
    }else{
        CPU::clearFlag(FLAG_Z);                                             // Else Clear Flag
    }

    CPU::clearFlag(FLAG_N);                                                 // Clear Substract Flag
    CPU::clearFlag(FLAG_H);                                                 // Clear Half Carry Flag

    CPU::cycleCount = 8;                                                    // Set Cyclec Count
}

void CPU::SLA(uint8_t addr1a, uint8_t addr1b){
    uint16_t addr = ((uint16_t) addr1a << 8) + addr1b;                      // Merge Addr
    
    uint8_t n = CPU::mmu -> read(addr);                                     // Read Byte From Address

    if((n >> 7) == 0b1){
        CPU::setFlag(FLAG_C);                                               // Set Carry Flag If Bit 7 Is 1
    }else{
        CPU::clearFlag(FLAG_C);                                             // Else Clear Carry Flag
    }

    n = n << 1;                                                             // Bit Shift n To The Left

    if(n == 0){
        CPU::setFlag(FLAG_Z);                                               // Set Zero Flag If Result Is 0
    }else{
        CPU::clearFlag(FLAG_Z);                                             // Else Clear Flag
    }

    CPU::clearFlag(FLAG_N);                                                 // Clear Substract Flag
    CPU::clearFlag(FLAG_H);                                                 // Clear Half Carry Flag

    CPU::mmu -> write(addr, n);                                             // Write Byte Back To Addr
    
    CPU::cycleCount = 16;                                                   // Set Cycle Count
}

void CPU::SRA(uint8_t *reg){
    if((*reg & 0b1) == 0b1){
        CPU::setFlag(FLAG_C);                                               // Set Carry Flag If Bit 0 Is 1
    }else{
        CPU::clearFlag(FLAG_C);                                             // Else Clear Carry Flag
    }

    *reg = *reg >> 1;                                                       // Bit Shift Reg To The Right

    if(*reg == 0){
        CPU::setFlag(FLAG_Z);                                               // Set Zero Flag If Result Is 0
    }else{
        CPU::clearFlag(FLAG_Z);                                             // Else Clear Flag
    }

    CPU::clearFlag(FLAG_N);                                                 // Clear Substract Flag
    CPU::clearFlag(FLAG_H);                                                 // Clear Half Carry Flag

    *reg |= (CPU::getFlag(FLAG_C) << 7);                                    // Set 7th Bit Equal To Carry Flag

    CPU::cycleCount = 8;                                                    // Set Cycle Count
}

void CPU::SRA(uint8_t addr1a, uint8_t addr1b){
    uint16_t addr = ((uint16_t) addr1a << 8) + addr1b;                      // Merge Addr
    
    uint8_t n = CPU::mmu -> read(addr);                                     // Read Byte From Address

    if((n & 0b1) == 0b1){
        CPU::setFlag(FLAG_C);                                               // Set Carry Flag If Bit 0 Is 1
    }else{
        CPU::clearFlag(FLAG_C);                                             // Else Clear Carry Flag
    }

    n = n >> 1;                                                             // Bit Shift n To The Right

    if(n == 0){
        CPU::setFlag(FLAG_Z);                                               // Set Zero Flag If Result Is 0
    }else{
        CPU::clearFlag(FLAG_Z);                                             // Else Clear Flag
    }

    CPU::clearFlag(FLAG_N);                                                 // Clear Substract Flag
    CPU::clearFlag(FLAG_H);                                                 // Clear Half Carry Flag

    n |= (CPU::getFlag(FLAG_C) << 7);                                       // Set 7th Bit Equal To Carry Flag

    CPU::mmu -> write(addr, n);                                             // Write Byte Back To Addr

    CPU::cycleCount = 16;                                                   // Set Cycle Count
}

void CPU::SRL(uint8_t *reg){

}

void CPU::SRL(uint8_t addr1a, uint8_t addr1b){

}

// SWAP Operations
void CPU::SWAP(uint8_t *reg){

}

void CPU::SWAP(uint8_t addr1a, uint8_t addr1b){

}

// BIT Operations
void CPU::BIT(uint8_t bitNr, uint8_t reg){

}

void CPU::BIT(uint8_t bitNr, uint8_t addr1a, uint8_t addr1b){

}

// RES Operations
void CPU::RES(uint8_t bitNr, uint8_t* reg){
    *reg &= ~(0b1 << bitNr);                                                // Reset Bit bitNr Of Register To 0

    CPU::cycleCount = 8;                                                    // Set Cycle Count
}

void CPU::RES(uint8_t bitNr, uint8_t addr1a, uint8_t addr1b){
    uint16_t addr = ((uint16_t) addr1a << 8) + addr1b;                      // Merge Addr
    
    uint8_t n = CPU::mmu -> read(addr);                                     // Read Byte From Address

    n &= ~(0b1 << bitNr);                                                   // Reset Bit bitNr Of Byte At Addr To 0

    CPU::mmu -> write(addr, n);                                             // Write Byte Back To Addr

    CPU::cycleCount = 16;                                                   // Set Cycle Count
}

// SET Operations
void CPU::SET(uint8_t bitNr, uint8_t* reg){

}

void CPU::SET(uint8_t bitNr, uint8_t addr1a, uint8_t addr1b){

}

// Rotate Operations
void CPU::RLC(uint8_t *reg){

}

void CPU::RLC(uint8_t addr1a, uint8_t addr1b){

}

void CPU::RRC(uint8_t *reg){

}

void CPU::RRC(uint8_t addr1a, uint8_t addr1b){

}

void CPU::RL(uint8_t *reg){

}

void CPU::RL(uint8_t addr1a, uint8_t addr1b){

}

void CPU::RR(uint8_t *reg){

}

void CPU::RR(uint8_t addr1a, uint8_t addr1b){

}

// Misc. Operations
void CPU::NOP(){
    CPU::cycleCount = 4;    
}

void CPU::STOP(){
    // CPU::stopMode = true;
    CPU::cycleCount = 4;
}

void CPU::HALT(){
    // CPU::haltMode = true;
    CPU::cycleCount = 4;
}

void CPU::DAA(){

}

void CPU::CPL(){
    CPU::setFlag(FLAG_N);
    CPU::setFlag(FLAG_H);

    CPU::A = ~CPU::A;

    CPU::cycleCount = 4;
}

void CPU::SCF(){
    CPU::setFlag(FLAG_C);
    CPU::clearFlag(FLAG_N);
    CPU::clearFlag(FLAG_H);

    CPU::cycleCount = 4;
}

void CPU::CCF(){
    if(CPU::getFlag(FLAG_C)){
        CPU::clearFlag(FLAG_C);
    }else{
        CPU::setFlag(FLAG_C);
    }

    CPU::clearFlag(FLAG_N);
    CPU::clearFlag(FLAG_H);

    CPU::cycleCount = 4;
}

void CPU::DI(){
    CPU::IME = 0;

    CPU::cycleCount = 4;
}

void CPU::EI(){
    CPU::IME = 1;

    CPU::cycleCount = 4;
}

void CPU::RST(uint8_t byteNr){
    CPU::SP -= 2;                                               // Decrement Stack Pointer

    CPU::PC++;                                                  // Increment Program Counter

    CPU::mmu -> write(CPU::SP, CPU::PC & 0xFF);                 // Write Least Significant Byte
    CPU::mmu -> write(CPU::SP + 1, (CPU::PC >> 8) & 0xFF);      // Write Most Significant Byte
    
    CPU::PC = byteNr;                                           // Set Program Counter
}

void CPU::execute(const uint8_t &instruction){
   switch(instruction){
        case 0x00:{
            CPU::NOP();
            CPU::PC++;
            break;
        }

        case 0x01:{
            CPU::LD(&B, &C, CPU::mmu -> read(CPU::PC + 2), CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 3;
            break;
        }

        case 0x02:{
            CPU::LD(CPU::B, CPU::C, CPU::A);
            CPU::PC++;
            break;
        }

        case 0x03:{
            CPU::INC(&B, &C);
            CPU::PC++;
            break;
        }

        case 0x04:{
            CPU::INC(&B);
            CPU::PC++;
            break;
        }

        case 0x05:{
            CPU::DEC(&B);
            CPU::PC++;
            break;
        }

        case 0x06:{
            CPU::LD(&B, CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0x07:{
            CPU::RLC(&A);
            CPU::PC += 1;
            break;
        }

        case 0x08:{
            CPU::LD(CPU::mmu -> read(CPU::PC + 2), CPU::mmu -> read(CPU::PC + 1),(CPU::SP & 0xFF));
            CPU::PC += 3;
            break;
        }

        case 0x09:{
            CPU::ADD(&H, &L, CPU::B, CPU::C);
            CPU::PC++;
            break;
        }

        case 0x0A:{
            CPU::LD(&A, CPU::B, CPU::C);
            CPU::PC++;
            break;
        }

        case 0x0B:{
            CPU::DEC(&B, &C);
            CPU::PC++;
            break;
        }

        case 0x0C:{
            CPU::INC(&C);
            CPU::PC++;
            break;
        }

        case 0x0D:{
            CPU::DEC(&C);
            CPU::PC++;
            break;
        }

        case 0x0E:{
            CPU::LD(&C, CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0x0F:{
            CPU::RRC(&A);
            CPU::PC++;
            break;
        }
        
        case 0x10:{
            CPU::STOP();
            CPU::PC += 2;
            break;
        }

        case 0x11:{
            CPU::LD(&D, &E, CPU::mmu -> read(CPU::PC + 2), CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 3;
            break;
        }

        case 0x12:{
            CPU::LD(CPU::D, CPU::E, CPU::A);
            CPU::PC++;
            break;
        }

        case 0x13:{
            CPU::INC(&D, &E);
            CPU::PC++;
            break;
        }

        case 0x14:{
            CPU::INC(&D);
            CPU::PC++;
            break;
        }

        case 0x15:{
            CPU::DEC(&D);
            CPU::PC++;
            break;
        }

        case 0x16:{
            CPU::LD(&D, CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0x17:{
            CPU::RL(&A);
            CPU::PC++;
            break;
        }

        case 0x18:{
            CPU::JR(CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0x19:{
            CPU::ADD(&H, &L, CPU::D, CPU::E);
            CPU::PC++;
            break;
        }

        case 0x1A:{
            CPU::LD(&A, CPU::D, CPU::E);
            CPU::PC++;
            break;
        }

        case 0x1B:{
            CPU::DEC(&D, &E);
            CPU::PC++;
            break;
        }

        case 0x1C:{
            CPU::INC(&E);
            CPU::PC++;
            break;
        }
        
        case 0x1D:{
            CPU::DEC(&E);
            CPU::PC++;
            break;
        }
        
        case 0x1E:{
            CPU::LD(&E, CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0x1F:{
            CPU::RR(&A);
            CPU::PC++;
            break;
        }

        case 0x20:{
            CPU::JRN(FLAG_Z, CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0x21:{
            CPU::LD(&H, &L, CPU::mmu -> read(CPU::PC + 2), CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 3;
           break;
        }

        case 0x22:{
            CPU::LD(CPU::H, CPU::L, CPU::A);

            // Increment HL
            uint16_t reg = ((uint16_t) H << 8) + CPU::L;
            reg++;
            CPU::H = (reg & 0xFF00) >> 8;
            CPU::L = reg & 0xFF;

            CPU::PC++;
            break;
        }

        case 0x23:{
            CPU::INC(&H, &L);
            CPU::PC++;
            break;
        }

        case 0x24:{
            CPU::INC(&H);
            CPU::PC++;
            break;
        }

        case 0x25:{
            CPU::DEC(&H);
            CPU::PC++;
            break;
        }

        case 0x26:{
            CPU::LD(&H, CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0x27:{
            CPU::DAA();
            CPU::PC++;
            break;
        }

        case 0x28:{
            CPU::JR(FLAG_Z, CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0x29:{
            CPU::ADD(&H, &L, CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x2A:{
            CPU::LD(&A, CPU::H, CPU::L);
            
            // Increment HL
            uint16_t reg = ((uint16_t) H << 8) + CPU::L;
            reg++;
            CPU::H = (reg & 0xFF00) >> 8;
            CPU::L = reg & 0xFF;

            CPU::PC++;
            break;
        }

        case 0x2B:{
            CPU::DEC(&H, &L);
            CPU::PC++;
            break;
        }

        case 0x2C:{
            CPU::INC(&L);
            CPU::PC++;
            break;
        }

        case 0x2D:{
            CPU::DEC(&L);
            CPU::PC++;
            break;
        }

        case 0x2E:{
            CPU::LD(&L, CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0x2F:{
            CPU::CPL();
            CPU::PC++;
            break;
        }

        case 0x30:{
            CPU::JRN(FLAG_C, CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0x31:{
            CPU::LD(&SP, CPU::mmu -> read(CPU::PC + 2), CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 3;
            break;
        }

        case 0x32:{
            CPU::LD(CPU::H, CPU::L, CPU::A);

            // Decrement HL
            uint16_t reg = ((uint16_t) H << 8) + CPU::L;
            reg--;
            CPU::H = (reg & 0xFF00) >> 8;
            CPU::L = reg & 0xFF;

            CPU::PC++;
            break;
        }

        case 0x33:{
            CPU::INC(&SP);
            CPU::PC++;
            break;
        }

        case 0x34:{
            CPU::INC(CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x35:{
            CPU::DEC(CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x36:{
            CPU::LD(CPU::H, CPU::L, CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0x37:{
            CPU::SCF();
            CPU::PC += 2;
            break;
        }

        case 0x38:{
            CPU::JR(FLAG_C, CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0x39:{
            CPU::ADD(&H, &L, CPU::SP);
            CPU::PC++;
            break;
        }

        case 0x3A:{
            CPU::LD(&A, CPU::H, CPU::L);
            
            // Decrement HL
            uint16_t reg = ((uint16_t) H << 8) + CPU::L;
            reg--;
            CPU::H = (reg & 0xFF00) >> 8;
            CPU::L = reg & 0xFF;

            CPU::PC++;
            break;
        }

        case 0x3B:{
            CPU::DEC(&SP);
            CPU::PC++;
            break;
        }

        case 0x3C:{
            CPU::INC(&A);
            CPU::PC++;
            break;
        }

        case 0x3D:{
            CPU::DEC(&A);
            CPU::PC++;
            break;
        }

        case 0x3E:{
            CPU::LD(&A, CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0x3F:{
            CPU::CCF();
            CPU::PC++;
            break;
        }

        case 0x40:{
            CPU::LD(&B, CPU::B);
            CPU::PC++;
            break;
        }

        case 0x41:{
            CPU::LD(&B, CPU::C);
            CPU::PC++;
            break;
        }

        case 0x42:{
            CPU::LD(&B, CPU::D);
            CPU::PC++;
            break;
        }

        case 0x43:{
            CPU::LD(&B, CPU::E);
            CPU::PC++;
            break;
        }

        case 0x44:{
            CPU::LD(&B, CPU::H);
            CPU::PC++;
            break;
        }

        case 0x45:{
            CPU::LD(&B, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x46:{
            CPU::LD(&B, CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x47:{
            CPU::LD(&B, CPU::A);
            CPU::PC++;
            break;
        }

        case 0x48:{
            CPU::LD(&C, CPU::B);
            CPU::PC++;
            break;
        }

        case 0x49:{
            CPU::LD(&C, CPU::C);
            CPU::PC++;
            break;
        }

        case 0x4A:{
            CPU::LD(&C, CPU::D);
            CPU::PC++;
            break;
        }

        case 0x4B:{
            CPU::LD(&C, CPU::E);
            CPU::PC++;
            break;
        }

        case 0x4C:{
            CPU::LD(&C, CPU::H);
            CPU::PC++;
            break;
        }

        case 0x4D:{
            CPU::LD(&C, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x4E:{
            CPU::LD(&C, CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x4F:{
            CPU::LD(&C, CPU::A);
            CPU::PC++;
            break;
        }

        case 0x50:{
            CPU::LD(&D, CPU::B);
            CPU::PC++;
            break;
        }

        case 0x51:{
            CPU::LD(&D, CPU::C);
            CPU::PC++;
            break;
        }

        case 0x52:{
            CPU::LD(&D, CPU::D);
            CPU::PC++;
            break;
        }

        case 0x53:{
            CPU::LD(&D, CPU::E);
            CPU::PC++;
            break;
        }

        case 0x54:{
            CPU::LD(&D, CPU::H);
            CPU::PC++;
            break;
        }

        case 0x55:{
            CPU::LD(&D, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x56:{
            CPU::LD(&D, CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x57:{
            CPU::LD(&D, CPU::A);
            CPU::PC++;
            break;
        }

        case 0x58:{
            CPU::LD(&E, CPU::B);
            CPU::PC++;
            break;
        }

        case 0x59:{
            CPU::LD(&E, CPU::C);
            CPU::PC++;
            break;
        }

        case 0x5A:{
            CPU::LD(&E, CPU::D);
            CPU::PC++;
            break;
        }

        case 0x5B:{
            CPU::LD(&E, CPU::E);
            CPU::PC++;
            break;
        }

        case 0x5C:{
            CPU::LD(&E, CPU::H);
            CPU::PC++;
            break;
        }

        case 0x5D:{
            CPU::LD(&E, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x5E:{
            CPU::LD(&E, CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x5F:{
            CPU::LD(&E, CPU::A);
            CPU::PC++;
            break;
        }

        case 0x60:{
            CPU::LD(&H, CPU::B);
            CPU::PC++;
            break;
        }

        case 0x61:{
            CPU::LD(&H, CPU::C);
            CPU::PC++;
            break;
        }

        case 0x62:{
            CPU::LD(&H, CPU::D);
            CPU::PC++;
            break;
        }

        case 0x63:{
            CPU::LD(&H, CPU::E);
            CPU::PC++;
            break;
        }

        case 0x64:{
            CPU::LD(&H, CPU::H);
            CPU::PC++;
            break;
        }

        case 0x65:{
            CPU::LD(&H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x66:{
            CPU::LD(&H, CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x67:{
            CPU::LD(&H, CPU::A);
            CPU::PC++;
            break;
        }

        case 0x68:{
            CPU::LD(&L, CPU::B);
            CPU::PC++;
            break;
        }

        case 0x69:{
            CPU::LD(&L, CPU::C);
            CPU::PC++;
            break;
        }

        case 0x6A:{
            CPU::LD(&L, CPU::D);
            CPU::PC++;
            break;
        }

        case 0x6B:{
            CPU::LD(&L, CPU::E);
            CPU::PC++;
            break;
        }

        case 0x6C:{
            CPU::LD(&L, CPU::H);
            CPU::PC++;
            break;
        }

        case 0x6D:{
            CPU::LD(&L, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x6E:{
            CPU::LD(&L, CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x6F:{
            CPU::LD(&L, CPU::A);
            CPU::PC++;
            break;
        }

        case 0x70:{
            CPU::LD(CPU::H, CPU::L, CPU::B);
            CPU::PC++;
            break;
        }

        case 0x71:{
            CPU::LD(CPU::H, CPU::L, CPU::C);
            CPU::PC++;
            break;
        }

        case 0x72:{
            CPU::LD(CPU::H, CPU::L, CPU::D);
            CPU::PC++;
            break;
        }

        case 0x73:{
            CPU::LD(CPU::H, CPU::L, CPU::E);
            CPU::PC++;
            break;
        }

        case 0x74:{
            CPU::LD(CPU::H, CPU::L, CPU::H);
            CPU::PC++;
            break;
        }

        case 0x75:{
            CPU::LD(CPU::H, CPU::L, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x76:{
            CPU::HALT();
            CPU::PC++;
            break;
        }

        case 0x77:{
            CPU::LD(CPU::H, CPU::L, CPU::A);
            CPU::PC++;
            break;
        }

        case 0x78:{
            CPU::LD(&A, CPU::B);
            CPU::PC++;
            break;
        }

        case 0x79:{
            CPU::LD(&A, CPU::C);
            CPU::PC++;
            break;
        }

        case 0x7A:{
            CPU::LD(&A, CPU::D);
            CPU::PC++;
            break;
        }

        case 0x7B:{
            CPU::LD(&A, CPU::E);
            CPU::PC++;
            break;
        }

        case 0x7C:{
            CPU::LD(&A, CPU::H);
            CPU::PC++;
            break;
        }

        case 0x7D:{
            CPU::LD(&A, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x7E:{
            CPU::LD(&A, CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x7F:{
            CPU::LD(&A, CPU::A);
            CPU::PC++;
            break;
        }

        case 0x80:{
            CPU::ADD(&A, CPU::B);
            CPU::PC++;
            break;
        }

        case 0x81:{
            CPU::ADD(&A, CPU::C);
            CPU::PC++;
            break;
        }

        case 0x82:{
            CPU::ADD(&A, CPU::D);
            CPU::PC++;
            break;
        }

        case 0x83:{
            CPU::ADD(&A, CPU::E);
            CPU::PC++;
            break;
        }

        case 0x84:{
            CPU::ADD(&A, CPU::H);
            CPU::PC++;
            break;
        }

        case 0x85:{
            CPU::ADD(&A, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x86:{
            CPU::ADD(&A, CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x87:{
            CPU::ADD(&A, CPU::A);
            CPU::PC++;
            break;
        }

        case 0x88:{
            CPU::ADC(CPU::B);
            CPU::PC++;
            break;
        }

        case 0x89:{
            CPU::ADC(CPU::C);
            CPU::PC++;
            break;
        }

        case 0x8A:{
            CPU::ADC(CPU::D);
            CPU::PC++;
            break;
        }

        case 0x8B:{
            CPU::ADC(CPU::E);
            CPU::PC++;
            break;
        }

        case 0x8C:{
            CPU::ADC(CPU::H);
            CPU::PC++;
            break;
        }

        case 0x8D:{
            CPU::ADC(CPU::L);
            CPU::PC++;
            break;
        }

        case 0x8E:{
            CPU::ADC(CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x8F:{
            CPU::ADC(CPU::A);
            CPU::PC++;
            break;
        }

        case 0x90:{
            CPU::SUB(CPU::B);
            CPU::PC++;
            break;
        }

        case 0x91:{
            CPU::SUB(CPU::C);
            CPU::PC++;
            break;
        }

        case 0x92:{
            CPU::SUB(CPU::D);
            CPU::PC++;
            break;
        }

        case 0x93:{
            CPU::SUB(CPU::E);
            CPU::PC++;
            break;
        }

        case 0x94:{
            CPU::SUB(CPU::H);
            CPU::PC++;
            break;
        }

        case 0x95:{
            CPU::SUB(CPU::L);
            CPU::PC++;
            break;
        }

        case 0x96:{
            CPU::SUB(CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x97:{
            CPU::SUB(CPU::A);
            CPU::PC++;
            break;
        }

        case 0x98:{
            CPU::SBC(CPU::B);
            CPU::PC++;
            break;
        }

        case 0x99:{
            CPU::SBC(CPU::C);
            CPU::PC++;
            break;
        }

        case 0x9A:{
            CPU::SBC(CPU::D);
            CPU::PC++;
            break;
        }

        case 0x9B:{
            CPU::SBC(CPU::E);
            CPU::PC++;
            break;
        }

        case 0x9C:{
            CPU::SBC(CPU::H);
            CPU::PC++;
            break;
        }

        case 0x9D:{
            CPU::SBC(CPU::L);
            CPU::PC++;
            break;
        }

        case 0x9E:{
            CPU::SBC(CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0x9F:{
            CPU::SBC(CPU::A);
            CPU::PC++;
            break;
        }

        case 0xA0:{
            CPU::AND(CPU::B);
            CPU::PC++;
            break;
        }

        case 0xA1:{
            CPU::AND(CPU::C);
            CPU::PC++;
            break;
        }

        case 0xA2:{
            CPU::AND(CPU::D);
            CPU::PC++;
            break;
        }

        case 0xA3:{
            CPU::AND(CPU::E);
            CPU::PC++;
            break;
        }

        case 0xA4:{
            CPU::AND(CPU::H);
            CPU::PC++;
            break;
        }

        case 0xA5:{
            CPU::AND(CPU::L);
            CPU::PC++;
            break;
        }

        case 0xA6:{
            CPU::AND(CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0xA7:{
            CPU::AND(CPU::A);
            CPU::PC++;
            break;
        }

        case 0xA8:{
            CPU::XOR(CPU::B);
            CPU::PC++;
            break;
        }

        case 0xA9:{
            CPU::XOR(CPU::C);
            CPU::PC++;
            break;
        }

        case 0xAA:{
            CPU::XOR(CPU::D);
            CPU::PC++;
            break;
        }

        case 0xAB:{
            CPU::XOR(CPU::E);
            CPU::PC++;
            break;
        }

        case 0xAC:{
            CPU::XOR(CPU::H);
            CPU::PC++;
            break;
        }

        case 0xAD:{
            CPU::XOR(CPU::L);
            CPU::PC++;
            break;
        }

        case 0xAE:{
            CPU::XOR(CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0xAF:{
            CPU::XOR(CPU::A);

            CPU::PC++;
            break;
        }

        case 0xB0:{
            CPU::OR(CPU::B);
            CPU::PC++;
            break;
        }

        case 0xB1:{
            CPU::OR(CPU::C);
            CPU::PC++;
            break;
        }

        case 0xB2:{
            CPU::OR(CPU::D);
            CPU::PC++;
            break;
        }

        case 0xB3:{
            CPU::OR(CPU::E);
            CPU::PC++;
            break;
        }

        case 0xB4:{
            CPU::OR(CPU::H);
            CPU::PC++;
            break;
        }

        case 0xB5:{
            CPU::OR(CPU::L);
            CPU::PC++;
            break;
        }

        case 0xB6:{
            CPU::OR(CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0xB7:{
            CPU::OR(CPU::A);
            CPU::PC++;
            break;
        }

        case 0xB8:{
            CPU::CP(CPU::B);
            CPU::PC++;
            break;
        }

        case 0xB9:{
            CPU::CP(CPU::C);
            CPU::PC++;
            break;
        }

        case 0xBA:{
            CPU::CP(CPU::D);
            CPU::PC++;
            break;
        }

        case 0xBB:{
            CPU::CP(CPU::E);
            CPU::PC++;
            break;
        }

        case 0xBC:{
            CPU::CP(CPU::H);
            CPU::PC++;
            break;
        }

        case 0xBD:{
            CPU::CP(CPU::L);
            CPU::PC++;
            break;
        }

        case 0xBE:{
            CPU::CP(CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0xBF:{
            CPU::CP(CPU::A);
            CPU::PC++;
            break;
        }

        case 0xC0:{
            CPU::RETN(FLAG_Z);
            break;
        }

        case 0xC1:{
            CPU::POP(&B, &C);
            CPU::PC++;
            break;
        }

        case 0xC2:{
            CPU::JPN(FLAG_Z, CPU::mmu -> read(CPU::PC + 2), CPU::mmu -> read(CPU::PC + 1));
            break;
        }

        case 0xC3:{
            CPU::JP(CPU::mmu -> read(CPU::PC + 2), CPU::mmu -> read(CPU::PC + 1));
            break;
        }

        case 0xC4:{
            CPU::CALLN(FLAG_Z, CPU::mmu -> read(CPU::PC + 2), CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 3;
            break;
        }

        case 0xC5:{
            CPU::PUSH(CPU::B, CPU::C);
            CPU::PC++;
            break;
        }

        case 0xC6:{
            CPU::ADD(&A, CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0xC7:{
            CPU::RST(0x00);
            break;
        }

        case 0xC8:{
            CPU::RET(FLAG_Z);
            break;
        }

        case 0xC9:{
            CPU::RET();
            break;
        }

        case 0xCA:{
            CPU::JP(FLAG_Z, CPU::mmu -> read(CPU::PC + 2), CPU::mmu -> read(CPU::PC + 1));
            break;
        }

        case 0xCB:{
            switch(CPU::mmu -> read(CPU::PC + 1)){
                case 0x00:{
                    CPU::RLC(&B);
                    break;
                }

                case 0x01:{
                    CPU::RLC(&C);
                    break;
                }
                
                case 0x02:{
                    CPU::RLC(&D);
                    break;
                }
                
                case 0x03:{
                    CPU::RLC(&E);
                    break;
                }
                
                case 0x04:{
                    CPU::RLC(&H);
                    break;
                }
                
                case 0x05:{
                    CPU::RLC(&L);
                    break;
                }
                
                case 0x06:{
                    CPU::RLC(CPU::H, CPU::L);
                    break;
                }

                case 0x07:{
                    CPU::RLC(&A);
                    break;
                }

                case 0x08:{
                    CPU::RRC(&B);
                    break;
                }

                case 0x09:{
                    CPU::RRC(&C);
                    break;
                }

                case 0x0A:{
                    CPU::RRC(&D);
                    break;
                }

                case 0x0B:{
                    CPU::RRC(&E);
                    break;
                }

                case 0x0C:{
                    CPU::RRC(&H);
                    break;
                }

                case 0x0D:{
                    CPU::RRC(&L);
                    break;
                }

                case 0x0E:{
                    CPU::RRC(CPU::H, CPU::L);
                    break;
                }

                case 0x0F:{
                    CPU::RRC(&A);
                    break;
                }

                case 0x10:{
                    CPU::RL(&B);
                    break;
                }

                case 0x11:{
                    CPU::RL(&C);
                    break;
                }

                case 0x12:{
                    CPU::RL(&D);
                    break;
                }

                case 0x13:{
                    CPU::RL(&E);
                    break;
                }

                case 0x14:{
                    CPU::RL(&H);
                    break;
                }

                case 0x15:{
                    CPU::RL(&L);
                    break;
                }

                case 0x16:{
                    CPU::RL(CPU::H, CPU::L);
                    break;
                }

                case 0x17:{
                    CPU::RL(&A);
                    break;
                }

                case 0x18:{
                    CPU::RR(&B);
                    break;
                }

                case 0x19:{
                    CPU::RR(&C);
                    break;
                }

                case 0x1A:{
                    CPU::RR(&D);
                    break;
                }

                case 0x1B:{
                    CPU::RR(&E);
                    break;
                }

                case 0x1C:{
                    CPU::RR(&H);
                    break;
                }

                case 0x1D:{
                    CPU::RR(&L);
                    break;
                }

                case 0x1E:{
                    CPU::RR(CPU::H, CPU::L);
                    break;
                }

                case 0x1F:{
                    CPU::RR(&A);
                    break;
                }

                case 0x20:{
                    CPU::SLA(&B);
                    break;
                }

                case 0x21:{
                    CPU::SLA(&C);
                    break;
                }

                case 0x22:{
                    CPU::SLA(&D);
                    break;
                }

                case 0x23:{
                    CPU::SLA(&E);
                    break;
                }

                case 0x24:{
                    CPU::SLA(&H);
                    break;
                }

                case 0x25:{
                    CPU::SLA(&L);
                    break;
                }

                case 0x26:{
                    CPU::SLA(CPU::H, CPU::L);
                    break;
                }

                case 0x27:{
                    CPU::SLA(&A);
                    break;
                }

                case 0x28:{
                    CPU::SRA(&B);
                    break;
                }

                case 0x29:{
                    CPU::SRA(&C);
                    break;
                }

                case 0x2A:{
                    CPU::SRA(&D);
                    break;
                }

                case 0x2B:{
                    CPU::SRA(&E);
                    break;
                }

                case 0x2C:{
                    CPU::SRA(&H);
                    break;
                }

                case 0x2D:{
                    CPU::SRA(&L);
                    break;
                }

                case 0x2E:{
                    CPU::SRA(CPU::H, CPU::L);
                    break;
                }

                case 0x2F:{
                    CPU::SRA(&A);
                    break;
                }

                case 0x30:{
                    CPU::SWAP(&B);
                    break;
                }

                case 0x31:{
                    CPU::SWAP(&C);
                    break;
                }

                case 0x32:{
                    CPU::SWAP(&D);
                    break;
                }

                case 0x33:{
                    CPU::SWAP(&E);
                    break;
                }

                case 0x34:{
                    CPU::SWAP(&H);
                    break;
                }

                case 0x35:{
                    CPU::SWAP(&L);
                    break;
                }

                case 0x36:{
                    CPU::SWAP(CPU::H, CPU::L);
                    break;
                }

                case 0x37:{
                    CPU::SWAP(&A);
                    break;
                }

                case 0x38:{
                    CPU::SRL(&B);
                    break;
                }

                case 0x39:{
                    CPU::SRL(&C);
                    break;
                }

                case 0x3A:{
                    CPU::SRL(&D);
                    break;
                }

                case 0x3B:{
                    CPU::SRL(&E);
                    break;
                }

                case 0x3C:{
                    CPU::SRL(&H);
                    break;
                }

                case 0x3D:{
                    CPU::SRL(&L);
                    break;
                }

                case 0x3E:{
                    CPU::SRL(CPU::H, CPU::L);
                    break;
                }

                case 0x3F:{
                    CPU::SRL(&A);
                    break;
                }

                case 0x40:{
                    CPU::BIT(0x0, CPU::B);
                    break;
                }

                case 0x41:{
                    CPU::BIT(0x0, CPU::C);
                    break;
                }

                case 0x42:{
                    CPU::BIT(0x0, CPU::D);
                    break;
                }

                case 0x43:{
                    CPU::BIT(0x0, CPU::E);
                    break;
                }

                case 0x44:{
                    CPU::BIT(0x0, CPU::H);
                    break;
                }

                case 0x45:{
                    CPU::BIT(0x0, CPU::L);
                    break;
                }

                case 0x46:{
                    CPU::BIT(0x0, CPU::H, CPU::L);
                    break;
                }

                case 0x47:{
                    CPU::BIT(0x0, CPU::A);
                    break;
                }

                case 0x48:{
                    CPU::BIT(0x1, CPU::B);
                    break;
                }

                case 0x49:{
                    CPU::BIT(0x1, CPU::C);
                    break;
                }

                case 0x4A:{
                    CPU::BIT(0x1, CPU::D);
                    break;
                }

                case 0x4B:{
                    CPU::BIT(0x1, CPU::E);
                    break;
                }

                case 0x4C:{
                    CPU::BIT(0x1, CPU::H);
                    break;
                }

                case 0x4D:{
                    CPU::BIT(0x1, CPU::L);
                    break;
                }

                case 0x4E:{
                    CPU::BIT(0x1, CPU::H, CPU::L);
                    break;
                }

                case 0x4F:{
                    CPU::BIT(0x1, CPU::A);
                    break;
                }

                case 0x50:{
                    CPU::BIT(0x2, CPU::B);
                    break;
                }

                case 0x51:{
                    CPU::BIT(0x2, CPU::C);
                    break;
                }

                case 0x52:{
                    CPU::BIT(0x2, CPU::D);
                    break;
                }

                case 0x53:{
                    CPU::BIT(0x2, CPU::E);
                    break;
                }

                case 0x54:{
                    CPU::BIT(0x2, CPU::H);
                    break;
                }

                case 0x55:{
                    CPU::BIT(0x2, CPU::L);
                    break;
                }

                case 0x56:{
                    CPU::BIT(0x2, CPU::H, CPU::L);
                    break;
                }

                case 0x57:{
                    CPU::BIT(0x2, CPU::A);
                    break;
                }

                case 0x58:{
                    CPU::BIT(0x3, CPU::B);
                    break;
                }

                case 0x59:{
                    CPU::BIT(0x3, CPU::C);
                    break;
                }

                case 0x5A:{
                    CPU::BIT(0x3, CPU::D);
                    break;
                }

                case 0x5B:{
                    CPU::BIT(0x3, CPU::E);
                    break;
                }

                case 0x5C:{
                    CPU::BIT(0x3, CPU::H);
                    break;
                }

                case 0x5D:{
                    CPU::BIT(0x3, CPU::L);
                    break;
                }

                case 0x5E:{
                    CPU::BIT(0x3, CPU::H, CPU::L);
                    break;
                }

                case 0x5F:{
                    CPU::BIT(0x3, CPU::A);
                    break;
                }

                case 0x60:{
                    CPU::BIT(0x4, CPU::B);
                    break;
                }

                case 0x61:{
                    CPU::BIT(0x4, CPU::C);
                    break;
                }

                case 0x62:{
                    CPU::BIT(0x4, CPU::D);
                    break;
                }

                case 0x63:{
                    CPU::BIT(0x4, CPU::E);
                    break;
                }

                case 0x64:{
                    CPU::BIT(0x4, CPU::H);
                    break;
                }

                case 0x65:{
                    CPU::BIT(0x4, CPU::L);
                    break;
                }

                case 0x66:{
                    CPU::BIT(0x4, CPU::H, CPU::L);
                    break;
                }

                case 0x67:{
                    CPU::BIT(0x4, CPU::A);
                    break;
                }

                case 0x68:{
                    CPU::BIT(0x5, CPU::B);
                    break;
                }

                case 0x69:{
                    CPU::BIT(0x5, CPU::C);
                    break;
                }

                case 0x6A:{
                    CPU::BIT(0x5, CPU::D);
                    break;
                }

                case 0x6B:{
                    CPU::BIT(0x5, CPU::E);
                    break;
                }

                case 0x6C:{
                    CPU::BIT(0x5, CPU::H);
                    break;
                }

                case 0x6D:{
                    CPU::BIT(0x5, CPU::L);
                    break;
                }

                case 0x6E:{
                    CPU::BIT(0x5, CPU::H, CPU::L);
                    break;
                }

                case 0x6F:{
                    CPU::BIT(0x5, CPU::A);
                    break;
                }

                case 0x70:{
                    CPU::BIT(0x6, CPU::B);
                    break;
                }

                case 0x71:{
                    CPU::BIT(0x6, CPU::C);
                    break;
                }

                case 0x72:{
                    CPU::BIT(0x6, CPU::D);
                    break;
                }

                case 0x73:{
                    CPU::BIT(0x6, CPU::E);
                    break;
                }

                case 0x74:{
                    CPU::BIT(0x6, CPU::H);
                    break;
                }

                case 0x75:{
                    CPU::BIT(0x6, CPU::L);
                    break;
                }
                
                case 0x76:{
                    CPU::BIT(0x6, CPU::H, CPU::L);
                    break;
                }

                case 0x77:{
                    CPU::BIT(0x6, CPU::A);
                    break;
                }

                case 0x78:{
                    CPU::BIT(0x7, CPU::B);
                    break;
                }

                case 0x79:{
                    CPU::BIT(0x7, CPU::C);
                    break;
                }

                case 0x7A:{
                    CPU::BIT(0x7, CPU::D);
                    break;
                }

                case 0x7B:{
                    CPU::BIT(0x7, CPU::E);
                    break;
                }

                case 0x7C:{
                    CPU::BIT(0x7, CPU::H);
                    break;
                }

                case 0x7D:{
                    CPU::BIT(0x7, CPU::L);
                    break;
                }

                case 0x7E:{
                    CPU::BIT(0x7, CPU::H, CPU::L);
                    break;
                }

                case 0x7F:{
                    CPU::BIT(0x7, CPU::A);
                    break;
                }

                case 0x80:{
                    CPU::RES(0x0, &B);
                    break;
                }

                case 0x81:{
                    CPU::RES(0x0, &C);
                    break;
                }

                case 0x82:{
                    CPU::RES(0x0, &D);
                    break;
                }

                case 0x83:{
                    CPU::RES(0x0, &E);
                    break;
                }

                case 0x84:{
                    CPU::RES(0x0, &H);
                    break;
                }

                case 0x85:{
                    CPU::RES(0x0, &L);
                    break;
                }

                case 0x86:{
                    CPU::RES(0x0, CPU::H, CPU::L);
                    break;
                }

                case 0x87:{
                    CPU::RES(0x0, &A);
                    break;
                }

                case 0x88:{
                    CPU::RES(0x1, &B);
                    break;
                }

                case 0x89:{
                    CPU::RES(0x1, &C);
                    break;
                }

                case 0x8A:{
                    CPU::RES(0x1, &D);
                    break;
                }

                case 0x8B:{
                    CPU::RES(0x1, &E);   
                    break;
                }

                case 0x8C:{
                    CPU::RES(0x1, &H);
                    break;
                }

                case 0x8D:{
                    CPU::RES(0x1, &L);
                    break;
                }

                case 0x8E:{
                    CPU::RES(0x1, CPU::H, CPU::L);
                    break;
                }

                case 0x8F:{
                    CPU::RES(0x1, &A);
                    break;
                }

                case 0x90:{
                    CPU::RES(0x2, &B);
                    break;
                }

                case 0x91:{
                    CPU::RES(0x2, &C);
                    break;
                }

                case 0x92:{
                    CPU::RES(0x2, &D);
                    break;
                }

                case 0x93:{
                    CPU::RES(0x2, &E);
                    break;
                }

                case 0x94:{
                    CPU::RES(0x2, &H);
                    break;
                }

                case 0x95:{
                    CPU::RES(0x2, &L);
                    break;
                }

                case 0x96:{
                    CPU::RES(0x2, CPU::H, CPU::L);
                    break;
                }

                case 0x97:{
                    CPU::RES(0x2, &A);
                    break;
                }

                case 0x98:{
                    CPU::RES(0x3, &B);
                    break;
                }

                case 0x99:{
                    CPU::RES(0x3, &C);
                    break;
                }

                case 0x9A:{
                    CPU::RES(0x3, &D);
                    break;
                }

                case 0x9B:{
                    CPU::RES(0x3, &E);
                    break;
                }

                case 0x9C:{
                    CPU::RES(0x3, &H);
                    break;
                }

                case 0x9D:{
                    CPU::RES(0x3, &L);
                    break;
                }

                case 0x9E:{
                    CPU::RES(0x3, CPU::H, CPU::L);
                    break;
                }

                case 0x9F:{
                    CPU::RES(0x3, &A);
                    break;
                }

                case 0xA0:{
                    CPU::RES(0x4, &B);
                    break;
                }

                case 0xA1:{
                    CPU::RES(0x4, &C);
                    break;
                }

                case 0xA2:{
                    CPU::RES(0x4, &D);
                    break;
                }

                case 0xA3:{
                    CPU::RES(0x4, &E);
                    break;
                }

                case 0xA4:{
                    CPU::RES(0x4, &H);
                    break;
                }

                case 0xA5:{
                    CPU::RES(0x4, &L);
                    break;
                }

                case 0xA6:{
                    CPU::RES(0x4, CPU::H, CPU::L);
                    break;
                }

                case 0xA7:{
                    CPU::RES(0x4, &A);
                    break;
                }

                case 0xA8:{
                    CPU::RES(0x5, &B);
                    break;
                }

                case 0xA9:{
                    CPU::RES(0x5, &C);
                    break;
                }

                case 0xAA:{
                    CPU::RES(0x5, &D);
                    break;
                }

                case 0xAB:{
                    CPU::RES(0x5, &E);
                    break;
                }

                case 0xAC:{
                    CPU::RES(0x5, &H);
                    break;
                }

                case 0xAD:{
                    CPU::RES(0x5, &L);
                    break;
                }

                case 0xAE:{
                    CPU::RES(0x5, CPU::H, CPU::L);
                    break;
                }

                case 0xAF:{
                    CPU::RES(0x5, &A);
                    break;
                }

                case 0xB0:{
                    CPU::RES(0x6, &B);
                    break;
                }

                case 0xB1:{
                    CPU::RES(0x6, &C);
                    break;
                }

                case 0xB2:{
                    CPU::RES(0x6, &D);
                    break;
                }

                case 0xB3:{
                    CPU::RES(0x6, &E);
                    break;
                }

                case 0xB4:{
                    CPU::RES(0x6, &H);
                    break;
                }

                case 0xB5:{
                    CPU::RES(0x6, &L);
                    break;
                }

                case 0xB6:{
                    CPU::RES(0x6, CPU::H, CPU::L);
                    break;
                }

                case 0xB7:{
                    CPU::RES(0x6, &A);
                    break;
                }

                case 0xB8:{
                    CPU::RES(0x7, &B);
                    break;
                }

                case 0xB9:{
                    CPU::RES(0x7, &C);
                    break;
                }

                case 0xBA:{
                    CPU::RES(0x7, &D);
                    break;
                }

                case 0xBB:{
                    CPU::RES(0x7, &E);
                    break;
                }

                case 0xBC:{
                    CPU::RES(0x7, &H);
                    break;
                }

                case 0xBD:{
                    CPU::RES(0x7, &L);
                    break;
                }

                case 0xBE:{
                    CPU::RES(0x7, CPU::H, CPU::L);
                    break;
                }

                case 0xBF:{
                    CPU::RES(0x7, &A);
                    break;
                }

                case 0xC0:{
                    CPU::SET(0x0, &B);
                    break;
                }

                case 0xC1:{
                    CPU::SET(0x0, &C);
                    break;
                }

                case 0xC2:{
                    CPU::SET(0x0, &D);
                    break;
                }

                case 0xC3:{
                    CPU::SET(0x0, &E);
                    break;
                }

                case 0xC4:{
                    CPU::SET(0x0, &H);
                    break;
                }

                case 0xC5:{
                    CPU::SET(0x0, &L);
                    break;
                }

                case 0xC6:{
                    CPU::SET(0x0, CPU::H, CPU::L);
                    break;
                }

                case 0xC7:{
                    CPU::SET(0x0, &A);
                    break;
                }

                case 0xC8:{
                    CPU::SET(0x1, &B);
                    break;
                }

                case 0xC9:{
                    CPU::SET(0x1, &C);
                    break;
                }

                case 0xCA:{
                    CPU::SET(0x1, &D);
                    break;
                }

                case 0xCB:{
                    CPU::SET(0x1, &E);
                    break;
                }

                case 0xCC:{
                    CPU::SET(0x1, &H);
                    break;
                }

                case 0xCD:{
                    CPU::SET(0x1, &L);
                    break;
                }

                case 0xCE:{
                    CPU::SET(0x1, CPU::H, CPU::L);   
                    break;
                }

                case 0xCF:{
                    CPU::SET(0x1, &A);
                    break;
                }

                case 0xD0:{
                    CPU::SET(0x2, &B);
                    break;
                }

                case 0xD1:{
                    CPU::SET(0x2, &C);
                    break;
                }

                case 0xD2:{
                    CPU::SET(0x2, &D);
                    break;
                }

                case 0xD3:{
                    CPU::SET(0x2, &E);   
                    break;
                }

                case 0xD4:{
                    CPU::SET(0x2, &H);
                    break;
                }

                case 0xD5:{
                    CPU::SET(0x2, &L);
                    break;
                }

                case 0xD6:{
                    CPU::SET(0x2, CPU::H, CPU::L);
                    break;
                }

                case 0xD7:{
                    CPU::SET(0x2, &A);
                    break;
                }

                case 0xD8:{
                    CPU::SET(0x3, &B);
                    break;
                }

                case 0xD9:{
                    CPU::SET(0x3, &C);
                    break;
                }

                case 0xDA:{
                    CPU::SET(0x3, &D);
                    break;
                }

                case 0xDB:{
                    CPU::SET(0x3, &E);
                    break;
                }

                case 0xDC:{
                    CPU::SET(0x3, &H);
                    break;
                }

                case 0xDD:{
                    CPU::SET(0x3, &L);
                    break;
                }

                case 0xDE:{
                    CPU::SET(0x3, CPU::H, CPU::L);
                    break;
                }

                case 0xDF:{
                    CPU::SET(0x3, &A);
                    break;
                }

                case 0xE0:{
                    CPU::SET(0x4, &B);
                    break;
                }

                case 0xE1:{
                    CPU::SET(0x4, &C);
                    break;
                }

                case 0xE2:{
                    CPU::SET(0x4, &D);
                    break;
                }

                case 0xE3:{
                    CPU::SET(0x4, &E);
                    break;
                }

                case 0xE4:{
                    CPU::SET(0x4, &H);
                    break;
                }

                case 0xE5:{
                    CPU::SET(0x4, &L);
                    break;
                }

                case 0xE6:{
                    CPU::SET(0x4, CPU::H, CPU::L);
                    break;
                }

                case 0xE7:{
                    CPU::SET(0x4, &A);
                    break;
                }

                case 0xE8:{
                    CPU::SET(0x5, &B);
                    break;
                }

                case 0xE9:{
                    CPU::SET(0x5, &C);
                    break;
                }

                case 0xEA:{
                    CPU::SET(0x5, &D);
                    break;
                }

                case 0xEB:{
                    CPU::SET(0x5, &E);
                    break;
                }

                case 0xEC:{
                    CPU::SET(0x5, &H);
                    break;
                }

                case 0xED:{
                    CPU::SET(0x5, &L);
                    break;
                }

                case 0xEE:{
                    CPU::SET(0x5, CPU::H, CPU::L);
                    break;
                }

                case 0xEF:{
                    CPU::SET(0x5, &A);
                    break;
                }

                case 0xF0:{
                    CPU::SET(0x6, &B);
                    break;
                }

                case 0xF1:{
                    CPU::SET(0x6, &C);
                    break;
                }

                case 0xF2:{
                    CPU::SET(0x6, &D);
                    break;
                }

                case 0xF3:{
                    CPU::SET(0x6, &E);
                    break;
                }

                case 0xF4:{
                    CPU::SET(0x6, &H);
                    break;
                }

                case 0xF5:{
                    CPU::SET(0x6, &L);
                    break;
                }

                case 0xF6:{
                    CPU::SET(0x6, CPU::H, CPU::L);
                    break;
                }

                case 0xF7:{
                    CPU::SET(0x6, &A);
                    break;
                }

                case 0xF8:{
                    CPU::SET(0x7, &B);
                    break;
                }

                case 0xF9:{
                    CPU::SET(0x7, &C);
                    break;
                }

                case 0xFA:{
                    CPU::SET(0x7, &D);
                    break;
                }

                case 0xFB:{
                    CPU::SET(0x7, &E);
                    break;
                }

                case 0xFC:{
                    CPU::SET(0x7, &H);
                    break;
                }

                case 0xFD:{
                    CPU::SET(0x7, &L);
                    break;
                }

                case 0xFE:{
                    CPU::SET(0x7, CPU::H, CPU::L);
                    break;
                }

                case 0xFF:{
                    CPU::SET(0x7, &A);
                    break;
                }

                default:{
                    std::cerr << "Invalid 0xCB Instruction!" << std::endl;
                }
            }

            CPU::PC += 2;
            break;
        }

        case 0xCC:{
            CPU::CALL(FLAG_Z, CPU::mmu -> read(CPU::PC + 2), CPU::mmu -> read(CPU::PC + 1));
            break;
        }

        case 0xCD:{
            CPU::CALL(CPU::mmu -> read(CPU::PC + 2), CPU::mmu -> read(CPU::PC + 1));
            break;
        }

        case 0xCE:{
            CPU::ADC(CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0xCF:{
            CPU::RST(0x08);
            break;
        }

        case 0xD0:{
            CPU::RETN(FLAG_C);
            break;
        }

        case 0xD1:{
            CPU::POP(&D, &E);
            CPU::PC++;
            break;
        }

        case 0xD2:{
            CPU::JPN(FLAG_C, CPU::mmu -> read(CPU::PC + 2), CPU::mmu -> read(CPU::PC + 1));
            break;
        }

        case 0xD4:{
            CPU::CALLN(FLAG_C, CPU::mmu -> read(CPU::PC + 2), CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 3;
            break;
        }

        case 0xD5:{
            CPU::PUSH(CPU::D, CPU::E);
            CPU::PC++;
            break;
        }

        case 0xD6:{
            CPU::SUB(CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0xD7:{
            CPU::RST(0x10);
            break;
        }

        case 0xD8:{
            CPU::RET(FLAG_C);
            break;
        }

        case 0xD9:{
            CPU::RETI();
            break;
        }

        case 0xDA:{
            CPU::JP(CPU::C, CPU::mmu -> read(CPU::PC + 2), CPU::mmu -> read(CPU::PC + 1));
            break;
        }

        case 0xDC:{
            CPU::CALL(FLAG_C, CPU::mmu -> read(CPU::PC + 2), CPU::mmu -> read(CPU::PC + 1));
            break;
        }

        case 0xDE:{
            CPU::SBC(CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0xDF:{
            CPU::RST(0x18);
            break;
        }

        case 0xE0:{
            CPU::LDH(CPU::mmu -> read(CPU::PC + 1), CPU::A);
            CPU::PC += 2;
            break;
        }

        case 0xE1:{
            CPU::POP(&H, &L);
            CPU::PC++;
            break;
        }

        case 0xE2:{
            CPU::LD(CPU::C, CPU::A);
            CPU::PC += 1;
            break;
        }

        case 0xE5:{
            CPU::PUSH(CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0xE6:{
            CPU::AND(CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0xE7:{
            CPU::RST(0x20);
            break;
        }

        case 0xE8:{
            CPU::ADD(&SP, CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0xE9:{
            CPU::JP(CPU::H, CPU::L);
            break;
        }

        case 0xEA:{
            CPU::LD(CPU::mmu -> read(CPU::PC + 2), CPU::mmu -> read(CPU::PC + 1), CPU::A);
            CPU::PC += 3;
            break;
        }

        case 0xEE:{
            CPU::XOR(CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0xEF:{
            CPU::RST(0x28);
            break;
        }

        case 0xF0:{
            CPU::LDH(&A, CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0xF1:{
            CPU::POP(&A, &F);
            CPU::PC++;
            break;
        }

        case 0xF2:{
            CPU::LD(&A, CPU::mmu -> read(CPU::C));
            CPU::PC += 1;
            break;
        }

        case 0xF3:{
            CPU::DI();
            CPU::PC++;
            break;
        }

        case 0xF5:{
            CPU::PUSH(CPU::A, CPU::F);
            CPU::PC++;
            break;
        }

        case 0xF6:{
            CPU::OR(CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0xF7:{
            CPU::RST(0x30);
            break;
        }

        case 0xF8:{
            // IMPLEMENT THIS OPERATION
            CPU::PC += 2;
            break;
        }

        case 0xF9:{
            CPU::LD(CPU::SP, CPU::H, CPU::L);
            CPU::PC++;
            break;
        }

        case 0xFA:{
            CPU::LD(&A, CPU::mmu -> read(CPU::PC + 2), CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 3;
            break;
        }

        case 0xFB:{
            CPU::EI();
            CPU::PC++;
            break;
        }

        case 0xFE:{
            CPU::CP(CPU::mmu -> read(CPU::PC + 1));
            CPU::PC += 2;
            break;
        }

        case 0xFF:{
            CPU::RST(0x38);
            break;
        }

        default:{
            std::cerr << "Invalid Instruction!" << std::endl;
            break;
        }
    }
}
