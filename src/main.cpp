#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "gameboy/gameboy.hpp"

void getInput(sf::Window &window);

int main(int argc, char** argv){
    //-----INITIALIZATION-----
    // Create SMFL Window
    sf::RenderWindow window(sf::VideoMode(1000, 500), "cppboy-revised");

    // Create Instance Of Game Boy Class
    Gameboy gameboy;

    // Load Game ROM
    if(argc < 2 || !gameboy.loadFile(argv[1])){
        std::cerr << "Couldn't load file at given path! Check your arguments and restart the emulator." << std::endl;
        return -1;
    }

    //-----MAIN LOOP-----
    while(window.isOpen()){
        // Get User Input
        getInput(window);

        // Step The Emulator
        gameboy.step();
    }

    return 0;
}

void getInput(sf::Window &window){
    sf::Event ev;

    while(window.pollEvent(ev)){
        switch(ev.type){
            case sf::Event::Closed:
                window.close();
                break;

            default:
                break;
        }
    }

    return;
}