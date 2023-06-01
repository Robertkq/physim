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
            }
            else if(event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(m_window);
                    std::cout << "Left-click at (" << mousePosition.x << ", " << mousePosition.y << ")" << std::endl;
                    static int selector = 0;
                    if(selector % 2 == 0)
                        m_entities.push_back(std::make_unique<Shape<sf::RectangleShape, DynamicBody>>(sf::Vector2f{100, 100}));
                    else
                        m_entities.push_back(std::make_unique<Shape<sf::CircleShape, DynamicBody>>(100.f));
                    ++selector;
                    m_entities.back()->setPosition({mousePosition.x, mousePosition.y});
                    m_entities.back()->applyForce({1000.f, 100.f});
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
    for(auto& entity : m_entities)
    {
        entity->update(deltaTime);
    }
}

void physim::mainMenu()
{
    
    ImGui::ShowDemoWindow();
    ImGui::SetNextWindowSize({(float)m_width, (float)m_height});
    bool collapsed = ImGui::Begin("Physim", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::End();
}

} // namespace kq

