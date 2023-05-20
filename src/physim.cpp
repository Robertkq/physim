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
    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                m_window.close();
            }
            else if(event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(m_window);
                    std::cout << "Left-click at (" << mousePosition.x << ", " << mousePosition.y << ")" << std::endl;
                    m_entities.push_back(std::make_unique<Shape<sf::CircleShape, DynamicBody>>(100.f));
                }
            }
        }

        ImGui::SFML::Update(m_window, sf::seconds(1.f / 60.f));

        m_window.clear();

        calculateObjects();
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

void physim::calculateObjects()
{
    for(auto& entity : m_entities)
    {
        entity->applyPhysics();
    }
}

void physim::mainMenu()
{
    
    ImGui::ShowDemoWindow();
    ImGui::SetNextWindowSize({(float)m_width, (float)m_height});
    ImGui::Begin("Physim", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    
    ImGui::End();
}

} // namespace kq

