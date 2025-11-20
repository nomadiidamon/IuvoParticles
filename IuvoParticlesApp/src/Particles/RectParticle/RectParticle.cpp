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
	const RectParticle FIRE_FADE_OUT{
	ParticleTransform::DEFAULT_TRANSFORM,
	ParticleAnimation2D::DEFAULT_ANIMATION,
	ParticleColor::FIRE_FADE_OUT,
	ParticleLifetime::DEFAULT_LIFETIME
	};
	const RectParticle SMOKE_PUFF{
	ParticleTransform::SMALL_FLOATING,
	ParticleAnimation2D{
		ImVec2(0.0f, -0.2f),
		10.0f,
		0.0f,
		0.05f,
		true, false, true
	},
	ParticleColor{
		ImVec4(0.4f,0.4f,0.4f,1.0f),
		ImVec4(0.4f,0.4f,0.4f,0.0f),
		ImVec4(0.4f,0.4f,0.4f,1.0f),
		0.2f,
		true,true,true,false
	},
	ParticleLifetime::MEDIUM_BURST
	};
	const RectParticle SPARK_EMBER{
	ParticleTransform::SMALL_FLOATING,
	ParticleAnimation2D::SMALL_RISING,
	ParticleColor::ORANGE_TO_TRANSPARENT_FADE_OUT,
	ParticleLifetime::SHORT_BURST
	};
	const RectParticle EXPLOSION_DEBRIS{
	ParticleTransform::EXPLOSION_CHUNK,
	ParticleAnimation2D::EXPLOSION,
	ParticleColor::RED_TO_YELLOW_FADE_OUT,
	ParticleLifetime::SHORT_BURST
	};
	const RectParticle RAINBOW_CYCLE{
	ParticleTransform::DEFAULT_TRANSFORM,
	ParticleAnimation2D::SPINNING,
	ParticleColor::CreateRandomColor(),
	ParticleLifetime::LONG_FLOATING
	};
	const RectParticle RAIN_STREAK{
	ParticleTransform{
		ImVec2(0,0), ImVec2(0,0),
		ImVec2(2,14), ImVec2(2,14),
		ImVec2(0,0),
		true,false,true
	},
	ParticleAnimation2D{
		ImVec2(0.0f, 1.0f),
		150.0f,
		0.0f,
		0.0f,
		true,false,false
	},
	ParticleColor::BLUE_TO_TRANSPARENT_FADE_OUT,
	ParticleLifetime::MEDIUM_BURST
	};

	RectParticle RectParticle::RANDOM_PARTICLE() {
		return RectParticle{
			ParticleTransform::CreateRandomTransform(),
			ParticleAnimation2D::CreateRandomAnimation2D(),
			ParticleColor::CreateRandomColor(),
			ParticleLifetime::CreateRandomLifetime()
		};
	}

	std::string RectParticle::ToString(const RectParticle& particle) {
		// Implementation to serialize RectParticle to string
		return "";
	}

	RectParticle RectParticle::FromString(const std::string& str) {
		// Implementation to deserialize RectParticle from string
		return RectParticle{};
	}
}
