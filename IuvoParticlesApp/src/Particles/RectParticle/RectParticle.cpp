#pragma once

#include "RectParticle.h"

namespace Particles {

	const RectParticle DEFAULT_RECT_PARTICLE{
		// Transform
		ParticleTransform::DEFAULT_TRANSFORM,
		// Animation
		ParticleAnimation2D::DEFAULT_ANIMATION,
		// Color
		ParticleColor::DEFAULT_COLOR,
		// Lifetime
		ParticleLifetime::DEFAULT_LIFETIME
	};

	const RectParticle RED_TO_YELLOW_FADE_OUT {
		// Transform
		ParticleTransform::DEFAULT_TRANSFORM,
		// Animation
		ParticleAnimation2D::DEFAULT_ANIMATION,
		// Color
		ParticleColor::RED_TO_YELLOW_FADE_OUT,
		// Lifetime
		ParticleLifetime::DEFAULT_LIFETIME
	};
	const RectParticle RED_TO_TRANSPARENT_FADE_OUT{
		// Transform
		ParticleTransform::DEFAULT_TRANSFORM,
		// Animation
		ParticleAnimation2D::DEFAULT_ANIMATION,
		// Color
		ParticleColor::RED_TO_TRANSPARENT_FADE_OUT,
		// Lifetime
		ParticleLifetime::DEFAULT_LIFETIME
	};

	const RectParticle YELLOW_TO_GREEN_FADE_OUT{
		// Transform
		ParticleTransform::DEFAULT_TRANSFORM,
		// Animation
		ParticleAnimation2D::DEFAULT_ANIMATION,
		// Color
		ParticleColor::YELLOW_TO_GREEN_FADE_OUT,
		// Lifetime
		ParticleLifetime::DEFAULT_LIFETIME
	};
	const RectParticle YELLOW_TO_TRANSPARENT_FADE_OUT{
		// Transform
		ParticleTransform::DEFAULT_TRANSFORM,
		// Animation
		ParticleAnimation2D::DEFAULT_ANIMATION,
		// Color
		ParticleColor::YELLOW_TO_TRANSPARENT_FADE_OUT,
		// Lifetime
		ParticleLifetime::DEFAULT_LIFETIME
	};

	const RectParticle BLUE_TO_CYAN_FADE_OUT{
		// Transform
		ParticleTransform::DEFAULT_TRANSFORM,
		// Animation
		ParticleAnimation2D::DEFAULT_ANIMATION,
		// Color
		ParticleColor::BLUE_TO_CYAN_FADE_OUT,
		// Lifetime
		ParticleLifetime::DEFAULT_LIFETIME
	};
	const RectParticle BLUE_TO_TRANSPARENT_FADE_OUT{
		// Transform
		ParticleTransform::DEFAULT_TRANSFORM,
		// Animation
		ParticleAnimation2D::DEFAULT_ANIMATION,
		// Color
		ParticleColor::BLUE_TO_TRANSPARENT_FADE_OUT,
		// Lifetime
		ParticleLifetime::DEFAULT_LIFETIME
	};

}
