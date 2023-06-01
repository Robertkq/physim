#ifndef PHYSIM_H
#define PHYSIM_H

#include "common.h"
#include "types.h"

namespace kq
{

class physim
{
public:
    physim();
    ~physim();
    
    void run();

private:
    void drawObjects();
    void updateObjects(float deltaTime);
    void mainMenu();

    uint16_t m_width;
    uint16_t m_height;
    sf::RenderWindow m_window;
    std::vector<std::unique_ptr<Entity>> m_entities;
};

}

#endif