#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

#include <SFML/Graphics.hpp>

class Block : public sf::Drawable {
public:
    Block();
    Block(float startX, float startY);
    virtual ~Block();

private:
    virtual void draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates) const;

    sf::RectangleShape block;
    sf::Vector2f position;
    float width;
    float height;
    sf::Color colour;
};

#endif