//
// Created by Cedric Gormond on 25/01/2019.
//

#include "lineContainer.h"

ContainerOfLines::ContainerOfLines() {
}

ContainerOfLines::ContainerOfLines(int useless, const sf::Vector2f pos) {
    if (useless > 0) {
        float x = pos.x;
        float y = pos.y;
        for (std::size_t i = 0; i < static_cast<std::size_t>(useless); ++i) {
            blockContainer.push_back(Line(x, y));
            x += 50.0f;
        }
    }
}

ContainerOfLines::~ContainerOfLines() {
}

std::vector<Line> &ContainerOfLines::getContainer() {
    return blockContainer;
}

void ContainerOfLines::drawContainer(sf::RenderWindow &window) {
    for (std::size_t i = 0; i < blockContainer.size(); ++i) {
        window.draw(blockContainer[i]);
    }
}

void ContainerOfLines::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates) const {
    for (std::size_t i = 0; i < blockContainer.size(); ++i) {
        renderTarget.draw(blockContainer[i]);
    }
}