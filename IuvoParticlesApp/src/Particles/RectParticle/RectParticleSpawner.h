#pragma once
#include "../ParticleConfig.h"

namespace Particles {

    class RectParticleSpawner {
    public:
        static RectParticle FromTemplate(const RectParticle& t);
		static RectParticle CreateParticle(const ParticleTransform& transform,
			const ParticleColor& color,
			const ParticleLifetime& lifetime,
			const ParticleAnimation2D& animation = ParticleAnimation2D());
        static RectParticle CreateRandom(const ImVec2& view);

    };
}