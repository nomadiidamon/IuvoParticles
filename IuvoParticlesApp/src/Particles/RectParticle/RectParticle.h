#pragma once
#include "../ParticleRandom.h"
#include "../ParticleTypes.h"

namespace Particles {

	struct RectParticle {
		ParticleTransform rp_transform;
		ParticleAnimation2D rp_animation;
		ParticleColor rp_colorData;
		ParticleLifetime rp_lifetimeData;
	};

	struct BackgroundParticle {
		ParticleColor bg_colorData;
		ParticleLifetime bg_lifetimeData;
	};

	static class BackgroundParticleUtils {

	};









}