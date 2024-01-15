#include "physim.h"

namespace kq
{

physim::physim()
    : m_width(SCREEN_WIDTH), m_height(SCREEN_LENGTH), m_window(sf::VideoMode(m_width, m_height), "physim", sf::Style::None),
    m_UIManager(this), m_entities(), m_fileManager(this)
{
    m_window.setFramerateLimit(60);
    (void)ImGui::SFML::Init(m_window);
}

physim::~physim()
{
    ImGui::SFML::Shutdown();
}

void physim::run()
{
    sf::Clock clock;
    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                m_window.close();
                break;
            }
            else if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape)
                {
                    m_UIManager.toggle();
                }
                else if(event.key.code == sf::Keyboard::Space)
                {
                    if(m_UIManager.isPlaying())
                        m_UIManager.pause();
                    else
                        m_UIManager.play();
                }
            }
            else if(event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    if(!m_UIManager.isActive())
                    {
                        auto type = m_UIManager.getType();
                        auto rotation = m_UIManager.getRotation();
                        auto radius = m_UIManager.getRadius();
                        auto size = m_UIManager.getSize();
                        auto velocity = m_UIManager.getVelocity();
                        auto color = m_UIManager.getColor();
                        auto mass = m_UIManager.getMass();
                        createObject(type, rotation, radius, size, velocity, color, mass);
                    }
                }
            }
        }


        ImGui::SFML::Update(m_window, sf::seconds(1.f / 60.f));

        float deltaTime = clock.restart().asSeconds();
        m_window.clear(sf::Color(50, 50, 50));

        updateObjects(deltaTime);
        drawObjects();
        mainMenu();

        ImGui::SFML::Render(m_window);

        m_window.display();
    }
}

void physim::drawObjects()
{
    uint32_t i = 0;
    for(auto& entity : m_entities)
    {
        physicalObject::m_outline = m_UIManager.isSelected() && i == m_UIManager.getSelected();
        entity->draw(m_window);
        ++i;
    }
}

void physim::updateObjects(float deltaTime)
{
    if(!m_UIManager.isPlaying())
        return;
    for(auto& entity : m_entities)
    {
        entity->update(deltaTime);
    }

    for(auto& entity1 : m_entities)
    {
        for(auto& entity2 : m_entities)
        {
            if(entity1 != entity2 && entity1->collidesWith(*entity2))
            {
                physicalObject::resolveCollision(*entity1, *entity2);
            }
        }
    }
}

void physim::mainMenu()
{
    
    if(m_UIManager.isActive())
    {
        m_UIManager.show();
        ImGui::ShowDemoWindow();
    }
}

const std::vector<physicalObject*>& physim::getEntities() const
{
    return m_entities;
}

std::vector<physicalObject*>& physim::getEntities()
{
    return m_entities;
}

fileManager& physim::getFileManager()
{
    return m_fileManager;
}

void physim::clearEntities()
{
    for(auto& entity : m_entities)
    {
        delete entity;
    }
    m_entities.clear();
}

void physim::Impulse()
{
    for(auto& entity : m_entities)
    {
        sf::Vector2f random = {static_cast<float>(rand() % 350 + 100) * entity->getMass() / 2,
                                static_cast<float>(rand() % 350 + 100) * entity->getMass() / 2};
        entity->applyForce(random);
    }
}

void physim::createObject(objectType type, float orientation, float radius, sf::Vector2f size,
                 sf::Vector2f velocity, std::array<float, 4> colors, float mass)
{
    sf::Color color(colors[0] * 255, colors[1]  * 255, colors[2]  * 255, colors[3] * 255);
    sf::Vector2 mousePosF = static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window));

    if(type == objectType::Circle)
    {
        m_entities.push_back(new Circle({mousePosF, velocity, color, mass, type}, radius));
    }
    if(type == objectType::Square)
    {
        m_entities.push_back(new Square({mousePosF, velocity, color, mass, type}, radius));
    }
    if(type == objectType::Rectangle)
    {
        m_entities.push_back(new Rectangle({mousePosF, velocity, color, mass, type}, size.x, size.y));
    }
    if(type == objectType::Triangle)
    {
        m_entities.push_back(new Triangle({mousePosF, velocity, color, mass, type}, radius));
    }
}

} // namespace kq

