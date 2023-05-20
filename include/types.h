#ifndef PHYSIM_TYPES_H
#define PHYSIM_TYPES_H

#include "common.h"

namespace kq
{

struct RigidBody {};

struct DynamicBody 
{
    static bool outOfBounds(float x, float y, float width, float length);

    template<typename sfShape>
    static void applyPhysics(sfShape& shape);
};

template<typename sfShape>
void DynamicBody::applyPhysics(sfShape& shape)
{
    sfShape copy = shape;
    copy.move(10, 10);
}

struct Entity
{
public:
    Entity();
    virtual ~Entity();

    virtual void applyPhysics() = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

protected:
    sf::Vector2f m_velocity;
    sf::Vector2f m_rotation;
};

template<typename sfShapeType, typename BodyType> 
struct Shape : public Entity, public BodyType
{
public:
    Shape();
    ~Shape() override;

    template<typename... Args>
    Shape(Args&&... args);

    void applyPhysics() override;
    void draw(sf::RenderWindow& window) override;
protected:
    sfShapeType m_shape;
};

template<typename sfShapeType, typename BodyType>
Shape<sfShapeType, BodyType>::Shape()
    : Entity(), BodyType(), m_shape()
{
    // Constructor implementation
}

template<typename sfShapeType, typename BodyType>
Shape<sfShapeType, BodyType>::~Shape()
{
    // Destructor implementation
}

template<typename sfShapeType, typename BodyType>
template<typename... Args>
Shape<sfShapeType, BodyType>::Shape(Args&&... args)
    : Entity(), BodyType(), m_shape(std::forward<Args>(args)...)
{

}

template<typename sfShapeType, typename BodyType>
void Shape<sfShapeType, BodyType>::applyPhysics()
{
    BodyType::applyPhysics(m_shape);
}

template<typename sfShapeType, typename BodyType>
void Shape<sfShapeType, BodyType>::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}


} // namespace kq

#endif 