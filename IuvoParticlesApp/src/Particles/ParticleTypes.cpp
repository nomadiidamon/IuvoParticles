#pragma once

#include "ParticleTypes.h"
#include "ParticleUtils.h"

namespace Particles {


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
