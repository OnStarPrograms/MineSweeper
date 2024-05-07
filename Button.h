#include "Toolbox.h"
#include <functional>

class Button
{
private:
    /* data */
    sf::Vector2f Position;
    Toolbox &toolbox = Toolbox::getInstance();
    std::function<void(void)> OnClick;
    sf::Sprite* ButtSprite;
public:
    Button(sf::Vector2f _position, std::function<void(void)> _onClick)
    {
        Position = _position;
        OnClick = _onClick;
    };

    sf::Vector2f getPosition()
    {
        return Position;
    };

    sf::Sprite* getSprite()
    {
        return ButtSprite;
    };

    void setSprite(sf::Sprite* _sprite)
    {
        ButtSprite = _sprite;
        ButtSprite->move(Position);
    };

    void onClick()
    {
        OnClick();
    };

    ~Button()
    {
        
    };
};
