#include "Toolbox.h"
#include <array>

class Tile
{
    public:
        enum State{ REVEALED, HIDDEN, FLAGGED, EXPLODED};
        Tile(sf::Vector2f position)
        {
            Position = position;
            RAWPosition = position;
            Position.x *= toolbox._tile_hidden.getSize().x;
            Position.y *= toolbox._tile_hidden.getSize().y;
        };
        ~Tile()
        {
            
        };

        void draw()
        {
            switch (TileState)
            {
            case FLAGGED:
                base = sf::Sprite(toolbox._tile_hidden);
                base.move(Position);
                toolbox.window.draw(base);

                Top = sf::Sprite(toolbox._flag);
                Top.move(Position);
                toolbox.window.draw(Top);
                break;
            case REVEALED:
                base = sf::Sprite(toolbox._tile_revealed);
                base.move(Position);
                toolbox.window.draw(base);

                if (count != 0)
                {
                    Top = sf::Sprite(toolbox._nums[count-1]);
                    Top.move(Position);
                    toolbox.window.draw(Top);
                }
                break;
            case EXPLODED:
                base = sf::Sprite(toolbox._tile_revealed);
                base.move(Position);
                toolbox.window.draw(base);

                Top = sf::Sprite(toolbox._mine);
                Top.move(Position);
                toolbox.window.draw(Top);
                break;
            default:
                base = sf::Sprite(toolbox._tile_hidden);
                base.move(Position);
                toolbox.window.draw(base);
            }
            if (toolbox.debugging == true)
            {
                if (toolbox.BombMap[int(RAWPosition.y)+1][int(RAWPosition.x)+1] == 1)
                {
                    base = sf::Sprite(toolbox._tile_revealed);
                    base.move(Position);
                    toolbox.window.draw(base);

                    Top = sf::Sprite(toolbox._mine);
                    Top.move(Position);
                    toolbox.window.draw(Top);
                }
            }
        };

        sf::Vector2f getLocation()
        {
            return Position;
        };

        State getState()
        {
            return TileState;
        };

        void setState(State _state)
        {
            TileState = _state;
        };

        void revealNeighbors()
        {
            for (int i = 0; i < 8; i++)
            {
                Neighbors[i]->onClickLeft();
            }
        };

        void onClickLeft()
        {
            //Reveal self, if this isnt a number or a bomb, reveal neighbors
            if (toolbox.BombMap[int(RAWPosition.y)+1][int(RAWPosition.x)+1] == 1)
            {
                TileState = EXPLODED;
            }
            else
            {
                if (TileState == HIDDEN)
                {
                    TileState = REVEALED;
                    count = 0;
                    int edgecount = 0;
                    for (int i = -1; i < 2; i++)
                    {
                        for (int j = -1; j < 2; j++)
                        {
                            if (toolbox.BombMap[int(RAWPosition.y)+i+1][int(RAWPosition.x)+j+1] == 1)
                            {
                                count++;
                            }
                            else if (int(RAWPosition.x)+j+1 == -1 || int(RAWPosition.y)+i+1 == 0 || int(RAWPosition.x)+j+1 == toolbox.width+2 || int(RAWPosition.y)+i+1 == toolbox.height+1)
                            {
                                edgecount++;
                            }
                        }
                    }
                    if (count == 0 && edgecount == 0)
                    {
                        revealNeighbors();
                    }
                    else if (int(RAWPosition.y) == 0 && count == 0 && int(RAWPosition.x) != -1 && int(RAWPosition.x) != toolbox.width)
                    {
                        Neighbors[4]->onClickLeft();
                        Neighbors[1]->onClickLeft();
                        Neighbors[2]->onClickLeft();
                        Neighbors[5]->onClickLeft();
                        Neighbors[7]->onClickLeft();
                    }
                    else if (int(RAWPosition.y)+2 == toolbox.height+1 && count == 0 && int(RAWPosition.x) != -1 && int(RAWPosition.x) != toolbox.width)
                    {
                        Neighbors[0]->onClickLeft();
                        Neighbors[1]->onClickLeft();
                        Neighbors[3]->onClickLeft();
                        Neighbors[4]->onClickLeft();
                        Neighbors[6]->onClickLeft();
                    }
                }
            }
        };

        void setNeighbors(std::array<Tile*, 8> _neighbors)
        {
            Neighbors = _neighbors;
        };

        void onClickRight()
        {
            if (!toolbox.debugging)
            {
                if (TileState == HIDDEN)
                {
                    TileState = FLAGGED;
                }
                else if (TileState == FLAGGED)
                {
                    TileState = HIDDEN;
                }
            }
        }

    private:
        /* data */
        int count;
        Toolbox &toolbox = Toolbox::getInstance();
        sf::Vector2f Position;
        sf::Vector2f RAWPosition;
        sf::Sprite base;
        sf::Sprite Top;
        std::array<Tile*, 8> Neighbors;
        State TileState = HIDDEN;
};