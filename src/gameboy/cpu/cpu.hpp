#ifndef CPU_HPP
#define CPU_HPP

#include <iostream>

#include "../mmu.hpp"

enum flags{
    FLAG_Z = 7,
    FLAG_N = 6,
    FLAG_H = 5,
    FLAG_C = 4
};

class CPU{
    private:
        // Private Class Variables
        MMU* mmu;
        uint8_t cycleCount;

        // Registers
        uint8_t A;
        uint8_t F;
        uint8_t B;
        uint8_t C;
        uint8_t D;
        uint8_t E;
        uint8_t H;
        uint8_t L;

        uint16_t SP;
        uint16_t PC;

        bool IME;

        // Private Class Functions
        void execute(const uint8_t &instruction);
        void checkInterrupts();

        // Flag Functions
        void setFlag(uint8_t flag);
        void clearFlag(uint8_t flag);
        bool getFlag(uint8_t flag);

        // Instruction Functions
        // LD Operations
        void LD(uint8_t *reg1a, uint8_t *reg1b, uint8_t d16a, uint8_t d16b);    // Store d16 In Reg1
        void LD(uint8_t *reg, uint8_t d8);                                      // Store d8 In Reg
        void LD(uint8_t addr1a, uint8_t addr1b, uint8_t d8);                    // Store d8 In Memory Location Addr
        void LD(uint8_t *reg, uint8_t addr1a, uint8_t addr1b);                  // Store Byte At Addr In Reg
        void LD(uint16_t *SP, uint8_t d16a, uint8_t d16b);                      // Store d16 In Stack Pointer
        void LD(uint8_t a8, uint8_t reg);
        void LDH(uint8_t a8, uint8_t reg);
        void LDH(uint8_t *reg, uint8_t a8);
        
        // INC Operations
        void INC(uint8_t *reg);                                                 // Increment 8-Bit Register
        void INC(uint8_t *reg1a, uint8_t *reg1b);                               // Increment 16-Bit Register
        void INC(uint8_t addr1a, uint8_t addr1b);                               // Increment Byte At Addr In Memory
        void INC(uint16_t* SP);                                                 // Increment Stack Pointer

        // DEC Operations
        void DEC(uint8_t *reg);                                                 // Decrement 8-Bit Register
        void DEC(uint8_t *reg1a, uint8_t *reg1b);                               // Decrement 16-Bit Register
        void DEC(uint8_t addr1a, uint8_t addr1b);                               // Decrement Byte At Addr in Memory
        void DEC(uint16_t *SP);                                                 // Decrement Stack Pointer

        // ADD Operations
        void ADD(uint8_t *reg1a, uint8_t *reg1b, uint8_t reg2a, uint8_t reg2b); // Add 16-Bit Reg1 and 16-Bit Reg2
        void ADD(uint8_t *reg, uint8_t d8);                                     // Add d8 And Reg And Store The Result In Reg
        void ADD(uint8_t *reg, uint8_t addr1a, uint8_t addr1b);                 // Add Reg With Byte At Address Add And Store The Result In Reg
        void ADD(uint8_t *reg1a, uint8_t *reg1b, uint16_t SP);                  // Add Reg1 With SP And Store The Result In Reg1
        void ADD(uint16_t* SP, int8_t s8);                                      // Add Signed s8 And SP And Store The Result In SP   
        void ADC(uint8_t reg);                                                  // Add Reg And The CY Flag And Store The Result In Register A
        void ADC(uint8_t addr1a, uint8_t addr1b);                               // Add Byte At Address Addr The CY Flag And Store The Result In Register A
        
        // SUB Operations
        void SUB(uint8_t reg);                                                  // Substract Register A From Reg And Store The Result In Register A
        void SUB(uint8_t addr1a, uint8_t addr1b);                               // Substract Register A From Byte At Address Addr1 And Store The Result In Register A
        void SBC(uint8_t reg);                                                  // Substract Register A From The CY Flag And Reg And Store The Result In Register A
        void SBC(uint8_t addr1a, uint8_t addr1b);                               // Substract Register A From The CY Flag And Byte At Address Addr and Store The Result In Register A
        
        // AND Operations
        void AND(uint8_t reg);                                                  // Store Result Of Logical AND Of Reg And A In A
        void AND(uint8_t addr1a, uint8_t addr1b);                               // Store Result Of Logical AND Of Reg And Byte At Address Addr In A
        
        // XOR Operations
        void XOR(uint8_t reg);                                                  // Store Result Of Logical XOR Of Reg And A In A
        void XOR(uint8_t addr1a, uint8_t addr1b);                               // Store Result Of Logical XOR Of Byte At Address Addr And A In A

        // OR Operations
        void OR(uint8_t reg);                                                   // Store Result Of Logical OR Of Reg And A In A
        void OR(uint8_t addr1a, uint8_t addr1b);                                // Store Result Of Logical OR Of Byte At Address Addr And A In A

        // CP Operations
        void CP(uint8_t reg);                                                   // Compare Reg With A And Set Flag Z If They're Equal
        void CP(uint8_t addr1a, uint8_t addr1b);                                // Compare Byte At Address Addr With A And Set Flag Z If They're Equal

