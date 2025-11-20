#pragma once
#include "../RectParticle.h"

namespace Particles {

    class RectParticleLifetimeController {
    public:
        static void UpdateLifetime(RectParticle& particle, float ts);
        static bool IsExpired(const RectParticle& particle, float& outLifeRatio);
    };

}