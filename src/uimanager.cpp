#include "uimanager.h"
#include "types.h"
#include "physim.h"

namespace kq {

UIManager::UIManager(physim* parent)
    : m_parent(parent), m_toggle(false), m_type(objectType::Circle), m_radius(100.f), m_rotation(0.f), m_size({100.f, 100.f}),
    m_velocity({50.f, 50.f}), m_play(false), m_color(), m_selected(0), m_showSelected(false), m_mass(1), m_exportMenu(false),
    m_importMenu(false)
{

}

void UIManager::toggle()
{
    m_toggle = !m_toggle;
}

bool UIManager::isActive() const
{
    return m_toggle;
}

void UIManager::show()
{
    editPanel();
    listPanel();
    objectPanel();
    importPanel();
    exportPanel();
}

void UIManager::play()
{
    m_play = true;
}

void UIManager::pause()
{
    m_play = false;
}

bool UIManager::isPlaying()
{
    return m_play;
}

objectType UIManager::getType() { return m_type; }

float UIManager::getRadius() { return m_radius; }

float UIManager::getRotation() { return m_rotation * (pi / 180.0f); }

sf::Vector2f UIManager::getSize() { return m_size; }

sf::Vector2f UIManager::getVelocity() {return m_velocity; }

bool UIManager::isSelected() { return m_showSelected; }

uint32_t UIManager::getSelected() { return m_selected; }

float UIManager::getMass() { return m_mass;}


void UIManager::editPanel()
{
    ImGui::Begin("Edit Panel");
    if(isPlaying())
    {
        if(ImGui::Button("Pause"))
        {
            pause();
        }
    }
    else
    {
        if(ImGui::Button("Play"))
        {
            play();
        }
    }
    ImGui::SameLine();
    if(ImGui::Button("Impulse"))
    {
        m_parent->Impulse();
    }
    ImGui::SameLine();
    if(ImGui::Button("Export"))
    {
        m_exportMenu = !m_exportMenu;
    }
    ImGui::SameLine();
    if(ImGui::Button("Import"))
    {
        m_importMenu = !m_importMenu;
    }

    ImGui::ListBox("Type of object", reinterpret_cast<int*>(&m_type), m_types, sizeof(m_types) / sizeof(m_types[0]), 4);
    ImGui::SliderFloat("Mass of object", &m_mass, 1.f, 100.f, "%.2f");
    ImGui::SliderFloat("X velocity of object", &(m_velocity.x), -750.f, 750.f, "%.2f");
    ImGui::SliderFloat("Y velocity of object", &(m_velocity.y), -750.f, 750.f, "%.2f");
    if(m_type == objectType::Circle)
    {
        ImGui::SliderFloat("Radius of object ", &m_radius, 50.f, 150.f, "%.2f");
    }
    else if(m_type == objectType::Square)
    {
        ImGui::SliderFloat("Length of side", &(m_radius), 50.f, 150.f, "%.2f");
    }
    else if(m_type == objectType::Rectangle)
    {
        ImGui::SliderFloat("Width of object", &(m_size.x), 50.f, 150.f, "%.2f");
        ImGui::SliderFloat("Length of object", &(m_size.y), 50.f, 150.f, "%.2f");
    }
    else if(m_type == objectType::Triangle)
    {
        ImGui::SliderFloat("Length of side", &(m_radius), 50.f, 150.f, "%.2f");
    }
    else if(m_type == objectType::Convex)
    {

    }

    // Initialize color as an array of 4 floats (RGBA), with initial white color
    static float colorInternal[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // Create a color picker
    ImGui::ColorPicker4("ColorPicker", colorInternal, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHex);
    m_color[0] = colorInternal[0];
    m_color[1] = colorInternal[1];
    m_color[2] = colorInternal[2];
    m_color[3] = colorInternal[3];

    // Display the hexadecimal color value
    char hexColor[9];
    sprintf(hexColor, "%02X%02X%02X%02X", 
            static_cast<int>(m_color[0] * 255), 
            static_cast<int>(m_color[1] * 255), 
            static_cast<int>(m_color[2] * 255), 
            static_cast<int>(m_color[3] * 255));
    ImGui::Text("Hex: %s", hexColor);

    ImGui::SliderFloat("Gravity force", &physicalObject::m_gravity, 0.f, 100.f, "%.2f");
    ImGui::SliderFloat("Air Resistance", &physicalObject::m_airResistance, 0.f, 0.5f, "%.2f");
    ImGui::SliderFloat("Time acceleration", &physicalObject::m_timeAcceleration, 0.1f, 10.f, "%.2f");

    ImGui::End();
    
}

void UIManager::listPanel()
{
    const std::vector<physicalObject*>& entities = m_parent->getEntities();
    ImGui::Begin("List Panel");

    if(ImGui::Button("Clear list"))
    {
        m_parent->clearEntities();
        m_showSelected = false;
    }
    static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
    ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
    if (ImGui::BeginTable("list_objects", 5, flags, outer_size))
    {
        ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
        ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Color", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Mass", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("View", ImGuiTableColumnFlags_None);
        ImGui::TableHeadersRow();

        for (uint32_t i = 0; i < entities.size(); ++i)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text(itos(i).data());
            ImGui::TableSetColumnIndex(1);
            ImGui::Text(getShapeName(entities[i]->getType()).data());
            ImGui::TableSetColumnIndex(2);
            getColorBox(entities[i]->getColor());
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%.2f", entities[i]->getMass());
            ImGui::TableSetColumnIndex(4);
            std::string buttonLabel = "View##" + std::to_string(i);
            if(ImGui::Button(buttonLabel.data()))
            {
                m_selected = i;
                m_showSelected = true;
            }
        }
        
        ImGui::EndTable();
    }

    ImGui::End();
}

void UIManager::objectPanel()
{
    if(!m_showSelected)
        return;

    ImGui::Begin("Object Panel");
    ImGui::Text("Type: %s", getShapeName(m_parent->getEntities()[m_selected]->getType()).data());
    ImGui::Text("Color: "); ImGui::SameLine(); getColorBox(m_parent->getEntities()[m_selected]->getColor());
    ImGui::Text("Mass: %.2f", m_parent->getEntities()[m_selected]->getMass());

    // Assuming position and velocity are sf::Vector2f
    sf::Vector2f position = m_parent->getEntities()[m_selected]->getPosition();
    sf::Vector2f velocity = m_parent->getEntities()[m_selected]->getVelocity();

    ImGui::Text("Position: (%f, %f)", position.x, position.y);
    ImGui::Text("Velocity: (%f, %f)", velocity.x, velocity.y);

    // Placeholder for collisions
    ImGui::Text("Collisions: %d", m_parent->getEntities()[m_selected]->getCollisions());

    ImGui::End();
}


std::string UIManager::getShapeName(objectType type)
{
    switch(type)
    {
        case objectType::Circle:
            return "Circle";
        case objectType::Square:
            return "Square";
        case objectType::Rectangle:
            return "Rectangle";
        case objectType::Triangle:
            return "Triangle";
        case objectType::Convex:
            return "Convex";
        default:
            return "Unknown";
    }
}

void UIManager::getColorBox(sf::Color color)
{
    uint8_t colors[4] = { color.r, color.g, color.b, color.a };
    ImVec4 imguiColor = ImVec4(colors[0] / 255.f, colors[1] / 255.f, colors[2] / 255.f, colors[3] / 255.f);
    
    ImGui::PushStyleColor(ImGuiCol_Button, imguiColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, imguiColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, imguiColor);
    ImGui::Button("!", ImVec2(20, 20));
    ImGui::PopStyleColor(3);
}

std::array<float, 4> UIManager::getColor() const
{
    return m_color;
}

void UIManager::importPanel()
{
    if(!m_importMenu)
        return;
    ImGui::Begin("Import Menu");
    static char filename[128] = "";
    ImGui::InputText("Filename", filename, IM_ARRAYSIZE(filename));
    
    static bool error = false;

    if(ImGui::Button("Import"))
    {
        error = false;
        m_parent->clearEntities();
        auto vec = m_parent->getEntities();
        bool result = m_parent->getFileManager().loadcsv(filename, vec);
        if(result == true)
        {
            m_importMenu = false;
        }
        else
        {
            error = true;
        }
    }
    if(error)
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Error: File not found");
    }
    ImGui::End();
}

void UIManager::exportPanel()
{
    if(!m_exportMenu)
        return;
    ImGui::Begin("Export Menu");  

    static char filename[128] = "";
    ImGui::InputText("Filename", filename, IM_ARRAYSIZE(filename));
    
    static bool error = false;

    if(ImGui::Button("Export"))
    {
        error = false;
        const auto vec = m_parent->getEntities();
        bool result = m_parent->getFileManager().savecsv(filename, vec);
        if(result == true)
        {
            m_exportMenu = false;
        }
        else
        {
            error = true;
        }
    }

    if(error)
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Error: File not found");
    }

    ImGui::End();
}

} // namespace kq