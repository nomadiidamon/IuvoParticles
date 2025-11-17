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

	RectParticle RED_TO_YELLOW_FADE_OUT {
		// Transform
		{
			ImVec2(0.0f, 0.0f), // position
			ImVec2(0.0f, 0.0f), // center
			ImVec2(50.0f, 50.0f), // size
			ImVec2(50.0f, 50.0f), // baseSize
			ImVec2(67.0f, 0.0f), // rotation
			false, // randomPosition
			false, // randomSize
			false  // randomRotation
		},
		// Animation
		{
			ImVec2(-7.0f, -3.0f), // velocity
			75.0f,               // movementSpeed
			87.0f,                // rotationSpeed
			0.0f,                // scaleSpeed
			true,                // canMove
			true,               // canRotate
			false                // canScale
		},
		// Color
		{
			ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // startColor (Red)
			ImVec4(1.0f, 1.0f, 0.0f, 0.0f), // endColor (Yellow, fully transparent)
			ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // currColor
			true,                           // lerpColor
			true,                           // fadeColor
			true,                           // useAlpha
			false                           // randomizeColor
		},
		// Lifetime
		{
			0.0f,   // lifetime
			5.0f,   // maxLifetime (5 seconds)
			false,  // randomizeLifetime
			false   // loop
		}
	};

	struct BackgroundParticle {
		ParticleColor bg_colorData;
		ParticleLifetime bg_lifetimeData;
	};

	class BackgroundParticleUtils {

	};

}