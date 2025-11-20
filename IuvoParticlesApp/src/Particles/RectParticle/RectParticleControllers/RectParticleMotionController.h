#pragma once
#include "../RectParticle.h"

namespace Particles {

    class RectParticleMotionController {
    public:
        static void ApplyVelocity(RectParticle& p, float ts);
        static bool HitBounds(const RectParticle& p, const ImVec2& view);
        static bool HitBoundsRotated(const RectParticle& p, const ImVec2& view);
        static bool ApplyRebound(RectParticle& p, const ImVec2& view);

        static void ComputeRotatedCorners(const RectParticle& p, ImVec2 out[4]);
    };

}