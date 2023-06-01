#ifndef PHYSIM_TYPES_H
#define PHYSIM_TYPES_H

#include "common.h"

namespace kq
{

struct Entity
{
public:
    Entity();
    virtual ~Entity();

    float& getMass();
    sf::Vector2f& getVelocity();
    sf::Vector2f& getRotation();

    virtual void move(const sf::Vector2f& offset, float deltaTime) = 0;
    virtual void applyForce(const sf::Vector2f& force) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void setPosition(const sf::Vector2f& pos) = 0;

protected:
    float m_mass;
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

    void move(const sf::Vector2f& offset, float deltaTime) override;
    void applyForce(const sf::Vector2f& force) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    void setPosition(const sf::Vector2f& pos) override;
protected:
    sfShapeType m_shape;
};

struct RigidBody {};

struct DynamicBody 
{
    static bool outOfBounds(float x, float y, float width, float length);

    template<typename sfShapeType>
    static void applyForce(Shape<sfShapeType, DynamicBody>& shape, const sf::Vector2f& force);
};



template<typename sfShapeType>
void DynamicBody::applyForce(Shape<sfShapeType, DynamicBody>& shape, const sf::Vector2f& force)
{
    sf::Vector2f acceleration = force / shape.getMass();
    shape.getVelocity() += acceleration;
    
}

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
void Shape<sfShapeType, BodyType>::move(const sf::Vector2f& offset, float deltaTime)
{
    sf::Vector2f pos = m_shape.getPosition();
    pos += offset;
    if(!BodyType::outOfBounds(offset.x, offset.y, SCREEN_WIDTH, SCREEN_LENGTH))
    {
        m_shape.move(m_velocity * deltaTime);
    }
}

template<typename sfShapeType, typename BodyType>
void Shape<sfShapeType, BodyType>::applyForce(const sf::Vector2f& force)
{
    BodyType::applyForce(*this, force);
}

template<typename sfShapeType, typename BodyType>
void Shape<sfShapeType, BodyType>::update(float deltaTime)
{
    move(m_velocity, deltaTime);
}

template<typename sfShapeType, typename BodyType>
void Shape<sfShapeType, BodyType>::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}

template<typename sfShapeType, typename BodyType>
void Shape<sfShapeType, BodyType>::setPosition(const sf::Vector2f& pos)
{
    m_shape.setPosition(pos);
}


} // namespace kq

#endif 