#include "fileManager.h"
#include "types.h"
#include <sstream>
#include "physim.h"

namespace kq
{

bool fileManager::loadcsv(const std::string& filename, std::vector<physicalObject*>& objects) 
{
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        std::getline(file, line); // Skip the first line
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string field;

            sf::Vector2f position;
            sf::Vector2f velocity;
            std::array<float, 4> color;
            float mass;
            objectType type;

            std::getline(ss, field, ',');
            position.x = std::stof(field);

            std::getline(ss, field, ',');
            position.y = std::stof(field);

            std::getline(ss, field, ',');
            velocity.x = std::stof(field);

            std::getline(ss, field, ',');
            velocity.y = std::stof(field);

            std::getline(ss, field, ',');
            color[0] = std::stoi(field) / 255.f;

            std::getline(ss, field, ',');
            color[1] = std::stoi(field) / 255.f;

            std::getline(ss, field, ',');
            color[2] = std::stoi(field) / 255.f;
            color[3] = 255 / 255.f;
            std::getline(ss, field, ',');
            mass = std::stof(field);

            std::getline(ss, field, ',');
            type = static_cast<objectType>(std::stoi(field));

            float radius;
            sf::Vector2f size;
            switch(type)
            {
                case objectType::Circle:
                {
                    
                    std::getline(ss, field, ',');
                    radius = std::stof(field);
                    break;
                }
                case objectType::Square:
                {
                    std::getline(ss, field, ',');
                    radius = std::stof(field);
                    break;
                }
                case objectType::Rectangle:
                {
                    
                    std::getline(ss, field, ',');
                    size.x = std::stof(field);
                    std::getline(ss, field, ',');
                    size.y = std::stof(field);
                    break;
                }
                case objectType::Triangle:
                {
                    std::getline(ss, field, ',');
                    radius = std::stof(field);
                    break;
                }
            }
            parent->createObject(type, {}, radius, size, velocity, color, mass);
            parent->getEntities().back()->getPosition() = position;

            // Now you can use these variables to create a physicalObject instance
            // ...
        }
        file.close();
    } else {
        std::cout << "Failed to open file: " << filename << std::endl;
        return false;
    }
    return true;
}

bool fileManager::savecsv(const std::string& filename, const std::vector<physicalObject*>& objects) 
{
    std::ofstream file(filename);
    file << "PositionX,PositionY,VelocityX,VelocityY,ColorR,ColorG,ColorB,Mass,Type,Extra1,Extra2\n";
    if (file.is_open()) {
        for (const physicalObject* obj : objects) {
            file <<  obj->toCSVString() << std::endl;
        }
        file.close();
    } else {
        std::cout << "Failed to create file: " << filename << std::endl;
        return false;
    }
    return true;
}

} //namespace kq