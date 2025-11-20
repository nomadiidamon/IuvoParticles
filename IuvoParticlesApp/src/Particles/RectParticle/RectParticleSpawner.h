#pragma once
#include "../ParticleConfig.h"

namespace Particles {

    class RectParticleSpawner {
    public:
        static RectParticle FromTemplate(const RectParticle& t);
        static RectParticle CreateRandom(const ImVec2& view);
    };
}