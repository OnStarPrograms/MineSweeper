#include "Toolbox.h"
#include "Button.h"
#include "GameState.h"
#include <string>

int launch();
void restart();
void toggleDebugMode();
bool getDebugMode();
void render();
int gameloop();
void Testers(std::string Stringer);
void CheckButton(Button* button, sf::Vector2i mouser);

/*
THINGS TO REMEMBER

The Tile map needs to be shifted (+1) due to having a 
compensation border.
*/


int main(){
    return launch();
}

int launch()
{
    Toolbox &toolbox = Toolbox::getInstance();
    toolbox.debugBut = new Button(
                    sf::Vector2f(500, 512), 
                    [](void)
                    {
                        toggleDebugMode();
                    });
    toolbox.debugBut->setSprite(new sf::Sprite(toolbox._debug));


    toolbox.newGameButton = new Button(
                    sf::Vector2f(370, 512), 
                    [](void)
                    {
                        restart();
                    });
    toolbox.newGameButton->setSprite(new sf::Sprite(toolbox._face_happy));

    toolbox.testButton1 = new Button(
                    sf::Vector2f(565, 512), 
                    [](void)
                    {
                        Testers("../boards/testboard1.brd");
                    });
    toolbox.testButton1->setSprite(new sf::Sprite(toolbox._test_1));

    toolbox.testButton2 = new Button(
                    sf::Vector2f(630, 512), 
                    [](void)
                    {
                        Testers("../boards/testboard2.brd");
                    });
    toolbox.testButton2->setSprite(new sf::Sprite(toolbox._test_2));
    toolbox.gameState = new GameState();

    return gameloop();
}

void restart()
{
    Toolbox &toolbox = Toolbox::getInstance();
    toolbox.debugging = false;
    toolbox.gameState = new GameState();
    toolbox.newGameButton->setSprite(new sf::Sprite(toolbox._face_happy));
}

void toggleDebugMode()
{
    Toolbox &toolbox = Toolbox::getInstance();
    if (toolbox.debugging == false)
    toolbox.debugging = true;
    else
    toolbox.debugging = false;
}

bool getDebugMode()
{
    Toolbox &toolbox = Toolbox::getInstance();
    return toolbox.debugging;
}

void render()
{
    Toolbox &toolbox = Toolbox::getInstance();
    //toolbox.window->draw(Buttons)
    int counter = 0;
    int secCounter = 0;
    for (int i = 1; i < 17; i++)
    {
        for (int j = 1; j < 26; j++)
        {
            toolbox.gameState->getTile(i, j)->draw();
            if (toolbox.gameState->getTile(i, j)->getState() != Tile::HIDDEN)
            {
                counter++;
            }
            else if (toolbox.BombMap[i][j] == 1)
            {
                counter++;
            }
            secCounter++;
        }
    }
    if (counter == secCounter)
    {
        toolbox.gameState->setPlayStatus(toolbox.gameState->WIN);
    }
    toolbox.window->draw(*toolbox.debugBut->getSprite());
    toolbox.window->draw(*toolbox.newGameButton->getSprite());
    toolbox.window->draw(*toolbox.testButton1->getSprite());
    toolbox.window->draw(*toolbox.testButton2->getSprite());

    if (toolbox.gameState->getMineCount() - toolbox.flags < 0)
    {
    toolbox.window->draw(toolbox.Digits[10]);
    }
    else
    {
    toolbox.window->draw(toolbox.Digits[std::abs(int((toolbox.gameState->getMineCount() - toolbox.flags)/100)%10)]);
    }
    sf::Sprite firss = toolbox.Digits[std::abs(int((toolbox.gameState->getMineCount() - toolbox.flags)/10)%10)];
    firss.move(sf::Vector2f(21, 0));
    toolbox.window->draw(firss);

    sf::Sprite seconds = toolbox.Digits[std::abs(int(toolbox.gameState->getMineCount() - toolbox.flags)%10)];
    seconds.move(sf::Vector2f(42, 0));
    toolbox.window->draw(seconds);
};

