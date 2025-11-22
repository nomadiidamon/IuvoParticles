#pragma once
#include "imgui.h"
#include "../../json/src/json.hpp"

namespace Particles {

	enum class ParticleAnimation2DPreset {
		NONE,
		SMALL_RISING,
		EXPLOSION,
		DRIFTING,
		SPINNING,
		LIGHTWIEGHT_DRIFT
	};
	struct ParticleAnimation2D {
		ImVec2 p_velocity = ImVec2(-3.0f, -1.5f);

		float p_movementSpeed = 85.5f;
		float p_rotationSpeed = 149.67f; // degrees per second
		float p_scaleSpeed = 0.35f;

		bool canMove = true;
		bool canRotate = true;
		bool canScale = false;

		static const ParticleAnimation2D DEFAULT_ANIMATION;
		static ParticleAnimation2D CreateRandomAnimation2D();
		static const ParticleAnimation2D SMALL_RISING;
		static const ParticleAnimation2D EXPLOSION;
		static const ParticleAnimation2D DRIFTING;
		static const ParticleAnimation2D SPINNING;
		static const ParticleAnimation2D LIGHTWIEGHT_DRIFT;
		static bool IsEqual(const ParticleAnimation2D& a, const ParticleAnimation2D& b);

	};
	struct ParticleAnimation2DRange {
		bool enforceConstraints = false;

		ImVec2 minVelocity = ImVec2(-5.0f, -5.0f);
		ImVec2 maxVelocity = ImVec2(5.0f, 5.0f);
		ImVec2 randomVelocityMin = ImVec2(-1.0f, -1.0f);
		ImVec2 randomVelocityMax = ImVec2(1.0f, 1.0f);
		ImVec2 defaultVelocity = ImVec2(0.0f, -1.0f);
		float minVelocityMagnitude = 0.0f;
		float maxVelocityMagnitude = 200.0f;

		float minMovementSpeed = 20.0f;
		float maxMovementSpeed = 200.0f;
		float randomMovementSpeedMin = 20.0f;
		float randomMovementSpeedMax = 200.0f;
		float defaultMovementSpeed = 85.0f;

		float minRotationSpeed = -360.0f;
		float maxRotationSpeed = 360.0f;
		float randomRotationSpeedMin = -180.0f;
		float randomRotationSpeedMax = 180.0f;
		float defaultRotationSpeed = 90.0f;

		float minScaleSpeed = 0.0f;
		float maxScaleSpeed = 1.0f;
		float randomScaleSpeedMin = 0.0f;
		float randomScaleSpeedMax = 1.0f;
		float defaultScaleSpeed = 0.35f;

		static const ParticleAnimation2DRange DEFAULT_ANIMATION_RANGE;
	};

	static class ParticleAnimation2DSerializer : nlohmann::json {

	public:
		static nlohmann::json Serialize(const ParticleAnimation2D& animation);
		static ParticleAnimation2D Deserialize(const nlohmann::json& j);
	};


}