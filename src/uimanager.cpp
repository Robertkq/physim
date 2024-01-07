#include "uimanager.h"
#include "types.h"

namespace kq {

UIManager::UIManager()
    : m_toggle(false), m_type(objectType::circle), m_radius(100.f), m_rotation(0.f), m_size({100.f, 100.f}), m_play(false)
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

void UIManager::show(std::vector<std::shared_ptr<Entity>>& entities)
{
    editPanel();
    listPanel(entities);
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

float UIManager::getRotation() { return m_rotation; }

sf::Vector2f UIManager::getSize() { return m_size; }

sf::Vector2f UIManager::getVelocity() {return m_velocity; }


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

    ImGui::ListBox("Type of object", reinterpret_cast<int*>(&m_type), m_types, sizeof(m_types) / sizeof(m_types[0]), 4);
    ImGui::SliderAngle("Rotation of object", &m_rotation);
    ImGui::SliderFloat("X velocity of object", &(m_velocity.x), -200.f, 200.f, "%.2f");
    ImGui::SliderFloat("Y velocity of object", &(m_velocity.y), -200.f, 200.f, "%.2f");
    if(m_type == objectType::circle)
    {
        ImGui::SliderFloat("Radius of object ", &m_radius, 50.f, 150.f, "%.2f");
    }
    else if(m_type == objectType::square)
    {
        ImGui::SliderFloat("Width & Length of object", &(m_size.x), 50.f, 150.f, "%.2f");
        m_size.y = m_size.x;
    }
    else if(m_type == objectType::rectangle)
    {
        ImGui::SliderFloat("Width of object", &(m_size.x), 50.f, 150.f, "%.2f");
        ImGui::SliderFloat("Length of object", &(m_size.y), 50.f, 150.f, "%.2f");
    }
    else if(m_type == objectType::triangle)
    {

    }
    else if(m_type == objectType::convex)
    {

    }
    ImGui::End();
    
}

void UIManager::listPanel(std::vector<std::shared_ptr<Entity>>& entities)
{
    ImGui::Begin("List Panel");

    static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
    ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
    if (ImGui::BeginTable("list_cars", 6, flags, outer_size))
    {
        ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
        ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Color", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Tag", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_None);
        ImGui::TableHeadersRow();

        for (uint32_t i = 0; i < entities.size(); ++i)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text(itos(i).data());
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("hi");
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("hii");
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("hi");
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("bye");
        }
        
        ImGui::EndTable();
    }

    ImGui::End();
}

void UIManager::objectPanel(std::shared_ptr<Entity>)
{

}

} // namespace kq