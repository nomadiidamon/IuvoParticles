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
		bool randomizeEverything = false;


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

		void ApplyColorPreset(const ParticleColor& presetData) {
			defaultParticle.color.startColor = presetData.startColor;
			defaultParticle.color.endColor = presetData.endColor;
			defaultParticle.color.lerpSpeed = presetData.lerpSpeed;
			defaultParticle.color.lerpColor = presetData.lerpColor;
		}

		void ApplyLifetimePreset(const ParticleLifetime& presetData) {
			defaultParticle.lifetime.maxLifetime = presetData.maxLifetime;
			defaultParticle.lifetime.randomizeLifetime = presetData.randomizeLifetime;
		}

		void ApplyTransformPreset(const ParticleTransform& presetData) {
			//defaultParticle.rp_transform.p_position = presetData.p_position;
			defaultParticle.transform.size = presetData.size;
			defaultParticle.transform.randomSize = presetData.randomSize;
			defaultParticle.transform.randomRotation = presetData.randomRotation;
			defaultParticle.transform.rotation = presetData.rotation;
		}

		void ApplyAnimationPreset(const ParticleAnimation2D& presetData) {
			defaultParticle.animation.p_velocity = presetData.p_velocity;
			defaultParticle.animation.p_movementSpeed = presetData.p_movementSpeed;
			defaultParticle.animation.p_rotationSpeed = presetData.p_rotationSpeed;
			defaultParticle.animation.p_scaleSpeed = presetData.p_scaleSpeed;
			defaultParticle.animation.canMove = presetData.canMove;
			defaultParticle.animation.canRotate = presetData.canRotate;
			defaultParticle.animation.canScale = presetData.canScale;
		}

		void ApplyParticlePreset(const RectParticle& presetData) {
			ApplyTransformPreset(presetData.transform);
			ApplyColorPreset(presetData.color);
			ApplyLifetimePreset(presetData.lifetime);
			ApplyAnimationPreset(presetData.animation);
		}
    };


    // Global config instance
    inline LayerConfig& GetLayerConfig()
    {
        static LayerConfig cfg;
        return cfg;
    }
}
