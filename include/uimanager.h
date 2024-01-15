#ifndef PHYSIM_UIMANAGER_H
#define PHYSIM_UIMANAGER_H

#include "common.h"
#include "types.h"
#include <array>

namespace kq
{

class physim;

class UIManager
{
public:
    UIManager(physim* parent);

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
    bool isSelected();
    uint32_t getSelected();
    float getMass();
    
    std::string getShapeName(objectType type);
    void getColorBox(sf::Color color);
    std::array<float, 4> getColor() const;
    

private:

    void editPanel();
    void listPanel();
    void objectPanel();
    void importPanel();
    void exportPanel();

    physim* m_parent;
    bool m_toggle;
    objectType m_type;
    float m_radius;
    float m_rotation;
    sf::Vector2f m_size;
    sf::Vector2f m_velocity;
    bool m_play;
    std::array<float, 4> m_color;
    uint32_t m_selected;
    bool m_showSelected;
    float m_mass;
    bool m_exportMenu;
    bool m_importMenu;
    
    const char* m_types[5] = { "Circle", "Square", "Rectangle", "Triangle", "Convex" };

};

} // namespace kq

#endif