#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>

#define WIDTH 512
#define HEIGHT WIDTH // square window

#define SQ_SIZE 8 // square size in pixel has to be the same solution of a common divisor of WIDTH & HEIGHT, has to be > 1
#define SQUARES WIDTH/SQ_SIZE // amount of squares (cells)

//#define USE_MACROS 1 // uncomment to define the variables below

#ifdef USE_MACROS
#define MAX_RUNS 50
#define STEPS 100 // steps each random walk takes
#define STEP_SPEED_MS 0
#endif // USE_MACROS

#ifndef USE_MACROS
int MAX_RUNS;// = 50;
int STEPS;// = 100; // steps each random walk takes
int STEP_SPEED_MS;// = 0;
#endif // USE_MACROS

void initSquares(sf::RectangleShape Cells[SQUARES][SQUARES], int square_size) {
    for(int i = 0; i < SQUARES; i++) {
        for(int j = 0; j < SQUARES; j++) {
            Cells[i][j].setSize(sf::Vector2f(square_size, square_size));
            Cells[i][j].setFillColor(sf::Color::White); // Grey
            Cells[i][j].setOutlineThickness(1);
            Cells[i][j].setOutlineColor(sf::Color::Black);
        }
    }
}

void setPos(sf::RectangleShape Cells[SQUARES][SQUARES], int square_size) {
    for(int i = 0; i < SQUARES; i++) { // Set Position
        for(int j = 0; j < SQUARES; j++) {
            Cells[i][j].setPosition(i*square_size, j*square_size);
        }
    }
}

void randomWalk(sf::RectangleShape Cells[SQUARES][SQUARES], sf::RenderWindow &wind) {
    int start_index_x = SQUARES/2;
    int start_index_y = SQUARES/2;
    std::string dir[4] = {"Up", "Down", "Left", "Right"};
    int R = rand()%256;
    int G = rand()%256;
    int B = rand()%256;
    for(int i = 0; i < STEPS; i++) {
        int n = rand()%4;
        if((start_index_x > 0 && start_index_x < SQUARES) && (start_index_y > 0 && start_index_y < SQUARES)) {
            if(dir[n] == "Up")
                Cells[start_index_x][start_index_y--].setFillColor(sf::Color(R,G,B));
            else if(dir[n] == "Down")
                Cells[start_index_x][start_index_y++].setFillColor(sf::Color(R,G,B));
            else if(dir[n] == "Left")
                Cells[start_index_x--][start_index_y].setFillColor(sf::Color(R,G,B));
            else if(dir[n] == "Right")
                Cells[start_index_x++][start_index_y].setFillColor(sf::Color(R,G,B));

            // draw Cells
            wind.clear();
            for(int i = 0; i < SQUARES; i++) {
                for(int j = 0; j < SQUARES; j++) {
                    wind.draw(Cells[i][j]);
                }
            }
            wind.display();
            sf::sleep(sf::milliseconds(STEP_SPEED_MS));
        }
    }
}

void take_screenshot(const sf::RenderWindow& wnd) {
    sf::Texture texture;
    texture.create(wnd.getSize().x, wnd.getSize().y);
    texture.update(wnd);
    if (texture.copyToImage().saveToFile("screen.jpg"))
        std::cout << "Screenshot saved" << std::endl;
    else
        std::cout << "Error taking screenshot..." << std::endl;
}


int main() {
    sf::RenderWindow wind(sf::VideoMode(WIDTH, HEIGHT), "Random Walk Visualization by Davis R.", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    sf::Event evnt;
    srand (time(NULL));

    wind.setSize(sf::Vector2u(WIDTH, HEIGHT)); // Resizing window with smaller Resolution to increase fps

#ifndef USE_MACROS
    std::cout << "Random Walk by Davis R.\n" << std::endl;
    std::cout << "Please type the max number of runs (f.e. 50): ";
    std::cin >> MAX_RUNS;
    std::cout << "Please type the max number of steps each run takes (f.e. 100): ";
    std::cin >> STEPS;
    std::cout << "Please type the step speed in ms (usually 0): ";
    std::cin >> STEP_SPEED_MS;
#endif // USE_MACROS

    auto Cells = new sf::RectangleShape[SQUARES][SQUARES]; // WIDTH/SQ_SIZE = SQUARES (32)

    /// Initialize array of Cells
    initSquares(Cells, SQ_SIZE);
    /// Set position of each Cell
    setPos(Cells, SQ_SIZE);

    int Nr_Tries = 0; // Random Walk counter

    while(wind.isOpen()) {
        while(wind.pollEvent(evnt)) {
            switch (evnt.type) {
            case sf::Event::Closed:
                wind.close();
                break;
            case sf::Event::TextEntered:
                if(evnt.text.unicode < 128) {
                    if(evnt.text.unicode == 27)
                        wind.close();
                    break;
                }
            }
        } /// Main loop

        if(Nr_Tries < MAX_RUNS) {
            randomWalk(Cells, wind);
            Nr_Tries++;
        }

        // screenshot
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F12))
            take_screenshot(wind);

        // draw loop
        wind.clear();
        for(int i = 0; i < SQUARES; i++) {
            for(int j = 0; j < SQUARES; j++)
                wind.draw(Cells[i][j]);
        }
        wind.display();
    }
    for(int i = 0; i < WIDTH; i++)
        delete [] Cells[i];
    delete [] Cells;
    return 0;
}
