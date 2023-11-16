#ifndef PHYSIM_UIMANAGER_H
#define PHYSIM_UIMANAGER_H

#include "common.h"

namespace kq
{

class UIManager
{
public:
    UIManager();

    void toggle();
    bool isActive() const;
    void show();

    void play();
    void pause();
    bool isPlaying();

    objectType getType();
    float getRadius();
    float getRotation();
    sf::Vector2f getSize();
    sf::Vector2f getVelocity();

private:

    void editPanel();

    bool m_toggle;
    objectType m_type;
    float m_radius;
    float m_rotation;
    sf::Vector2f m_size;
    sf::Vector2f m_velocity;
    bool m_play;
    
    const char* m_types[5] = { "Circle", "Square", "Rectangle", "Triangle", "Convex" };

};

} // namespace kq

#endif