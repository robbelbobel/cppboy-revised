#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP


#include <iostream>
#include <fstream>

class Cartridge{
    private:
        // Private Class Variables
        char data[0x800000];

        char romBank0[0x4000];
        char romBank1[0x4000];
        char ramBank[0x2000];

        char ramSlots[4][0x2000];

        // Registers
        uint8_t ramEnable;
        uint8_t romNumber;
        uint8_t ramNumber;
        uint8_t bankMode;

        // Private Class Functions
        void loadHeader();
        void updateROMBank();
        void updateRAMBank();

    public:
        // Public Class Functions
        Cartridge();
        ~Cartridge();

        bool loadFile(char* path);
        
        uint8_t read(const uint16_t &address);
        void write(const uint16_t &address, const uint8_t &value);

        // Public Class Variables
        // Cartridge Header Data
        char title[16];
        char manCode[4];
        char type;
        char romSize;
        char ramSize;
};

#endif