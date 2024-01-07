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
    void setVelocity(sf::Vector2f newVel);

    virtual void move(const sf::Vector2f& offset, float deltaTime) = 0;
    virtual sf::Vector2f nextPosition(const sf::Vector2f& velocity, float deltaTime) = 0;
    virtual void applyForce(const sf::Vector2f& force) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void setPosition(const sf::Vector2f& pos) = 0;
    virtual void setColor(const sf::Color& color) = 0;
    virtual sf::Color getColor() = 0;

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
    sf::Vector2f nextPosition(const sf::Vector2f& velocity, float deltaTime) override;
    void applyForce(const sf::Vector2f& force) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    void setPosition(const sf::Vector2f& pos) override;
    void setColor(const sf::Color& color) override;
    sf::Color getColor() override;

    sfShapeType& getShape();
    bool& getNorth();
    bool& getEast();
    bool& getSouth();
    bool& getWest();


protected:
    sf::Color m_color;
    sfShapeType m_shape;
    bool m_north;
    bool m_east;
    bool m_south;
    bool m_west;
};

struct RigidBody {};

struct DynamicBody 
{
    template<typename sfShapeType>
    static bool outOfBounds(Shape<sfShapeType, DynamicBody>& shape);

    template<typename sfShapeType>
    static void applyForce(Shape<sfShapeType, DynamicBody>& shape, const sf::Vector2f& force);
};

template<typename sfShapeType>
bool DynamicBody::outOfBounds(Shape<sfShapeType, DynamicBody>& shape)
{
    auto& sfShape = shape.getShape();
    sf::FloatRect shapeBounds = sfShape.getGlobalBounds();
    sf::FloatRect windowBounds(0.f, 0.f, static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_LENGTH));

    bool topLeft        = !windowBounds.contains(shapeBounds.left, shapeBounds.top);
    bool topRight       = !windowBounds.contains(shapeBounds.left + shapeBounds.width, shapeBounds.top);
    bool bottomLeft     = !windowBounds.contains(shapeBounds.left, shapeBounds.top + shapeBounds.height);
    bool bottomRight    = !windowBounds.contains(shapeBounds.left + shapeBounds.width, shapeBounds.top + shapeBounds.height);

    auto velocity = shape.getVelocity();

    bool& north = shape.getNorth();
    bool& east = shape.getEast();
    bool& south = shape.getSouth();
    bool& west = shape.getWest();

    // north border
    if(topLeft && topRight)
    {
        velocity.y *= -1.f;
        north = true;
    }
    else north = false;
    // east border
    if(topRight && bottomRight)
    {
        velocity.x *= -1.f;
        east = true;
    }
    else east = false;
    // south border
    if(bottomRight && bottomLeft)
    {
        velocity.y *= -1.f;
        south = true;
    }
    else south = false;
    // west border
    if(bottomLeft && topLeft)
    {
        velocity.x *= -1.f;
        west = true;
    }
    else west = false;


    shape.setVelocity(velocity);
    return false; // should be true
        // but because we flipped the velocity it should be good now...
    

    
    
}

template<typename sfShapeType>
void DynamicBody::applyForce(Shape<sfShapeType, DynamicBody>& shape, const sf::Vector2f& force)
{
    sf::Vector2f acceleration = force / shape.getMass();
    shape.getVelocity() += acceleration;
    
}

template<typename sfShapeType, typename BodyType>
Shape<sfShapeType, BodyType>::Shape()
    : Entity(), BodyType(), m_color(), m_shape(), m_north(false), m_east(false), m_south(false), m_west(false)
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
    : Entity(), BodyType(), m_color(), m_shape(std::forward<Args>(args)...), m_north(false), m_east(false), m_south(false), m_west(false)
{

}

template<typename sfShapeType, typename BodyType>
void Shape<sfShapeType, BodyType>::move(const sf::Vector2f& velocity, float deltaTime)
{
    sf::Vector2f nextPos = nextPosition(velocity, deltaTime);

    //std::cout << "From " << m_shape.getPosition().x << ", " << m_shape.getPosition().y << " to: " << pos.x << ", " << pos.y << '\n';
    
    if(!BodyType::outOfBounds(*this))
    {
        m_shape.move(velocity * deltaTime);
    }
}

template<typename sfShapeType, typename BodyType>
sf::Vector2f Shape<sfShapeType, BodyType>::nextPosition(const sf::Vector2f& velocity, float deltaTime)
{
    sf::Vector2f pos = m_shape.getPosition();
    pos += velocity * deltaTime;
    return pos;
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

template<typename sfShapeType, typename BodyType>
void Shape<sfShapeType, BodyType>::setColor(const sf::Color& color)
{
    m_shape.setFillColor(color);
}

template<typename sfShapeType, typename BodyType>
sf::Color Shape<sfShapeType, BodyType>::getColor()
{
    return m_shape.getFillColor();
}

template<typename sfShapeType, typename BodyType>
sfShapeType& Shape<sfShapeType, BodyType>::getShape() { return m_shape; }

template<typename sfShapeType, typename BodyType>
bool& Shape<sfShapeType, BodyType>::getNorth() { return m_north; }

template<typename sfShapeType, typename BodyType>
bool& Shape<sfShapeType, BodyType>::getEast() { return m_east; }

template<typename sfShapeType, typename BodyType>
bool& Shape<sfShapeType, BodyType>::getSouth() { return m_south; }

template<typename sfShapeType, typename BodyType>
bool& Shape<sfShapeType, BodyType>::getWest() { return m_west; }

} // namespace kq

#endif 