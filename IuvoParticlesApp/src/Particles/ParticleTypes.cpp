#pragma once

#include "ParticleTypes.h"

namespace Particles {

#pragma region ParticleColor Definitions
	const ParticleColor ParticleColor::DEFAULT_COLOR = ParticleColor{};

	const ParticleColor ParticleColor::RED_TO_YELLOW_FADE_OUT = ParticleColor{
		ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // startColor (Red)
		ImVec4(1.0f, 1.0f, 0.0f, 0.0f), // endColor (Yellow, fully transparent)
		ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // currColor (default to start color)
		true,                           // lerpColor
		true,                           // fadeColor
		true,                           // useAlpha
		false                           // randomizeColor
	};
	const ParticleColor ParticleColor::RED_TO_TRANSPARENT_FADE_OUT = ParticleColor{
		ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // startColor (Red)
		ImVec4(1.0f, 0.0f, 0.0f, 0.0f), // endColor (Red, fully transparent)
		ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // currColor  (default to start color)
		true,                          // lerpColor
		true,                           // fadeColor
		true,                           // useAlpha
		false                           // randomizeColor
	};
	const ParticleColor ParticleColor::YELLOW_TO_GREEN_FADE_OUT = ParticleColor{
		ImVec4(1.0f, 1.0f, 0.0f, 1.0f), // startColor (Yellow)
		ImVec4(0.0f, 1.0f, 0.0f, 0.0f), // endColor (Green, fully transparent)
		ImVec4(1.0f, 1.0f, 0.0f, 1.0f), // currColor  (default to start color)
		true,                           // lerpColor
		true,                           // fadeColor
		true,                           // useAlpha
		false                           // randomizeColor
	};
	const ParticleColor ParticleColor::YELLOW_TO_TRANSPARENT_FADE_OUT = ParticleColor{
		ImVec4(1.0f, 1.0f, 0.0f, 1.0f), // startColor (Yellow)
		ImVec4(1.0f, 1.0f, 0.0f, 0.0f), // endColor (Yellow, fully transparent)
		ImVec4(1.0f, 1.0f, 0.0f, 1.0f), // currColor (default to start color)
		true,                           // lerpColor
		true,                           // fadeColor
		true,                           // useAlpha
		false                           // randomizeColor
	};
	const ParticleColor ParticleColor::BLUE_TO_CYAN_FADE_OUT = ParticleColor{
		ImVec4(0.0f, 0.0f, 1.0f, 1.0f), // startColor (Blue)
		ImVec4(0.0f, 1.0f, 1.0f, 0.0f), // endColor (Cyan, fully transparent)
		ImVec4(0.0f, 0.0f, 1.0f, 1.0f), // currColor (default to start color)
		true,                           // lerpColor
		true,                           // fadeColor
		true,                           // useAlpha
		false                           // randomizeColor
	};
	const ParticleColor ParticleColor::BLUE_TO_TRANSPARENT_FADE_OUT = ParticleColor{
		ImVec4(0.0f, 0.0f, 1.0f, 1.0f), // startColor (Blue)
		ImVec4(0.0f, 0.0f, 1.0f, 0.0f), // endColor (Blue, fully transparent)
		ImVec4(0.0f, 0.0f, 1.0f, 1.0f), // currColor (default to start color)
		true,                          // lerpColor
		true,                           // fadeColor
		true,                           // useAlpha
		false                           // randomizeColor
	};
#pragma endregion

	const ParticleLifetime ParticleLifetime::DEFAULT_LIFETIME = ParticleLifetime{};

	const ParticleTransform ParticleTransform::DEFAULT_TRANSFORM = ParticleTransform{};

	const ParticleAnimation2D ParticleAnimation2D::DEFAULT_ANIMATION = ParticleAnimation2D{};

}
