#ifndef BLOCKCONTAINER_H_INCLUDED
#define BLOCKCONTAINER_H_INCLUDED

#include "block.h"

class ContainerOfBlocks : public sf::Drawable {
public:
    ContainerOfBlocks();
    ContainerOfBlocks(int useless, const sf::Vector2f pos);
    ~ContainerOfBlocks();

    std::vector<Block> &getContainer();
    void drawContainer(sf::RenderWindow &window);

private:
    virtual void draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates) const;

    std::vector<Block> blockContainer;
};

#endif