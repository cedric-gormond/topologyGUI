//
// Created by Cedric Gormond on 25/01/2019.
//

#ifndef TOPOLOGYGUI_LINECONTAINER_H
#define TOPOLOGYGUI_LINECONTAINER_H

#include "line.h"

class ContainerOfLines : public sf::Drawable {
public:
    ContainerOfLines();
    ContainerOfLines(int useless, const sf::Vector2f pos);
    ~ContainerOfLines();

    std::vector<Line> &getContainer();
    void drawContainer(sf::RenderWindow &window);

private:
    virtual void draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates) const;

    std::vector<Line> blockContainer;
};


#endif //TOPOLOGYGUI_LINECONTAINER_H
