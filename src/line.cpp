//
// Created by Cedric Gormond on 25/01/2019.
//

#include "line.h"

Line::Line() :
        position(sf::Vector2f()),
        width(20.0f),
        height(20.0f),
        colour(sf::Color())
{
}

Line::Line(float startX, float startY) :
        width(20.0f),
        height(20.0f)
{
    position.x = startX;
    position.y = startY;

    colour = sf::Color::Red;

    block.setSize(sf::Vector2f(width, height));
    block.setFillColor(colour);
    block.setPosition(position);
    block.setOutlineThickness(2);
    block.setOutlineColor(sf::Color(255,165,0));
}

Line::~Line() {
}

void Line::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates) const {
    renderTarget.draw(block);
}
