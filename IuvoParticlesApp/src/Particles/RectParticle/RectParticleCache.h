#pragma once
#include "../RectParticle/RectParticleUtils.h"


namespace Particles {

    struct RectParticleCache {
        ImVec2 cachedCenter = ImVec2(0.0f, 0.0f);
        ImVec2 cachedCorners[4];    // rotated rect corners
        ImU32  cachedColor = 0;     // current display color (as U32)
        float  cachedLifetimeRatio = 0.0f;
        bool   isExpired = false;

        void UpdateFromParticle(const RectParticle& particle) {
            // Lifetime ratio
            cachedLifetimeRatio = particle.lifetime.maxLifetime > 0.0f ?
                particle.lifetime.lifetime / particle.lifetime.maxLifetime : 0.0f;
            cachedLifetimeRatio = std::clamp(cachedLifetimeRatio, 0.0f, 1.0f);

            // Expiration
            isExpired = particle.lifetime.lifetime >= particle.lifetime.maxLifetime;

            // Center
            cachedCenter = ImVec2(
                particle.transform.position.x + particle.transform.size.x * 0.5f,
                particle.transform.position.y + particle.transform.size.y * 0.5f
            );

            // Color (already updated in utils update)
            cachedColor = ImGui::ColorConvertFloat4ToU32(particle.color.currColor);

            // Rotated corners
            RectParticleUtils::CalculateRotatedRectCorners(particle, cachedCorners);
        }
    };

}
