#pragma once
#include "ParticleUtils.h"
#include "ParticleAnimation2D.h"

namespace Particles {

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
	bool ParticleAnimation2D::IsEqual(const ParticleAnimation2D& a, const ParticleAnimation2D& b) {
		return Particles::FloatEquals(a.p_velocity.x, b.p_velocity.x) &&
			Particles::FloatEquals(a.p_velocity.y, b.p_velocity.y) &&
			Particles::FloatEquals(a.p_movementSpeed, b.p_movementSpeed) &&
			Particles::FloatEquals(a.p_rotationSpeed, b.p_rotationSpeed) &&
			Particles::FloatEquals(a.p_scaleSpeed, b.p_scaleSpeed) &&
			(a.canMove == b.canMove) &&
			(a.canRotate == b.canRotate) &&
			(a.canScale == b.canScale);
	}


	nlohmann::json ParticleAnimation2DSerializer::Serialize(const ParticleAnimation2D& animation) {
		nlohmann::json j;
		j["p_velocity"] = { animation.p_velocity.x, animation.p_velocity.y };
		j["p_movementSpeed"] = animation.p_movementSpeed;
		j["p_rotationSpeed"] = animation.p_rotationSpeed;
		j["p_scaleSpeed"] = animation.p_scaleSpeed;
		j["canMove"] = animation.canMove;
		j["canRotate"] = animation.canRotate;
		j["canScale"] = animation.canScale;
		return j;
	}
	ParticleAnimation2D ParticleAnimation2DSerializer::Deserialize(const nlohmann::json& j) {
		ParticleAnimation2D animation;
		animation.p_velocity = ImVec2(j["p_velocity"][0], j["p_velocity"][1]);
		animation.p_movementSpeed = j["p_movementSpeed"];
		animation.p_rotationSpeed = j["p_rotationSpeed"];
		animation.p_scaleSpeed = j["p_scaleSpeed"];
		animation.canMove = j["canMove"];
		animation.canRotate = j["canRotate"];
		animation.canScale = j["canScale"];
		return animation;
	}
}