int gameloop()
{
    Toolbox &toolbox = Toolbox::getInstance();
    sf::Mouse mouse;


    toolbox.window->isOpen();
    toolbox.window->setFramerateLimit(60);

    while (toolbox.window->isOpen())
    {
        //Mouse Tracking 
        sf::Vector2i mousevec = mouse.getPosition(*toolbox.window);
        if (toolbox.gameState->getPlayStatus() == toolbox.gameState->LOSS)
        {
            toolbox.newGameButton->setSprite(new sf::Sprite(toolbox._face_lose));
        }
        else if (toolbox.gameState->getPlayStatus() == toolbox.gameState->WIN)
        {
            toolbox.newGameButton->setSprite(new sf::Sprite(toolbox._face_win));
        }


        // Event processing
        // if ()
        sf::Event event;
        while (toolbox.window->pollEvent(event))
        {
            
            // Request for closing the window
            if (event.type == sf::Event::Closed)
                toolbox.window->close();
                // // Requested mouse button is a left click and run all the actions
                if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Button::Left))
                {
                    if (toolbox.gameState->getPlayStatus() == toolbox.gameState->PLAYING)
                    {
                        // if it was a tile
                        if ((mousevec.x < 800) && (mousevec.y < 512))
                        {
                            toolbox.gameState->getTile(int(mousevec.y/toolbox._tile_hidden.getSize().y)+1, int(mousevec.x/toolbox._tile_hidden.getSize().x)+1)->onClickLeft();
                        }
                    }
                    //if it was a button
                    CheckButton(toolbox.debugBut, mousevec);
                    CheckButton(toolbox.newGameButton, mousevec);
                    CheckButton(toolbox.testButton1, mousevec);
                    CheckButton(toolbox.testButton2, mousevec);
                }
                if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Button::Right) && (mousevec.x < 800) && (mousevec.y < 480))
                {
                    toolbox.gameState->getTile(int(mousevec.y/toolbox._tile_hidden.getSize().y)+1, int(mousevec.x/toolbox._tile_hidden.getSize().x)+1)->onClickRight();\
                    if (!toolbox.debugging)
                    {
                        if (toolbox.gameState->getTile(int(mousevec.y/toolbox._tile_hidden.getSize().y)+1, int(mousevec.x/toolbox._tile_hidden.getSize().x)+1)->getState() == Tile::FLAGGED)
                        toolbox.flags++;
                        else if (toolbox.gameState->getTile(int(mousevec.y/toolbox._tile_hidden.getSize().y)+1, int(mousevec.x/toolbox._tile_hidden.getSize().x)+1)->getState() != Tile::REVEALED)
                        toolbox.flags--;
                    }
                }
            if (toolbox.gameState->getPlayStatus() == toolbox.gameState->WIN || toolbox.gameState->getPlayStatus() == toolbox.gameState->LOSS)
            {
                if (getDebugMode() == false)
                {
                    toggleDebugMode();
                }
            }
        }
        // Clear the whole window before rendering a new frame
        toolbox.window->clear();
        
        // Draw some graphical entities
        // window->draw(sprite);
        // toolbox.window->draw(sf::RectangleShape());
        render();

        // End the current frame and display its contents on screen
        toolbox.window->display();
    }
    return 0;
}

void CheckButton(Button* button, sf::Vector2i mouser)
{
    Toolbox &toolbox = Toolbox::getInstance();
    if (button->getPosition().x <= mouser.x &&
        button->getPosition().x + toolbox._debug.getSize().x > mouser.x &&
        512 <= mouser.y &&
        512 + toolbox._debug.getSize().y > mouser.y)
    {
        button->onClick();
    }
}

void Testers(std::string Stringer)
{
    Toolbox &toolbox = Toolbox::getInstance();
    toolbox.debugging = false;
    toolbox.gameState = new GameState(Stringer.c_str());
    toolbox.newGameButton->setSprite(new sf::Sprite(toolbox._face_happy));
}