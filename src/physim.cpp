#include "physim.h"

namespace kq
{

physim::physim()
    : m_width(SCREEN_WIDTH), m_height(SCREEN_LENGTH), m_window(sf::VideoMode(m_width, m_height), "physim", sf::Style::None),
    m_entities()
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
                        createObject(type, rotation, radius, size, velocity);
                    }
                }
            }
        }


        ImGui::SFML::Update(m_window, sf::seconds(1.f / 60.f));

        float deltaTime = clock.restart().asSeconds();
        m_window.clear();

        updateObjects(deltaTime);
        drawObjects();
        mainMenu();

        ImGui::SFML::Render(m_window);

        m_window.display();
    }
}

void physim::drawObjects()
{
    for(auto& entity : m_entities)
    {
        entity->draw(m_window);
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
}

void physim::mainMenu()
{
    
    if(m_UIManager.isActive())
    {
        m_UIManager.show();
        ImGui::ShowDemoWindow();
    }
}

void physim::createObject(objectType type, float rotation, float radius, sf::Vector2f size, sf::Vector2f velocity)
{
    
    sf::Vector2i mousePos = sf::Mouse::getPosition(m_window); 
    sf::Vector2f mousePosF {float(mousePos.x), float(mousePos.y)};

    std::cout << "Gotta create a " << int(type) << " at " << mousePosF.x << " and " << mousePosF.y << "\n";

    if(type == objectType::circle)
    {
        m_entities.push_back(std::make_unique<Shape<sf::CircleShape, DynamicBody>>(radius));
    }
    if(type == objectType::square)
    {
        m_entities.push_back(std::make_unique<Shape<sf::RectangleShape, DynamicBody>>(size));
    }
    
    m_entities.back()->setPosition(mousePosF);
    m_entities.back()->setVelocity(velocity);
}

} // namespace kq

