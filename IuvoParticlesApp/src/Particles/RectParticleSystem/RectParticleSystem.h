#pragma once
#include "../RectParticle/RectParticleUtils.h"
#include "../RectParticle/RectParticleCache.h"

namespace Particles {
    struct ParticleSystem {
        Particles::RectParticle emitterParticle;
        Particles::RectParticle spawnParticle;
        std::vector<Particles::RectParticle> activeParticles;
        std::vector<RectParticleCache> particleCache;

        // exterior position array
        float s_position[2] = { 0.0f, 0.0f };

        // Emitter configuration
        ImVec2 viewSize = ImVec2(800.0f, 600.0f);
        int maxParticles = 200;
        bool useDefaultParticle = false;
        float emissionRate = 10.0f; // particles per second
        float emissionAccumulator = 0.0f; // internal timing accumulator
    };

}