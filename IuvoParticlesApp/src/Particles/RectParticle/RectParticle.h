#pragma once
#include "../ParticleTypes.h"
#include "../ParticleUtils.h"

namespace Particles {

	struct RectParticle {
		ParticleTransform transform;
		ParticleAnimation2D animation;
		ParticleColor color;
		ParticleLifetime lifetime;
		
		static RectParticle RANDOM_PARTICLE();
		static std::string ToString(const RectParticle& particle);
		static RectParticle FromString(const std::string& str);
	};

	extern const RectParticle DEFAULT_RECT_PARTICLE;
	extern const RectParticle RED_TO_YELLOW_FADE_OUT;
	extern const RectParticle RED_TO_TRANSPARENT_FADE_OUT;
	extern const RectParticle YELLOW_TO_GREEN_FADE_OUT;
	extern const RectParticle YELLOW_TO_TRANSPARENT_FADE_OUT;
	extern const RectParticle BLUE_TO_CYAN_FADE_OUT;
	extern const RectParticle BLUE_TO_TRANSPARENT_FADE_OUT;
	extern const RectParticle FIRE_FADE_OUT;
	extern const RectParticle SMOKE_PUFF;
	extern const RectParticle SPARK_EMBER;
	extern const RectParticle EXPLOSION_DEBRIS;
	extern const RectParticle RAINBOW_CYCLE;
	extern const RectParticle RAIN_STREAK;

	enum class RectParticlePreset {
		NONE,
		RED_TO_YELLOW_FADE_OUT,
		RED_TO_TRANSPARENT_FADE_OUT,
		YELLOW_TO_GREEN_FADE_OUT,
		YELLOW_TO_TRANSPARENT_FADE_OUT,
		BLUE_TO_CYAN_FADE_OUT,
		BLUE_TO_TRANSPARENT_FADE_OUT,
		FIRE_FADE_OUT,
		SMOKE_PUFF,
		SPARK_EMBER,
		EXPLOSION_DEBRIS,
		RAINBOW_CYCLE,
		RAIN_STREAK
	};

	struct BackgroundParticle {
		ParticleColor bg_colorData;
		ParticleLifetime bg_lifetimeData;
	};

	class BackgroundParticleUtils {

	};

}