#ifndef PHYSIM_H
#define PHYSIM_H

#include "common.h"
#include "types.h"
#include "uimanager.h"
#include "collider.h"

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

    void createObject(objectType type, float rotation, float m_radius, sf::Vector2f m_size, sf::Vector2f m_velocity);

    uint16_t m_width;
    uint16_t m_height;
    sf::RenderWindow m_window;

    UIManager m_UIManager;

    std::vector<std::unique_ptr<Entity>> m_entities;
};

} // namespace kq

#endif