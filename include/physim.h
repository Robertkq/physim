#ifndef PHYSIM_H
#define PHYSIM_H

#include "common.h"
#include "types.h"
#include "uimanager.h"
#include "collider.h"
#include "fileManager.h"

namespace kq
{

class physim
{
public:
    physim();
    ~physim();
    
    void run();
    const std::vector<physicalObject*>& getEntities() const;
    std::vector<physicalObject*>& getEntities();
    fileManager& getFileManager();
    void clearEntities();
    void Impulse();
    void createObject(objectType type, float rotation, float radius, sf::Vector2f size,
                 sf::Vector2f velocity, std::array<float, 4> colors, float mass);

private:
    void drawObjects();
    void updateObjects(float deltaTime);
    void mainMenu();

    

    

    uint16_t m_width;
    uint16_t m_height;
    sf::RenderWindow m_window;

    UIManager m_UIManager;

    std::vector<physicalObject*> m_entities;
    fileManager m_fileManager;
};

} // namespace kq

#endif