        // JP Operations
        void JP(uint8_t a16a, uint8_t a16b);                                    // Jump To Address a16                                  
        void JP(uint8_t flag, uint8_t a16a, uint8_t a16b);                      // Jump To Address a16 If Flag Is Set
        void JPN(uint8_t flag, uint8_t a16a, uint8_t a16b);                     // Jump To Address a16 If Flag Is Not Set

        // JR Operations
        void JR(int8_t s8);                                                     // Jump s8 Steps Relative To PC
        void JR(uint8_t flag, int8_t s8);                                       // Jump s8 Steps Relative If Flag Is Set
        void JRN(uint8_t flag, int8_t s8);                                      // Jump s8 Step Relative If Flag Is Not Set
        
        // CALL Operations
        void INTCALL(uint8_t a8);                                               // Call A Subroutine When An Interrupt Is Triggered
        void CALL(uint8_t a16a, uint8_t a16b);                                  // Call A Subroutine
        void CALL(uint8_t flag, uint8_t a16a, uint8_t a16b);                    // Call A Subroutine If Flag Is Set
        void CALLN(uint8_t flag, uint8_t a16a, uint8_t a16b);                   // Call A Subroutine If Flag Is Not Set

        // RET Operations
        void RET();                                                             // Return To Source Program
        void RET(uint8_t flag);                                                 // Return To Source Program If Flag Is 1
        void RETN(uint8_t flag);                                                // Return To Source Program If Flag Is 0
        void RETI();

        // Stack Related Operations
        void POP(uint8_t* reg1a, uint8_t* reg1b);                               // POP Memory Stack Into Reg
        void PUSH(uint8_t reg1a, uint8_t reg1b);                                // PUSH Contents Of Reg Onto The Memory Stack

        // Shift Operations
        void SLA(uint8_t *reg);                                                 // Shift Reg To The Left
        void SLA(uint8_t addr1a, uint8_t addr1b);                               // Shift Byte At Address Addr To The Left
        void SRA(uint8_t *reg);                                                 // Shift Reg To The Right
        void SRA(uint8_t addr1a, uint8_t addr1b);                               // Shift Byte At Addresss Addr To The Right
        void SRL(uint8_t *reg);                                                 // Shift Reg To The Right
        void SRL(uint8_t addr1a, uint8_t addr1b);                               // Shift Byte At Address Addr To The Right

        // SWAP Operations
        void SWAP(uint8_t *reg);                                                // Swap The Higher 4 Bits With The Lower 4 Bits Of Reg
        void SWAP(uint8_t addr1a, uint8_t addr1b);                              // Swap The Higher 4 Bits With The Lower 4 Bits Of The Byte At Address Addr

        // BIT Operations
        void BIT(uint8_t bitNr, uint8_t reg);                                   // Set Z Flag Equal To Bit With BitNr From Reg
        void BIT(uint8_t bitNr, uint8_t addr1a, uint8_t addr1b);                // Set Z Flag Equal To Bit With BitNr From Byte At Address Addr

        // RES Operations
        void RES(uint8_t bitNr, uint8_t* reg);                                  // Set Bit With BitNr From Reg Equal To 0
        void RES(uint8_t bitNr, uint8_t addr1a, uint8_t addr1b);                // Set Bit With BitNr From Byte At Address Addr Equal To 0

        // SET Operations
        void SET(uint8_t bitNr, uint8_t* reg);                                  // Set Bit With BitNr From Reg Equal To 1
        void SET(uint8_t bitNr, uint8_t addr1a, uint8_t addr1b);                // Set Bit With BitNr From Byte At Address Addr Equal To 1

        // Rotate Operations
        void RLCA();                                                            // rotate A To Left
        void RLC(uint8_t *reg);                                                 // Rotate Reg To Left
        void RLC(uint8_t addr1a, uint8_t addr1b);                               // Rotate Byte At Address Addr To Left
        void RRCA();                                                            // Rotate A To Right
        void RRC(uint8_t *reg);                                                 // Rotate Reg To Right
        void RRC(uint8_t addr1a, uint8_t addr1b);                               // Rotate Byte At address Addr To Left    
        void RL(uint8_t *reg);                                                  // Rotate Reg To Left
        void RL(uint8_t addr1a, uint8_t addr1b);                                // Rotate Byte At Address Addr To Left
        void RR(uint8_t *reg);                                                  // Rotate Reg To Right
        void RR(uint8_t addr1a, uint8_t addr1b);                                // Rotate Byte At Address Addr To Right

        // Misc. Operations
        void NOP();                                                             // Do Nothing
        void STOP();                                                            // Stop System Clock And Oscillator
        void HALT();                                                            // Put Emulator In HALT Mode
        void DAA();                                                             // Adjust A To A BCD
        void CPL();                                                             // Flip Contents Of A Register
        void SCF();                                                             // Set Carry Flag
        void CCF();                                                             // Flip Carry Flag
        void DI();
        void EI();
        void RST(uint8_t byteNr);

    public:
        // Public Class Functions
        CPU(MMU* mmu);
        ~CPU();
        
        void reset();
        void step();
};

#endif