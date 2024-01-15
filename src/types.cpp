#include "types.h"
#include <array>
#include <string>
#include <sstream>

namespace kq
{

physicalObjectArgs::physicalObjectArgs(const sf::Vector2f& position, const sf::Vector2f& velocity,
									const sf::Color& color, float mass, objectType type)
	: position(position), velocity(velocity), color(color), mass(mass), type(type) {}

physicalObject::physicalObject(physicalObjectArgs&& args)
        : m_position(args.position), m_velocity(args.velocity), m_color(args.color),
          m_mass(args.mass), m_type(args.type), m_collisions(0) {}

sf::Vector2f& physicalObject::getPosition() { return m_position; }
sf::Vector2f& physicalObject::getVelocity() { return m_velocity; }
sf::Color& physicalObject::getColor() { return m_color; }
float& physicalObject::getMass() { return m_mass; }
objectType& physicalObject::getObjectType() { return m_type; }

sf::Vector2f physicalObject::getPosition() const { return m_position; }
sf::Vector2f physicalObject::getVelocity() const { return m_velocity; }
sf::Color physicalObject::getColor() const { return m_color; }
float physicalObject::getMass() const { return m_mass; }
float physicalObject::getInvMass() const { return 1 / m_mass; }
objectType physicalObject::getObjectType() const { return m_type; }
uint32_t physicalObject::getCollisions() const { return m_collisions; }

void physicalObject::applyGravity(float deltaTime)
{
	m_velocity += sf::Vector2f{0, m_gravity * m_mass} * deltaTime;
}

void physicalObject::applyAirResistance(float deltaTime)
{
	float relativeAirResistance = 1.0f - (m_airResistance * deltaTime) / m_mass;
	m_velocity *= relativeAirResistance;
}

float physicalObject::dotProduct(const sf::Vector2f& a, const sf::Vector2f& b)
{
    return a.x * b.x + a.y * b.y;
}

float physicalObject::crossProduct(const sf::Vector2f& a, const sf::Vector2f& b)
{
    return a.x * b.y - a.y * b.x;
}

sf::Vector2f physicalObject::normalize(const sf::Vector2f& vector)
{
    float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
    if (length != 0) {
        return sf::Vector2f(vector.x / length, vector.y / length);
    } else {
        return vector;
    }
}

void physicalObject::resolveCollision(physicalObject& obj1, physicalObject& obj2)
{
	++obj1.m_collisions;
	 // Calculate the direction of the collision
    sf::Vector2f collisionDir = obj1.getPosition() - obj2.getPosition();
    collisionDir = normalize(collisionDir);

    // Calculate the relative velocity
    sf::Vector2f relVel = obj1.getVelocity() - obj2.getVelocity();

    // Calculate the velocity along the direction of the collision
    float velAlongCollision = dotProduct(relVel, collisionDir);

    // If the objects are moving away from each other, do nothing
    if(velAlongCollision > 0)
        return;

	// Calculate the impulse
    float impulse = -(1 + restitution) * velAlongCollision / (obj1.getInvMass() + obj2.getInvMass());
    sf::Vector2f impulseVec = impulse * collisionDir;

    // Apply the impulse to the objects
    sf::Vector2f impulseVecScaled1 = sf::Vector2f(impulseVec.x * obj1.getInvMass(), impulseVec.y * obj1.getInvMass());
	obj1.getVelocity() = obj1.getVelocity() + impulseVecScaled1;

	sf::Vector2f impulseVecScaled2 = sf::Vector2f(impulseVec.x * obj2.getInvMass(), impulseVec.y * obj2.getInvMass());
	obj2.getVelocity() = obj2.getVelocity() - impulseVecScaled2;

    
}

float physicalObject::restitution = 0.8f;
bool physicalObject::m_outline = false;
float physicalObject::m_gravity = 9.8f;
float physicalObject::m_airResistance = 0.01f;
float physicalObject::m_timeAcceleration = 1.f;

/* ========== Circle ========== */

Circle::Circle(physicalObjectArgs&& args, float radius)
	: physicalObject(std::move(args)), m_radius(radius) {}

void Circle::move(const sf::Vector2f& offset, float deltaTime)  
{
	m_position += offset * deltaTime;
	// Check if the new position is outside the screen
    if (m_position.x - m_radius < 0) {
        m_position.x = m_radius;
        m_velocity.x *= -1; // Reverse the x velocity to make the circle bounce
    } else if (m_position.x + m_radius > SCREEN_WIDTH_F) {
        m_position.x = SCREEN_WIDTH_F - m_radius;
        m_velocity.x *= -1; // Reverse the x velocity to make the circle bounce
    }

    if (m_position.y - m_radius < 0) {
        m_position.y = m_radius;
        m_velocity.y *= -1; // Reverse the y velocity to make the circle bounce
    } else if (m_position.y + m_radius > SCREEN_LENGTH_F) {
        m_position.y = SCREEN_LENGTH_F - m_radius;
        m_velocity.y *= -1; // Reverse the y velocity to make the circle bounce
    }
}

void Circle::applyForce(const sf::Vector2f& force)  
{
	m_velocity += force / static_cast<float>(m_mass);
}

void Circle::update(float deltaTime)  
{
	// Update position based on velocity.
	deltaTime *= m_timeAcceleration;
	applyGravity(deltaTime);
	applyAirResistance(deltaTime);
	move(m_velocity, deltaTime);
}

void Circle::draw(sf::RenderWindow& window) const  
{
	sf::CircleShape circle(m_radius);
	circle.setOrigin(m_radius, m_radius);
	circle.setFillColor(m_color);
	if(physicalObject::m_outline)
	{
		sf::Color outlineColor = sf::Color{static_cast<sf::Uint8>(255 - m_color.r), static_cast<sf::Uint8>(255 - m_color.g),
		 static_cast<sf::Uint8>(255 - m_color.b), m_color.a};
		circle.setOutlineColor(outlineColor);
		circle.setOutlineThickness(2.0f);
	}
	circle.setPosition(m_position);
	window.draw(circle);
}

objectType Circle::getType() const  
{
	return objectType::Circle;
}

bool Circle::collidesWith(const physicalObject& other) const  
{
	if (const Circle* circle = dynamic_cast<const Circle*>(&other)) 
	{
		return collidesWith(*circle);
	} 
	else if (const Square* square = dynamic_cast<const Square*>(&other)) 
	{
		return collidesWith(*square);
	}
	else if(const Triangle* triangle = dynamic_cast<const Triangle*>(&other))
	{
		return collidesWith(*triangle);
	}
	else if(const Rectangle* rectangle = dynamic_cast<const Rectangle*>(&other))
	{
		return collidesWith(*rectangle);
	}
	else return false;
}

bool Circle::collidesWith(const Circle& other) const 
{
	float distance = sqrt(pow(m_position.x - other.m_position.x, 2) + pow(m_position.y - other.m_position.y, 2));
	return distance < (m_radius + other.m_radius);
}

bool Circle::collidesWith(const Square& other) const 
{
    // Get the four corners of the square
    std::array<sf::Vector2f, 4> corners = other.getVertices();

    // Check each corner to see if it's inside the circle
    for (const auto& corner : corners) 
    {
        if (containsPoint(corner))
        {
            return true;  // The corner is inside the circle, so there's a collision
        }
    }

    // If none of the corners are inside the circle, there's no collision
    return false;
}

bool Circle::collidesWith(const Triangle& other) const
{
	// Iterate over the vertices of the triangle
    for (const auto& vertex : other.getVertices())
    {
        // Check if the circle contains the vertex
        if (containsPoint(vertex))
        {
            return true;  // The vertex is inside the circle, so there's a collision
        }
    }

    // If none of the vertices are inside the circle, there's no collision
    return false;
}

bool Circle::collidesWith(const Rectangle& other) const 
{
    // Get the vertices of the rectangle
    auto vertices = other.getVertices();

    // Check if any vertex of the rectangle is inside this circle
    for (const auto& vertex : vertices)
    {
        if (containsPoint(vertex))
        {
            return true;  // The vertex is inside this circle, so there's a collision
        }
    }

    // If none of the vertices are inside this circle, there's no collision
    return false;
}

bool Circle::containsPoint(sf::Vector2f point) const
{
    // Calculate the distance between the point and the center of the circle
    float distanceX = m_position.x - point.x;
    float distanceY = m_position.y - point.y;

    // If the distance is less than the circle's radius, the point is inside the circle
    return (distanceX * distanceX + distanceY * distanceY) <= (m_radius * m_radius);
}

std::string Circle::toCSVString() const
{
    std::ostringstream ss;
    ss << m_position.x << "," << m_position.y << ","
       << m_velocity.x << "," << m_velocity.y << ","
       << static_cast<int>(m_color.r) << "," << static_cast<int>(m_color.g) << "," << static_cast<int>(m_color.b) << ","
       << m_mass << ","
       << static_cast<int>(m_type) << "," << m_radius;
    return ss.str();
}

/* ========== Square ========== */

Square::Square(physicalObjectArgs&& args, float sideLength)
	: physicalObject(std::move(args)), m_sideLength(sideLength) {}

void Square::move(const sf::Vector2f& offset, float deltaTime) 
{
	m_position += offset * deltaTime;
	// Update position based on velocity.

	// check position to not be out of screen
	float halfSideLength = m_sideLength / 2;

	if (m_position.x - halfSideLength < 0) {
		m_position.x = halfSideLength;
		m_velocity.x *= -1; // Reverse the x velocity to make the square bounce
	} else if (m_position.x + halfSideLength > SCREEN_WIDTH_F) {
		m_position.x = SCREEN_WIDTH_F - halfSideLength;
		m_velocity.x *= -1; // Reverse the x velocity to make the square bounce
	}

	if (m_position.y - halfSideLength < 0) {
		m_position.y = halfSideLength;
		m_velocity.y *= -1; // Reverse the y velocity to make the square bounce
	} else if (m_position.y + halfSideLength > SCREEN_LENGTH_F) {
		m_position.y = SCREEN_LENGTH_F - halfSideLength;
		m_velocity.y *= -1; // Reverse the y velocity to make the square bounce
	}
}

void Square::applyForce(const sf::Vector2f& force) 
{
	m_velocity += force / static_cast<float>(m_mass);
}

void Square::update(float deltaTime) 
{
	// Update position based on velocity.
	deltaTime *= m_timeAcceleration;
	applyGravity(deltaTime);
	applyAirResistance(deltaTime);
	move(m_velocity, deltaTime);
}

void Square::draw(sf::RenderWindow& window) const 
{
	sf::RectangleShape square(sf::Vector2f(m_sideLength, m_sideLength));
	square.setOrigin(m_sideLength / 2, m_sideLength / 2);
	square.setFillColor(m_color);
	if(physicalObject::m_outline)
	{
		sf::Color outlineColor = sf::Color{static_cast<sf::Uint8>(255 - m_color.r), static_cast<sf::Uint8>(255 - m_color.g),
		 static_cast<sf::Uint8>(255 - m_color.b), m_color.a};
		square.setOutlineColor(outlineColor);
		square.setOutlineThickness(2.0f);
	}
	square.setPosition(m_position);
	window.draw(square);
}

objectType Square::getType() const 
{
	return objectType::Square;
}

bool Square::collidesWith(const physicalObject& other) const
{
	if (const Circle* circle = dynamic_cast<const Circle*>(&other)) 
	{
		return collidesWith(*circle);
	} 
	else if (const Square* square = dynamic_cast<const Square*>(&other)) 
	{
		return collidesWith(*square);
	}
	else if(const Triangle* triangle = dynamic_cast<const Triangle*>(&other))
	{
		return collidesWith(*triangle);
	}
	else if(const Rectangle* rectangle = dynamic_cast<const Rectangle*>(&other))
	{
		return collidesWith(*rectangle);
	}
	else return false;
}

bool Square::collidesWith(const Circle& other) const
{
	return other.collidesWith(*this);
}

bool Square::collidesWith(const Square& other) const
{
	// Check if the squares overlap on the x-axis.
	if (other.getPosition().x < m_position.x && m_position.x < other.getPosition().x + other.getSideLength() ||
		other.getPosition().x < m_position.x + m_sideLength && m_position.x + m_sideLength < other.getPosition().x + other.getSideLength()) 
	{
		// Check if the squares overlap on the y-axis.
		if (other.getPosition().y < m_position.y && m_position.y < other.getPosition().y + other.getSideLength() ||
			other.getPosition().y < m_position.y + m_sideLength && m_position.y + m_sideLength < other.getPosition().y + other.getSideLength()) 
		{
			return true;
		}
	}

	return false;
}

bool Square::collidesWith(const Triangle& other) const 
{
    // Iterate over the vertices of the triangle
    for (const auto& vertex : other.getVertices())
    {
        // Check if the square contains the vertex
        if (containsPoint(vertex))
        {
            return true;  // The vertex is inside the square, so there's a collision
        }
    }

    // If none of the vertices are inside the square, there's no collision
    return false;
}

bool Square::collidesWith(const Rectangle& other) const
{
    // Get the vertices of the square
    auto vertices = getVertices();

    // Check if any vertex of the square is inside the rectangle
    for (const auto& vertex : vertices)
    {
        if (other.containsPoint(vertex))
        {
            return true;  // The vertex is inside the rectangle, so there's a collision
        }
    }

    // If none of the vertices of the square are inside the rectangle, there's no collision
    return false;
}

float Square::getSideLength() const { return m_sideLength; }

std::array<sf::Vector2f, 4> Square::getVertices() const 
{
    float halfSideLength = getSideLength() / 2;
    return {
        sf::Vector2f(getPosition().x - halfSideLength, getPosition().y - halfSideLength), // top-left corner
        sf::Vector2f(getPosition().x + halfSideLength, getPosition().y - halfSideLength), // top-right corner
        sf::Vector2f(getPosition().x - halfSideLength, getPosition().y + halfSideLength), // bottom-left corner
        sf::Vector2f(getPosition().x + halfSideLength, getPosition().y + halfSideLength)  // bottom-right corner
    };
}

bool Square::containsPoint(sf::Vector2f point) const
{
	// Calculate the half size of the square
    float halfSize = m_sideLength / 2.0f;

    // Check if the point is within the boundaries of the square
    if (point.x >= (m_position.x - halfSize) && point.x <= (m_position.x + halfSize) &&
        point.y >= (m_position.y - halfSize) && point.y <= (m_position.y + halfSize))
    {
        return true;  // The point is inside the square
    }

    // The point is outside the square
    return false;
}

std::string Square::toCSVString() const
{
    std::ostringstream ss;
    ss << m_position.x << "," << m_position.y << ","
       << m_velocity.x << "," << m_velocity.y << ","
       << static_cast<int>(m_color.r) << "," << static_cast<int>(m_color.g) << "," << static_cast<int>(m_color.b) << ","
       << m_mass << ","
       << static_cast<int>(m_type) << "," << m_sideLength;
    return ss.str();
}

/* ========== Triangle ========== */

Triangle::Triangle(physicalObjectArgs&& args, float sideLength)
	: physicalObject(std::move(args)), m_sideLength(sideLength) {}

void Triangle::move(const sf::Vector2f& offset, float deltaTime)
{
    m_position += offset * deltaTime;
    float height = m_sideLength * sqrt(3) / 2.0f;
    float halfSideLength = m_sideLength / 2.0f;
    float halfHeight = height / 2.0f;

    // Check if the triangle is out of the map and handle collision
    if (m_position.x - halfSideLength < 0)
    {
        m_position.x = halfSideLength;
        m_velocity.x = -m_velocity.x;
    }
    else if (m_position.y - halfHeight < 0)
    {
        m_position.y = halfHeight;
        m_velocity.y = -m_velocity.y;
    }
    else if (m_position.x + halfSideLength > SCREEN_WIDTH)
    {
        m_position.x = SCREEN_WIDTH - halfSideLength;
        m_velocity.x = -m_velocity.x;
    }
    else if (m_position.y + halfHeight > SCREEN_LENGTH)
    {
        m_position.y = SCREEN_LENGTH - halfHeight;
        m_velocity.y = -m_velocity.y;
    }
}

void Triangle::applyForce(const sf::Vector2f& force) 
{
	m_velocity += force / static_cast<float>(m_mass);
}

void Triangle::update(float deltaTime)
{
	deltaTime *= m_timeAcceleration;
	move(m_velocity, deltaTime);
	applyGravity(deltaTime);
	applyAirResistance(deltaTime);
}

void Triangle::draw(sf::RenderWindow& window) const 
{
	sf::ConvexShape triangle;
    triangle.setPointCount(3); // Set the number of points to 3 for a triangle

    // Set the points of the triangle
	float halfBase = m_sideLength / 2.0f;
    triangle.setPoint(0, sf::Vector2f(0, 0));
    triangle.setPoint(1, sf::Vector2f(m_sideLength, 0));
    triangle.setPoint(2, sf::Vector2f(halfBase, halfBase * sqrt(3)));

    triangle.setOrigin(halfBase, halfBase * sqrt(3) / 3);

    triangle.setFillColor(m_color); 
	if(physicalObject::m_outline)
	{
		sf::Color outlineColor = sf::Color{static_cast<sf::Uint8>(255 - m_color.r), static_cast<sf::Uint8>(255 - m_color.g),
		 static_cast<sf::Uint8>(255 - m_color.b), m_color.a};
		triangle.setOutlineColor(outlineColor);
		triangle.setOutlineThickness(2.0f);
	}
    triangle.setPosition(m_position); 

    window.draw(triangle); 
}

objectType Triangle::getType() const 
{
	return objectType::Triangle;
}

bool Triangle::collidesWith(const physicalObject& other) const
{
	if (const Circle* circle = dynamic_cast<const Circle*>(&other)) 
	{
		return collidesWith(*circle);
	} 
	else if (const Square* square = dynamic_cast<const Square*>(&other)) 
	{
		return collidesWith(*square);
	}
	else if(const Triangle* triangle = dynamic_cast<const Triangle*>(&other))
	{
		return collidesWith(*triangle);
	}
	else if(const Rectangle* rectangle = dynamic_cast<const Rectangle*>(&other))
	{
		return collidesWith(*rectangle);
	}
	else return false;
}

bool Triangle::collidesWith(const Circle& other) const
{
	return other.collidesWith(*this);
}

bool Triangle::collidesWith(const Square& other) const
{
	return other.collidesWith(*this);
}

bool Triangle::collidesWith(const Triangle& other) const 
{
    // Get the vertices of the other triangle
    auto otherVertices = other.getVertices();

    // Check if any vertex of the other triangle is inside this triangle
    for (const auto& vertex : otherVertices)
    {
        if (containsPoint(vertex))
        {
            return true;  // The vertex is inside this triangle, so there's a collision
        }
    }

    // If none of the vertices are inside this triangle, there's no collision
    return false;
}

bool Triangle::collidesWith(const Rectangle& other) const
{
    // Get the vertices of the triangle
    auto vertices = getVertices();

    // Check if any vertex of the triangle is inside the rectangle
    for (const auto& vertex : vertices)
    {
        if (other.containsPoint(vertex))
        {
            return true;  // The vertex is inside the rectangle, so there's a collision
        }
    }

    // If none of the vertices of the triangle are inside the rectangle, there's no collision
    return false;
}

float Triangle::getSideLength() const { return m_sideLength; }

std::array<sf::Vector2f, 3> Triangle::getVertices() const
{
    std::array<sf::Vector2f, 3> vertices;
    float halfBase = m_sideLength / 2.0f;
    float height = halfBase * sqrt(3);

    vertices[0] = sf::Vector2f(m_position.x - halfBase, m_position.y - height / 3); // The first vertex is at the left corner of the base
    vertices[1] = sf::Vector2f(m_position.x + halfBase, m_position.y - height / 3); // The second vertex is at the right corner of the base
    vertices[2] = sf::Vector2f(m_position.x, m_position.y + 2 * height / 3); // The third vertex is at the top of the triangle

    return vertices;
}

bool Triangle::containsPoint(sf::Vector2f point) const
{
	auto vertices = getVertices();
    float area = 0.5f * (-vertices[1].y * vertices[2].x + vertices[0].y * (-vertices[1].x + vertices[2].x) + vertices[0].x * (vertices[1].y - vertices[2].y) + vertices[1].x * vertices[2].y);
    float s = 1.0f / (2 * area) * (vertices[0].y * vertices[2].x - vertices[0].x * vertices[2].y + (vertices[2].y - vertices[0].y) * point.x + (vertices[0].x - vertices[2].x) * point.y);
    float t = 1.0f / (2 * area) * (vertices[0].x * vertices[1].y - vertices[0].y * vertices[1].x + (vertices[0].y - vertices[1].y) * point.x + (vertices[1].x - vertices[0].x) * point.y);

    return s > 0 && t > 0 && (1 - s - t) > 0;
}

std::string Triangle::toCSVString() const
{
    std::ostringstream ss;
    ss << m_position.x << "," << m_position.y << ","
       << m_velocity.x << "," << m_velocity.y << ","
       << static_cast<int>(m_color.r) << "," << static_cast<int>(m_color.g) << "," << static_cast<int>(m_color.b) << ","
       << m_mass << ","
       << static_cast<int>(m_type) << "," << m_sideLength;
    return ss.str();
}

/* ========== Rectangle ========== */

Rectangle::Rectangle(physicalObjectArgs&& args, float width, float height)
	: physicalObject(std::move(args)), m_width(width), m_height(height) {}

void Rectangle::move(const sf::Vector2f& offset, float deltaTime)
{
    m_position += offset * deltaTime;
    float halfWidth = m_width / 2.0f;
    float halfHeight = m_height / 2.0f;

    // Check if the rectangle is out of the map and handle collision
    if (m_position.x - halfWidth < 0)
    {
        m_position.x = halfWidth;
        m_velocity.x = -m_velocity.x;
    }
    else if (m_position.y - halfHeight < 0)
    {
        m_position.y = halfHeight;
        m_velocity.y = -m_velocity.y;
    }
    else if (m_position.x + halfWidth > SCREEN_WIDTH_F)
    {
        m_position.x = SCREEN_WIDTH_F - halfWidth;
        m_velocity.x = -m_velocity.x;
    }
    else if (m_position.y + halfHeight > SCREEN_LENGTH_F)
    {
        m_position.y = SCREEN_LENGTH_F - halfHeight;
        m_velocity.y = -m_velocity.y;
    }
}

void Rectangle::applyForce(const sf::Vector2f& force)
{
	m_velocity += force / static_cast<float>(m_mass);
}

void Rectangle::update(float deltaTime)
{
	deltaTime *= m_timeAcceleration;
	move(m_velocity, deltaTime);
	applyGravity(deltaTime);
	applyAirResistance(deltaTime);
}

void Rectangle::draw(sf::RenderWindow& window) const
{
	sf::RectangleShape rectangle(sf::Vector2f(m_width, m_height));
	rectangle.setOrigin(m_width / 2.0f, m_height / 2.0f);
    rectangle.setPosition(m_position);
    rectangle.setFillColor(m_color);
	if(physicalObject::m_outline)
	{
		sf::Color outlineColor = sf::Color{static_cast<sf::Uint8>(255 - m_color.r), static_cast<sf::Uint8>(255 - m_color.g),
		 static_cast<sf::Uint8>(255 - m_color.b), m_color.a};
		rectangle.setOutlineColor(outlineColor);
		rectangle.setOutlineThickness(2.0f);
	}
    window.draw(rectangle);
}

objectType Rectangle::getType() const { return objectType::Rectangle; }

bool Rectangle::collidesWith(const physicalObject& other) const
{
	if (const Circle* circle = dynamic_cast<const Circle*>(&other)) 
	{
		return collidesWith(*circle);
	} 
	else if (const Square* square = dynamic_cast<const Square*>(&other)) 
	{
		return collidesWith(*square);
	}
	else if(const Triangle* triangle = dynamic_cast<const Triangle*>(&other))
	{
		return collidesWith(*triangle);
	}
	else if(const Rectangle* rectangle = dynamic_cast<const Rectangle*>(&other))
	{
		return collidesWith(*rectangle);
	}
	else return false;
}

bool Rectangle::collidesWith(const Circle& other) const
{
	return other.collidesWith(*this);
}

bool Rectangle::collidesWith(const Square& other) const
{
	return other.collidesWith(*this);
}

bool Rectangle::collidesWith(const Triangle& other) const
{
	return 	other.collidesWith(*this);
}

bool Rectangle::collidesWith(const Rectangle& other) const
{
    // Get the vertices of the other rectangle
    auto otherVertices = other.getVertices();

    // Check if any vertex of the other rectangle is inside this rectangle
    for (const auto& vertex : otherVertices)
    {
        if (containsPoint(vertex))
        {
            return true;  // The vertex is inside this rectangle, so there's a collision
        }
    }

    
    return false;
}

float Rectangle::getWidth() const { return m_width; }

float Rectangle::getHeight() const { return m_height; }

bool Rectangle::containsPoint(sf::Vector2f point) const
{
    float halfWidth = m_width / 2.0f;
    float halfHeight = m_height / 2.0f;

    return point.x >= m_position.x - halfWidth && point.x <= m_position.x + halfWidth &&
           point.y >= m_position.y - halfHeight && point.y <= m_position.y + halfHeight;
}

std::array<sf::Vector2f, 4> Rectangle::getVertices() const
{
    std::array<sf::Vector2f, 4> vertices;

    float halfWidth = m_width / 2.0f;
    float halfHeight = m_height / 2.0f;

    vertices[0] = sf::Vector2f(m_position.x - halfWidth, m_position.y - halfHeight); // Top-left corner
    vertices[1] = sf::Vector2f(m_position.x + halfWidth, m_position.y - halfHeight); // Top-right corner
    vertices[2] = sf::Vector2f(m_position.x + halfWidth, m_position.y + halfHeight); // Bottom-right corner
    vertices[3] = sf::Vector2f(m_position.x - halfWidth, m_position.y + halfHeight); // Bottom-left corner

    return vertices;
}

std::string Rectangle::toCSVString() const
{
    std::ostringstream ss;
    ss << m_position.x << "," << m_position.y << ","
       << m_velocity.x << "," << m_velocity.y << ","
       << static_cast<int>(m_color.r) << "," << static_cast<int>(m_color.g) << "," << static_cast<int>(m_color.b) << ","
       << m_mass << ","
       << static_cast<int>(m_type) << "," << m_width << "," << m_height;
    return ss.str();
}

	

} // namespace kq