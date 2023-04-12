#pragma once

#include <SFML/Graphics/Glsl.hpp>

struct LightingProperties {
    sf::Glsl::Vec3 lightPosition = {0.0f, 0.0f, 400.0f};
    sf::Glsl::Vec3 lightColor = {1.0f, 1.0f, 1.0f};
    float ambientIntensity = 0.2f;
    float diffuseIntensity = 0.8f;
    float specularIntensity = 0.3f;
    float shininess = 30.0f;
    bool shadowMappingEnabled = false;
};

