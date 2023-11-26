#include "types.h"

namespace kq
{

Entity::Entity() : m_mass(10), m_velocity({0, 0}), m_rotation({0, 0}) {}
Entity::~Entity() {}

float& Entity::getMass() { return m_mass; }
sf::Vector2f& Entity::getVelocity() { return m_velocity; }
sf::Vector2f& Entity::getRotation() { return m_rotation; }
void Entity::setVelocity(sf::Vector2f newVel)
{
	m_velocity = newVel;
}

} // namespace kq