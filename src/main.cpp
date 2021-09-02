#include "gameboy/gameboy.hpp"

#define GAMEBOY_SPEED 4194304
#define UPDATE_SPEED 60

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

    // SFML Clocks For Timing
    sf::Clock stepClock;
    sf::Clock updateClock;
    sf::Clock timeClock;
    stepClock.restart();
    updateClock.restart();

    uint64_t stepCount = 0;

    //-----MAIN LOOP-----
    while(window.isOpen()){
        // Step The Emulator
        if(stepClock.getElapsedTime().asMicroseconds() >= (1000000 / GAMEBOY_SPEED)){
            // Step Emulator
            gameboy.step();
            stepCount++;

            // Restart Clock
            stepClock.restart();
        }

        if(updateClock.getElapsedTime().asMilliseconds() >= (1000 / UPDATE_SPEED)){
            // Get User Input
            getInput(window);

            // Draw Emulator
            gameboy.draw(window);

            // Restart Clock
            updateClock.restart();
        }
    }

    std::cout << "stepcount: " << stepCount << std::endl;

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