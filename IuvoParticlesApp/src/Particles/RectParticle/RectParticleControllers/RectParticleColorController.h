#pragma once
#include "../RectParticle.h"

namespace Particles {

    class RectParticleColorController {
    public:
        static ImU32 LerpColor(RectParticle& particle);
        static ImU32 FadeColor(RectParticle& particle);
        static void UpdateColor(RectParticle& particle, float& lifetimeRatio);
    };

}