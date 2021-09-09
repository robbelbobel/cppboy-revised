#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <stdint.h>
#include <SFML/Graphics.hpp>

#include <iostream>

enum keys{
    KEY_RIGHT,
    KEY_LEFT,
    KEY_UP,
    KEY_DOWN,
    KEY_A,
    KEY_B,
    KEY_SELECT,
    KEY_START
};

class InputHandler{
private:
    // Private Class Variables
    sf::Keyboard::Key keyBindings[8];

public:
    // Private Class Functions
    InputHandler();
    ~InputHandler();

    void update();

    // Private Class Variables
    // JOYP Register
    uint8_t joyp;
};

#endif