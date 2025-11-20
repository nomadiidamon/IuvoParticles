#pragma once
#include "../RectParticle.h"
#include "../../ParticleConfig.h"

namespace Particles {

    class RectParticleScaleController {
    public:
        static void ApplyScale(RectParticle& p, float ts);
    };

}