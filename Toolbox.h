#pragma once
#include <SFML/Graphics.hpp>

class GameState;
class Button;

class Toolbox{
    private:
        Toolbox()
        {
            _debug.loadFromFile("../images/debug.png");

            sf::IntRect DigitRect;
            DigitRect.height = 32;
            DigitRect.width = 21;
            _digits.loadFromFile("../images/digits.png");
            for (int i = 0; i < 11 ; i++)
            {
                Digits[i] = sf::Sprite(_digits, DigitRect);
                Digits[i].move(sf::Vector2f(0, 512));
                DigitRect.left += 21;
            }

            _face_happy.loadFromFile("../images/face_happy.png");

            _face_lose.loadFromFile("../images/face_lose.png");

            _face_win.loadFromFile("../images/face_win.png");

            _flag.loadFromFile("../images/flag.png");

            _mine.loadFromFile("../images/mine.png");

            for (int i = 0; i < 8; i++)
            {
                _nums[i].loadFromFile("../images/number_"+std::to_string(i+1)+".png");
            }

            _test_1.loadFromFile("../images/test_1.png");
            _test_2.loadFromFile("../images/test_2.png");


            _tile_hidden.loadFromFile("../images/tile_hidden.png");
            _tile_revealed.loadFromFile("../images/tile_revealed.png");
        };
    public:
        static Toolbox &getInstance()
        // : window(element)
        {
            static Toolbox mine;
            return mine;
        };

        //Stupid sfml deleting my render windows
        sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(800, 600), "P4 - Minesweeper, Aiden Thomas");

        bool **BombMap;
        int width;
        int height;
        GameState* gameState;
        Button* debugBut;
        Button* newGameButton;
        Button* testButton1;
        Button* testButton2;

        sf::Texture _debug;

        sf::Texture _digits;
        sf::Sprite Digits[11];

        sf::Texture _face_happy;
        sf::Texture _face_lose;
        sf::Texture _face_win;

        sf::Texture _flag;

        sf::Texture _mine;

        sf::Texture _tile_hidden;

        sf::Texture _tile_revealed;

        sf::Texture _nums[8];
        
        sf::Texture _test_1;
        sf::Texture _test_2;

        bool debugging = false;
        int flags = 0;
};