#pragma once
#include "RectParticle/RectParticle.h"
#include "ParticleRandom.h"

namespace Particles {

    struct ParticleConfig {
        /// Transform
        // Position fallback (center)
        ImVec2 defaultEmitterPosition = ImVec2(150.0f, 150.0f);
        // Size ranges for randomized size (sx, sy)
        ImVec2 randomSizeMin = ImVec2(1.0f, 1.0f);
        ImVec2 randomSizeMax = ImVec2(250.0f, 250.0f);
        // Fallback base-size when none supplied
        ImVec2 defaultBaseSize = ImVec2(10.0f, 10.0f);
        ImVec2 defaultSpawnSize = ImVec2(10.0f, 10.0f);

		/// Animation
        // Rotation ranges (degrees)
        float rotationMin = 0.0f;
        float rotationMax = 360.0f;
        // Movement speed range (units/sec)
        float movementSpeedMin = 0.0f;
        float movementSpeedMax = 100.0f;
        // Rotation speed range (degrees/sec)
        float rotationSpeedMin = -90.0f;
        float rotationSpeedMax = 90.0f;
        // Scale speed default (used by scaling math)
        float scaleSpeedDefault = 0.35f;
		float scaleSpeedMin = 0.1f;
		float scaleSpeedMax = 1.0f;
		// Velocity ranges (units/sec)
		ImVec2 velocityMin = ImVec2(-50.0f, -50.0f);
		ImVec2 velocityMax = ImVec2(50.0f, 50.0f);

        /// Lifetime
        // Lifetime ranges (seconds)
        float lifetimeMin = 0.1f;
        float lifetimeMax = 60.0f;
        float lifetimeRandomMin = 0.5f;
        float lifetimeRandomMax = 60.0f;

        /// Color
        // Color randomization defaults (0..1)
        ImVec4 defaultStartColor = RandColor();
        ImVec4 defaultEndColor = RandColor();
        ImVec4 defaultCurrColor = RandColor();
        ImVec4 startColorMin = RandColor();
        ImVec4 endColorMin = RandColor();

        // Default movement/behavior
        ImVec2 defaultVelocity = ImVec2(RandFloat(-50.0f, -50.0f), RandFloat(50.0f, 50.0f));
        float defaultMovementSpeed = 50.0f;
        bool  defaultCanMove = true;
        bool  defaultCanRotate = true;
        bool  defaultCanScale = false;

        // Defaults for emitter/system
        int   defaultMaxParticles = 200;
        float defaultEmissionRate = 10.0f; // particles/sec
        ImVec2 defaultViewSize = ImVec2(800.0f, 600.0f);

        // Very small epsilon for comparisons
        float eps = 1e-6f;

        // Template particles that can be swapped out at runtime
        RectParticle defaultEmitterTemplate;
        RectParticle defaultSpawnTemplate;

        // A helper to reset templates to sensible defaults
        void ResetTemplates() {
            // default emitter: center
            defaultEmitterTemplate = RectParticle();
            defaultEmitterTemplate.rp_transform.p_position = defaultEmitterPosition;
            defaultEmitterTemplate.rp_transform.p_size = defaultSpawnSize;
            defaultEmitterTemplate.rp_transform.p_baseSize = defaultSpawnSize;
            defaultEmitterTemplate.rp_transform.p_randomSize = false;
            defaultEmitterTemplate.rp_transform.p_randomRotation = false;

            defaultEmitterTemplate.rp_animation.p_velocity = defaultVelocity;
            defaultEmitterTemplate.rp_animation.p_movementSpeed = defaultMovementSpeed;
            defaultEmitterTemplate.rp_animation.canMove = defaultCanMove;
            defaultEmitterTemplate.rp_animation.canRotate = defaultCanRotate;
            defaultEmitterTemplate.rp_animation.canScale = defaultCanScale;

            defaultEmitterTemplate.rp_colorData.p_startColor = defaultStartColor;
            defaultEmitterTemplate.rp_colorData.p_endColor = defaultEndColor;
            defaultEmitterTemplate.rp_colorData.p_lerpColor = true;
            defaultEmitterTemplate.rp_colorData.p_useAlpha = true;

            defaultEmitterTemplate.rp_lifetimeData.p_maxLifetime = (lifetimeMin + lifetimeMax) * 0.5f;
            defaultEmitterTemplate.rp_lifetimeData.p_randomizeLifetime = false;

            // spawn template left as a copy of emitter initially
            defaultSpawnTemplate = defaultEmitterTemplate;
        }
    };

    // return a global config instance (modifiable at runtime)
    inline ParticleConfig& GetParticleConfig() {
        static ParticleConfig cfg;
        static bool initialized = false;
        if (!initialized) {
            cfg.ResetTemplates();
            initialized = true;
        }
        return cfg;
    }
}