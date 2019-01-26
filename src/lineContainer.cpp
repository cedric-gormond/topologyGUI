//
// Created by Cedric Gormond on 25/01/2019.
//

#include "lineContainer.h"
#include "ctr.hpp"
#include "gen_tools.h"

ContainerOfLines::ContainerOfLines() {
}

ContainerOfLines::ContainerOfLines(int size, const sf::Vector2f pos, constraint *ctr, int distance) {

    int max_X = max_genX(ctr, size);
    int max_Y = max_genY(ctr, size);

    for (int j = 0; j < max_X; ++j) {
        for (int i = 0; i < max_Y; ++i) {
            blockContainer.push_back(Line(ctr[j].CenterX - 1, ctr[j].CenterY - 1, ctr[j].width, ctr[j].heigth, distance, 2)); //-1 pour etre au centre
        }
        blockContainer.push_back(Line(ctr[j].CenterX - 1, ctr[j].CenterY - 1, ctr[j].width,ctr[j].heigth, 2, distance)); //-1 pour etre au centre
    }

    //float x = pos.x;
    //float y = pos.y;
    //for (std::size_t i = 0; i < static_cast<std::size_t>(size); ++i) {
    //    blockContainer.push_back(Line(x, y, distance));
    //    x += 50.0f;
    //}
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