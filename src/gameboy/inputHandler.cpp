#include "inputHandler.hpp"

InputHandler::InputHandler(){
    // Initialize Private Class Variables
    InputHandler::joyp = 0xFF;

    InputHandler::keyBindings[KEY_RIGHT]    = sf::Keyboard::Right;
    InputHandler::keyBindings[KEY_LEFT]     = sf::Keyboard::Left;
    InputHandler::keyBindings[KEY_UP]       = sf::Keyboard::Up;
    InputHandler::keyBindings[KEY_DOWN]     = sf::Keyboard::Down;
    InputHandler::keyBindings[KEY_A]        = sf::Keyboard::A;
    InputHandler::keyBindings[KEY_B]        = sf::Keyboard::B;
    InputHandler::keyBindings[KEY_SELECT]   = sf::Keyboard::Backspace;
    InputHandler::keyBindings[KEY_START]    = sf::Keyboard::Return;
}

InputHandler::~InputHandler(){
    ;
}

void InputHandler::update(){
    // Check Direction Buttons
    if(((InputHandler::joyp >> 4) & 0b1) == 0){
        // Clear Buttons
        InputHandler::joyp &= ~0x0F;

        // Fetch Button States
        InputHandler::joyp += (!sf::Keyboard::isKeyPressed(InputHandler::keyBindings[KEY_RIGHT]));
        InputHandler::joyp += (!sf::Keyboard::isKeyPressed(InputHandler::keyBindings[KEY_LEFT])) << 1;
        InputHandler::joyp += (!sf::Keyboard::isKeyPressed(InputHandler::keyBindings[KEY_UP])) << 2;
        InputHandler::joyp += (!sf::Keyboard::isKeyPressed(InputHandler::keyBindings[KEY_DOWN])) << 3;
    }

    // Check Action Buttons
    if(((InputHandler::joyp >> 5) & 0b1) == 0){
        // Clear Buttons
        InputHandler::joyp &= ~0x0F;

        // Fetch Button States
        InputHandler::joyp += (!sf::Keyboard::isKeyPressed(InputHandler::keyBindings[KEY_A]));
        InputHandler::joyp += (!sf::Keyboard::isKeyPressed(InputHandler::keyBindings[KEY_B])) << 1;
        InputHandler::joyp += (!sf::Keyboard::isKeyPressed(InputHandler::keyBindings[KEY_SELECT])) << 2;
        InputHandler::joyp += (!sf::Keyboard::isKeyPressed(InputHandler::keyBindings[KEY_START])) << 3;
    }
}