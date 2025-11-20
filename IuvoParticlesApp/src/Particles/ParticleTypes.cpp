#pragma once

#include "ParticleTypes.h"
#include "ParticleRandom.h"
#include "ParticleConfig.h"

namespace Particles {

#pragma region ParticleColor Definitions
	const ParticleColor ParticleColor::DEFAULT_COLOR = ParticleColor{};
	ParticleColor ParticleColor::CreateRandomColor() {
		auto& startColor = RandColor();

		ParticleColor out = ParticleColor{
			RandColor(), // startColor
			RandColor(), // endColor
			startColor, // currColor
			RandFloat(0.0f, 1.0f), // lerpSpeed
			true,  // lerpColor
			true,  // fadeColor
			true,  // useAlpha
			true  // randomizeColor			
		};
		return out;
	}
	ParticleColor ParticleColor::CreateRandomColorInRange(const ImVec4& minColor, const ImVec4& maxColor) {
		auto& cfg = GetParticleConfig();
		auto& startColor = RandColorInRange(minColor, maxColor);

		ParticleColor out = ParticleColor{
			startColor, // startColor
			RandColorInRange(minColor, maxColor), // endColor
			startColor, // currColor
			RandFloat(0.0f, 1.0f), // lerpSpeed
			true,  // lerpColor
			true,  // fadeColor
			true,  // useAlpha
			true  // randomizeColor			
		};
		return out;
	}
	const ParticleColor ParticleColor::RED_TO_YELLOW_FADE_OUT = ParticleColor{
		ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // startColor (Red)
		ImVec4(1.0f, 1.0f, 0.0f, 0.0f), // endColor (Yellow, fully transparent)
		ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // currColor (default to start color)
		true,                           // lerpColor
		true,                           // fadeColor
		true,                           // useAlpha
		false                           // randomizeColor
	};
	const ParticleColor ParticleColor::RED_TO_BLUE_FADE_OUT = ParticleColor{
		ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // startColor (Red)
		ImVec4(0.0f, 0.0f, 1.0f, 0.0f), // endColor (Blue, fully transparent)
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
	const ParticleColor ParticleColor::ORANGE_TO_YELLOW_FADE_OUT = ParticleColor{
		ImVec4(1.0f, 0.5f, 0.0f, 1.0f), // startColor (Orange)
		ImVec4(1.0f, 1.0f, 0.0f, 0.0f), // endColor (Yellow, fully transparent)
		ImVec4(1.0f, 0.5f, 0.0f, 1.0f), // currColor (default to start color)
		true,                           // lerpColor
		true,                           // fadeColor
		true,                           // useAlpha
		false                           // randomizeColor
	};
	const ParticleColor ParticleColor::ORANGE_TO_TRANSPARENT_FADE_OUT = ParticleColor{
		ImVec4(1.0f, 0.5f, 0.0f, 1.0f), // startColor (Orange)
		ImVec4(1.0f, 0.5f, 0.0f, 0.0f), // endColor (Orange, fully transparent)
		ImVec4(1.0f, 0.5f, 0.0f, 1.0f), // currColor (default to start color)
		true,                           // lerpColor
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

	const ParticleColor ParticleColor::FIRE_FADE_OUT{
	ImVec4(1.0f, 0.4f, 0.0f, 1.0f),  // fiery orange
	ImVec4(0.8f, 0.0f, 0.0f, 0.0f),  // dark red transparent
	ImVec4(1.0f, 0.4f, 0.0f, 1.0f),
	0.35f, true, true, true, false
	};
	const ParticleColor ParticleColor::ICE_FADE_OUT{
	ImVec4(0.85f, 0.95f, 1.0f, 1.0f),
	ImVec4(0.0f, 0.4f, 1.0f, 0.0f),
	ImVec4(0.85f, 0.95f, 1.0f, 1.0f),
	0.25f, true, true, true, false
	};
	const ParticleColor ParticleColor::TOXIC_FADE_OUT{
	ImVec4(0.0f, 1.0f, 0.3f, 1.0f),
	ImVec4(0.7f, 1.0f, 0.0f, 0.0f),
	ImVec4(0.0f, 1.0f, 0.3f, 1.0f),
	0.3f, true, true, true, false
	};
	const ParticleColor ParticleColor::MAGIC_PURPLE_FADE_OUT{
	ImVec4(0.6f, 0.0f, 1.0f, 1.0f),
	ImVec4(1.0f, 0.3f, 1.0f, 0.0f),
	ImVec4(0.6f, 0.0f, 1.0f, 1.0f),
	0.4f, true, true, true, false
	};

#pragma endregion

#pragma region ParticleLifetime Definitions
	const ParticleLifetime ParticleLifetime::DEFAULT_LIFETIME = ParticleLifetime{};
	ParticleLifetime ParticleLifetime::CreateRandomLifetime() {
		auto& cfg = GetParticleConfig();

		ParticleLifetime out = ParticleLifetime{
			0.0f,
			RandFloat(cfg.lifetime.minMaxLifetime, cfg.lifetime.maxMaxLifetime),
			false,
			false
		};
		return out;
	}
	const ParticleLifetime ParticleLifetime::SHORT_BURST = ParticleLifetime{
		0.0f,
		RandFloat(0.01f, 5.0f),  // maxLifetime 5 second
		false, // randomizeLifetime
		false  // loop
	};
	const ParticleLifetime ParticleLifetime::MEDIUM_BURST = ParticleLifetime{
		0.0f,
		RandFloat(5.0f, 15.0f),
		false,
		false
	};
	const ParticleLifetime ParticleLifetime::LONG_BURST = ParticleLifetime{
		0.0f,
		RandFloat(15.0f, 60.0f),
		false,
		false
	};
	const ParticleLifetime ParticleLifetime::LONG_FLOATING = ParticleLifetime{
		0.0f,
		RandFloat(15.0f, 60.0f),
		false,
		false
	};

#pragma endregion

#pragma region ParticleTransform Definitions
	const ParticleTransform ParticleTransform::DEFAULT_TRANSFORM = ParticleTransform{};
	ParticleTransform ParticleTransform::CreateRandomTransform() {
		auto& cfg = GetParticleConfig();
		ImVec2& _size = RandImVec2Float(cfg.transform.randomSizeMin, cfg.transform.randomSizeMax);
		ParticleTransform out = ParticleTransform{
			ImVec2(
				RandFloat(0.0f, cfg.defaultViewSize.x),
				RandFloat(0.0f, cfg.defaultViewSize.y)
			), // position
			ImVec2(0.0f, 0.0f), // center
			_size, // size
			_size, // baseSize
			ImVec2(
				RandFloat(0.0f, 360.0f),
				0.0f
			), // rotation
			false, // randomPosition
			false, // randomSize
			true   // randomRotation
		};
		return out;
	}
	const ParticleTransform ParticleTransform::SMALL_FLOATING = ParticleTransform{
		ImVec2(0.0f, 0.0f),      // position
		ImVec2(0.0f, 0.0f),      // center
		ImVec2(6.0f, 6.0f),      // size
		ImVec2(6.0f, 6.0f),      // base size
		ImVec2(0.0f, 0.0f),      // rotation
		false,                   // randomPosition
		false,                   // randomSize
		false                    // randomRotation

	};
	const ParticleTransform ParticleTransform::EXPLOSION_CHUNK = ParticleTransform{
		ImVec2(0.0f, 0.0f),
		ImVec2(0.0f, 0.0f),
		ImVec2(14.0f, 14.0f),    // larger debris
		ImVec2(14.0f, 14.0f),
		ImVec2(0.0f, 0.0f),      // initial rotation
		true,    // randomPosition
		true,    // randomSize
		true     // randomRotation
	};
#pragma endregion

#pragma region ParticleAnimation2D Definitions

	const ParticleAnimation2D ParticleAnimation2D::DEFAULT_ANIMATION = ParticleAnimation2D{};
	ParticleAnimation2D ParticleAnimation2D::CreateRandomAnimation2D() {
		auto& cfg = GetParticleConfig();
		ParticleAnimation2D out = ParticleAnimation2D{
			ImVec2(
				RandFloat(cfg.animation.minVelocity.x, cfg.animation.maxVelocity.x),
				RandFloat(cfg.animation.minVelocity.y, cfg.animation.maxVelocity.y)
			), // velocity
			RandFloat(cfg.animation.minMovementSpeed, cfg.animation.maxMovementSpeed), // movementSpeed
			RandFloat(cfg.animation.minRotationSpeed, cfg.animation.maxRotationSpeed), // rotationSpeed
			RandFloat(cfg.animation.minScaleSpeed, cfg.animation.maxScaleSpeed),       // scaleSpeed
			true,  // canMove
			true,  // canRotate
			false  // canScale
		};
		return out;
	}
	const ParticleAnimation2D ParticleAnimation2D::SMALL_RISING = ParticleAnimation2D{
		ImVec2(0.0f, -1.0f), // upward
		40.0f,               // slow rise
		45.0f,               // gentle rotation
		0.1f,                // minimal scale change
		true,                // canMove
		false,               // canRotate
		false                // canScale
	};
	const ParticleAnimation2D ParticleAnimation2D::EXPLOSION = ParticleAnimation2D{
		ImVec2(0.0f, 0.0f),  // velocity randomized elsewhere
		200.0f,              // fast movement
		300.0f,              // aggressive spin
		0.6f,                // big scaling changes
		true,
		true,
		true
	};
	const ParticleAnimation2D ParticleAnimation2D::DRIFTING = ParticleAnimation2D{
		ImVec2(-0.4f, -0.2f),
		15.0f,    // very slow
		10.0f,    // barely rotates
		0.0f,     // no scaling
		true,
		false,
		false
	};
	const ParticleAnimation2D ParticleAnimation2D::SPINNING = ParticleAnimation2D{
		ImVec2(0.0f, 0.0f),
		0.0f,       // no movement
		360.0f,     // fast spin
		0.0f,
		false,
		true,
		false
	};
	const ParticleAnimation2D ParticleAnimation2D::LIGHTWIEGHT_DRIFT = ParticleAnimation2D{
		ImVec2(-0.5f, -0.3f),
		25.0f,
		20.0f,
		0.05f,
		true,
		false,
		false
	};
#pragma endregion



	//const ParticleCollision ParticleCollision::DEFAULT_COLLISION = ParticleCollision{};
	//const ParticleBoundingObject ParticleBoundingObject::DEFAULT_BOUNDING_OBJECT = ParticleBoundingObject{};
	//const ParticlePhysics ParticlePhysics::DEFAULT_PHYSICS = ParticlePhysics{};
	//const ParticleState ParticleState::DEFAULT_STATE = ParticleState{};
	//const ParticleDelegate ParticleDelegate::DEFAULT_DELEGATE = ParticleDelegate{};

}
