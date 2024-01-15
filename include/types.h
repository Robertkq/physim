#ifndef PHYSIM_TYPES_H
#define PHYSIM_TYPES_H

#include "common.h"

namespace kq
{

class physicalObjectArgs;
class physicalObject;
class Circle;
class Square;
class Triangle;
class Rectangle;

class physicalObjectArgs 
{
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;
    float mass;
    objectType type;
    float angularVelocity;
    float orientation;

    physicalObjectArgs(const sf::Vector2f& position, const sf::Vector2f& velocity,
                       const sf::Color& color, float mass, objectType type);
};

class physicalObject 
{
public:

    physicalObject(physicalObjectArgs&& args);

    // Common methods for all shapes.
    virtual void move(const sf::Vector2f& offset, float deltaTime) = 0;
    virtual void applyForce(const sf::Vector2f& force) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual objectType getType() const = 0;
    virtual bool collidesWith(const physicalObject& other) const = 0;

    sf::Vector2f& getPosition();
    sf::Vector2f& getVelocity();
    sf::Color& getColor();
    float& getMass();
    objectType& getObjectType();

    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;
    sf::Color getColor() const;
    float getMass() const;
    float getInvMass() const;
    objectType getObjectType() const;
    uint32_t getCollisions() const;

    void applyGravity(float deltaTime);
    void applyAirResistance(float deltaTime);
    virtual std::string toCSVString() const = 0;

    // ... other common methods ...

    static float crossProduct(const sf::Vector2f& a, const sf::Vector2f& b);
    static float dotProduct(const sf::Vector2f& a, const sf::Vector2f& b);
    static sf::Vector2f normalize(const sf::Vector2f& vec);
    static void resolveCollision(physicalObject& obj1, physicalObject& obj2);
    static float restitution;
    static bool m_outline;
    static float m_gravity;
    static float m_airResistance;
    static float m_timeAcceleration;

    
protected:
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    sf::Color m_color;
    float m_mass;
    objectType m_type;
    uint32_t m_collisions;
    // ... other common attributes ...
};


class Circle : public physicalObject 
{
public:
    Circle(physicalObjectArgs&& args, float radius);

    void move(const sf::Vector2f& offset, float deltaTime) override;

    void applyForce(const sf::Vector2f& force) override;

    void update(float deltaTime) override;

    void draw(sf::RenderWindow& window) const override;

    objectType getType() const override;

    bool collidesWith(const physicalObject& other) const override;

    bool collidesWith(const Circle& other) const;

    bool collidesWith(const Square& other) const;

    bool collidesWith(const Triangle& other) const;

    bool collidesWith(const Rectangle& other) const;

    bool containsPoint(sf::Vector2f point) const;

    std::string Circle::toCSVString() const override;

private:
    float m_radius;
};

class Square : public physicalObject 
{
public:
    Square(physicalObjectArgs&& args, float sideLength);

    void move(const sf::Vector2f& offset, float deltaTime) override;

    void applyForce(const sf::Vector2f& force) override;

    void update(float deltaTime) override;

    void draw(sf::RenderWindow& window) const override;

    objectType getType() const override;

    bool collidesWith(const physicalObject& other) const;

    bool collidesWith(const Circle& other) const;

    bool collidesWith(const Square& other) const;

    bool collidesWith(const Triangle& other) const;

    bool collidesWith(const Rectangle& other) const;

    float getSideLength() const;

    std::array<sf::Vector2f, 4> getVertices() const; 

    bool containsPoint(sf::Vector2f point) const;

    std::string toCSVString() const override;

private:
    float m_sideLength;
};

class Triangle : public physicalObject {
public:
    Triangle(physicalObjectArgs&& args, float sideLength);

    void move(const sf::Vector2f& offset, float deltaTime) override;

    void applyForce(const sf::Vector2f& force) override;

    void update(float deltaTime) override;

    void draw(sf::RenderWindow& window) const override;

    objectType getType() const override;

    bool collidesWith(const physicalObject& other) const;

    bool collidesWith(const Circle& other) const;

    bool collidesWith(const Square& other) const;

    bool collidesWith(const Triangle& other) const;

    bool collidesWith(const Rectangle& other) const;

    float getSideLength() const;

    std::array<sf::Vector2f, 3> Triangle::getVertices() const;

    bool containsPoint(sf::Vector2f point) const;

    std::string toCSVString() const override;

private:
    float m_sideLength;
};

class Rectangle : public physicalObject {
public:
    Rectangle(physicalObjectArgs&& args, float width, float height);

    void move(const sf::Vector2f& offset, float deltaTime) override;

    void applyForce(const sf::Vector2f& force) override;

    void update(float deltaTime) override;

    void draw(sf::RenderWindow& window) const override;

    objectType getType() const override;

    bool collidesWith(const physicalObject& other) const;

    bool collidesWith(const Circle& other) const;

    bool collidesWith(const Square& other) const;

    bool collidesWith(const Triangle& other) const;

    bool collidesWith(const Rectangle& other) const;

    float getWidth() const;

    float getHeight() const;

    bool containsPoint(sf::Vector2f point) const;

    std::array<sf::Vector2f, 4> getVertices() const;

    std::string toCSVString() const override;

private:
    float m_width;
    float m_height;
};



} // namespace kq

#endif 