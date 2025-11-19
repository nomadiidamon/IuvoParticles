#pragma once
#include "../Particles/RectParticle/RectParticleUtils.h"
#include <imgui.h>

namespace Particles {

    struct LayerConfig
    {
        // Particle pool sizes
        int defaultParticleCount = 100;
        int maxParticleCount = 20000;
		int currentParticleCount = 0;

        // Hover / click
        ImVec2 mouse_pos = ImVec2(0.0f, 0.0f);
        ImVec2 lastClickPos = ImVec2(0.0f, 0.0f);
        bool emitOnHover = false;
		bool isHovering = false;
		bool wasClicked = false;
        int hoverBurstCount = 25;
        float hoverBurstInterval = 0.05f;
		float hoverBurstTimer = 0.0f;


        ImVec2 view_pos = ImVec2(0.0f, 0.0f);
        ImVec2 view_size = ImVec2(800.0f, 600.0f);
        ImVec2 min_view_size = ImVec2(200, 200);


        // Runtime control
        bool paused = false;
        bool stepOnce = false;
        float globalSpeedMultiplier = 1.0f;
        bool useDefaultParticle = true;


        // Physics
        ImVec2 gravity = ImVec2(0.0f, 200.0f);
        float drag = 0.1f;

        // Templates
        RectParticle defaultParticle;
        BackgroundParticle backgroundParticle;
		ParticleAnimation2DPreset animationPreset = ParticleAnimation2DPreset::NONE;
		ParticleColorPreset colorPreset = ParticleColorPreset::RED_TO_YELLOW_FADE_OUT;
		ParticleLifetimePreset lifetimePreset = ParticleLifetimePreset::MEDIUM_BURST;
		ParticleTransformPreset transformPreset = ParticleTransformPreset::NONE;
		RectParticlePreset rectParticlePreset = RectParticlePreset::NONE;
    };

    // Global config instance
    inline LayerConfig& GetLayerConfig()
    {
        static LayerConfig cfg;
        return cfg;
    }
}
