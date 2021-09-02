#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include <fstream>

class Cartridge{
    private:
        // Private Class Variables
        char data[0x8000];
    
    public:
        // Public Class Functions
        Cartridge();
        ~Cartridge();

        bool loadFile(char* path);
        
        uint8_t read(const uint16_t &address);
};

#endif