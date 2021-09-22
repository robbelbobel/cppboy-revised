#include "gameboy/gameboy.hpp"

#define GAMEBOY_SPEED 4194304
#define DRAW_SPEED 60

#define DRAW_HEIGHT 144
#define DRAW_WIDTH 160

void getInput(sf::RenderWindow &window, sf::View &view);
void updateView(sf::RenderWindow &window, sf::View &view);

int main(int argc, char** argv){
    //-----INITIALIZATION-----
    // Create SMFL Window
    sf::RenderWindow window(sf::VideoMode(512, 512), "cppboy-revised", sf::Style::Default);

    // Create SFML View
    sf::View view(sf::Vector2f(80, 72), sf::Vector2f(160, 144));
    window.setView(view);

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
    stepClock.restart();
    updateClock.restart();

    //-----MAIN LOOP-----
    while(window.isOpen()){
        // Step The Emulator
        if(stepClock.getElapsedTime().asMicroseconds() >= (1000000 / GAMEBOY_SPEED)){
            // Step Emulator
            gameboy.step();

            // Restart Clock
            stepClock.restart();
        }

        if(updateClock.getElapsedTime().asMilliseconds() >= (1000 / DRAW_SPEED)){
            // Get User Input
            getInput(window, view);

            // Update Window
            gameboy.draw(window);

            // Restart Clock
            updateClock.restart();
        }
    }

    return 0;
}

void getInput(sf::RenderWindow &window, sf::View &view){
    sf::Event ev;

    while(window.pollEvent(ev)){
        switch(ev.type){
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::Resized:
                updateView(window, view);
                break;

            default:
                break;
        }
    }

    return;
}

void updateView(sf::RenderWindow &window, sf::View &view){
    view.setSize(sf::Vector2f((float) window.getSize().x / window.getSize().y * DRAW_HEIGHT, DRAW_HEIGHT));

    // Update Window View
    window.setView(view);
}