#pragma once
#include "../RectParticle.h"

namespace Particles {

    class RectParticleRotationController {
    public:
        static void ApplyRotation(RectParticle& particle, float ts);
        static void NormalizeRotation(RectParticle& p);
    };

}