#pragma once
#include "RectParticle.h"
#include "RectParticleUtils.h"

namespace Particles {
    struct RectParticleCache {
        ImVec2 cachedCenter = ImVec2(0.0f, 0.0f);
        ImVec2 cachedCorners[4];    // rotated rect corners
        ImU32  cachedColor = 0;     // current display color (as U32)
        float  cachedLifetimeRatio = 0.0f;
        bool   isExpired = false;

        void UpdateFromParticle(const RectParticle& particle) {
            // Lifetime ratio
            cachedLifetimeRatio = particle.rp_lifetimeData.p_maxLifetime > 0.0f ?
                particle.rp_lifetimeData.p_lifetime / particle.rp_lifetimeData.p_maxLifetime : 0.0f;
            cachedLifetimeRatio = std::clamp(cachedLifetimeRatio, 0.0f, 1.0f);

            // Expiration
            isExpired = particle.rp_lifetimeData.p_lifetime >= particle.rp_lifetimeData.p_maxLifetime;

            // Center
            cachedCenter = ImVec2(
                particle.rp_transform.p_position.x + particle.rp_transform.p_size.x * 0.5f,
                particle.rp_transform.p_position.y + particle.rp_transform.p_size.y * 0.5f
            );

            // Color (already updated in utils update)
            cachedColor = ImGui::ColorConvertFloat4ToU32(particle.rp_colorData.p_currColor);

            // Rotated corners
            RectParticleUtils::CalculateRotatedRectCorners(particle, cachedCorners);
        }
    };

}
