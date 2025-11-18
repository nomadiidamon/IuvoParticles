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

	extern const RectParticle DEFAULT_RECT_PARTICLE;

	extern const RectParticle RED_TO_YELLOW_FADE_OUT;
	extern const RectParticle RED_TO_TRANSPARENT_FADE_OUT;

	extern const RectParticle YELLOW_TO_GREEN_FADE_OUT;
	extern const RectParticle YELLOW_TO_TRANSPARENT_FADE_OUT;

	extern const RectParticle BLUE_TO_CYAN_FADE_OUT;
	extern const RectParticle BLUE_TO_TRANSPARENT_FADE_OUT;

	enum class RectParticlePreset {
		NONE,
		RED_TO_YELLOW_FADE_OUT,
		RED_TO_TRANSPARENT_FADE_OUT,
		YELLOW_TO_GREEN_FADE_OUT,
		YELLOW_TO_TRANSPARENT_FADE_OUT,
		BLUE_TO_CYAN_FADE_OUT,
		BLUE_TO_TRANSPARENT_FADE_OUT
	};

	struct BackgroundParticle {
		ParticleColor bg_colorData;
		ParticleLifetime bg_lifetimeData;
	};

	class BackgroundParticleUtils {

	};

}