#include "game.h"
#include "imGuiLayer.h"

#include <imgui.h>
#include <imgui-SFML.h>

ImGuiLayer::ImGuiLayer(Game &game) : m_Game(game) {
    ImGui::SFML::Init(m_Game.m_Window);
}

ImGuiLayer::~ImGuiLayer() {
    ImGui::SFML::Shutdown();
}

void ImGuiLayer::handleEvent(const sf::Event &event) const {
    ImGui::SFML::ProcessEvent(event);
}

void ImGuiLayer::update(const sf::Time &frameTime) const {
    ImGui::SFML::Update(m_Game.m_Window, frameTime);
}

void ImGuiLayer::render(const RenderStats &renderStats) const {
    ImGui::Begin("Debug");
    if(ImGui::TreeNode("Render stats")) {
        ImGui::Text("FPS: %i", renderStats.fps);
        ImGui::Text("Frame time: %.10f ms", renderStats.frameTimeMs);
        ImGui::Text("Balls render time: %.10f ms", renderStats.ballsRenderTime.asMicroseconds() * 0.001f);
        ImGui::Text("Table render time: %.10f ms", renderStats.tableRenderTime.asMicroseconds() * 0.001f);
        ImGui::Text("Cue render time: %.10f ms", renderStats.cueRenderTime.asMicroseconds() * 0.001f);
        ImGui::Text("Debug render time: %.10f ms", renderStats.debugRenderTime.asMicroseconds() * 0.001f);
        ImGui::TreePop();
    }

    if(ImGui::TreeNode("Options")) {
        if(ImGui::TreeNode("Lighting")) {
            ImGui::SliderFloat3("Light position", reinterpret_cast<float*>(&m_Game.m_LightProps.lightPosition), -1000.0f, 1000.0f);
            ImGui::SliderFloat3("Light color", reinterpret_cast<float*>(&m_Game.m_LightProps.lightColor), 0.0f, 1.0f);
            ImGui::SliderFloat("Ambient intensity", &m_Game.m_LightProps.ambientIntensity, 0.0f, 1.0f);
            ImGui::SliderFloat("Diffuse intensity", &m_Game.m_LightProps.diffuseIntensity, 0.0f, 1.0f);
            ImGui::SliderFloat("Specular intensity", &m_Game.m_LightProps.specularIntensity, 0.0f, 1.0f);
            ImGui::SliderFloat("Shininess", &m_Game.m_LightProps.shininess, 0.0f, 100.0f);
            ImGui::TreePop();
        }

        if(ImGui::Checkbox("VSync", &m_Game.m_Options.vsync))
            m_Game.m_Window.setVerticalSyncEnabled(m_Game.m_Options.vsync);

        ImGui::Checkbox("Draw pockets", &m_Game.m_Options.renderPocket);
        ImGui::Checkbox("Draw ball's velocity", &m_Game.m_Options.renderBallVelocity);
        if(ImGui::Checkbox("Camera follow cue ball", &m_Game.m_Options.cameraFollowCueBall) && !m_Game.m_Options.cameraFollowCueBall)
            m_Game.m_View.setCenter(0,0);

        ImGui::TreePop();
    }

    ImGui::End();
    ImGui::SFML::Render(m_Game.m_Window);
}
