#include "types.h"

namespace kq
{

bool DynamicBody::outOfBounds(float x, float y, float width, float length)
{
    return (x < 0 || x > width  || y < 0 || y > length);
}

Entity::Entity() : m_mass(10), m_velocity({0, 0}), m_rotation({0, 0}) {}
Entity::~Entity() {}

float& Entity::getMass() { return m_mass; }
sf::Vector2f& Entity::getVelocity() { return m_velocity; }
sf::Vector2f& Entity::getRotation() { return m_rotation; }

} // namespace kq