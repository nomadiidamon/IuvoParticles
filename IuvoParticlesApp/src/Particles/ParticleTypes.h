#pragma once
#include "ParticleColor.h"
#include "ParticleTransform.h"

namespace Particles {

	enum class ParticleLifetimePreset {
		NONE,
		SHORT_BURST,
		MEDIUM_BURST,
		LONG_BURST,
		LONG_FLOATING
	};
	struct ParticleLifetime {
		float p_lifetime = 0.0f;
		float p_maxLifetime = 5.0f;
		bool p_randomizeLifetime = false;
		bool p_loop = false;
		int p_loopCount = 0; // number of times the particle has looped

		static const ParticleLifetime DEFAULT_LIFETIME;
		static ParticleLifetime CreateRandomLifetime();
		static const ParticleLifetime SHORT_BURST;
		static const ParticleLifetime MEDIUM_BURST;
		static const ParticleLifetime LONG_BURST;
		static const ParticleLifetime LONG_FLOATING;
	};
	struct ParticleLifetimeRange {
		bool enforceConstraints = false;

		float minMaxLifetime = 1.0f;
		float maxMaxLifetime = 10.0f;
		float randomLifetimeMin = 1.0f;
		float randomLifetimeMax = 10.0f;
		float defaultMaxLifetime = 5.0f;
		int minLoopCount = 0;
		int maxLoopCount = 5;
		static const ParticleLifetimeRange DEFAULT_LIFETIME_RANGE;
	};


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


}
	//struct ParticleCollision {
	//	bool p_enableCollision = false;
	//	float p_bounciness = 0.5f; // 0 = no bounce, 1 = full bounce
	//	float p_friction = 0.2f;   // 0 = no friction, 1 = full stop
	//	std::vector<ImVec2> p_collisionBounds; // list of all auto computed collision bounds
	//	static const ParticleCollision DEFAULT_COLLISION;
	//};

	//enum class ParticleBoundingType {
	//	NONE,
	//	CIRCLE,
	//	RECTANGLE,
	//	POLYGON
	//};

	//struct ParticleBoundingObject {
	//	ParticleBoundingType p_type = ParticleBoundingType::NONE;
	//	ImVec2 p_size = ImVec2(0.0f, 0.0f); // size of bounding object
	//	float p_radius = 0.0f;              // radius for circle bounding object
	//	std::vector<ImVec2> p_polygonPoints; // points for polygon bounding object
	//	static const ParticleBoundingObject DEFAULT_BOUNDING_OBJECT;
	//};

	//struct ParticlePhysics {
	//	ImVec2 p_gravity = ImVec2(0.0f, 98.1f); // pixels per second squared
	//	float p_mass = 1.0f; // mass of the particle
	//	float p_airResistance = 0.1f; // 0 = no air resistance, 1 = full stop
	//	static const ParticlePhysics DEFAULT_PHYSICS;
	//};

	//struct ParticleState {
	//	bool p_active = true;
	//	bool p_visible = true;
	//	static const ParticleState DEFAULT_STATE;
	//};

	//struct ParticleDelegate {
	//	bool p_respectSteps = true; // whether to respect fixed time steps in updates
	//	void (*p_onExpire)(void* particlePtr) = nullptr; // callback for when particle expires
	//	void (*p_onCollide)(void* particlePtr, void* otherParticle, const ImVec2& collisionPoint) = nullptr; // callback for when particle collides
	//	void (*p_onUpdate)(void* particlePtr, float ts) = nullptr; // custom update callback
	//	static const ParticleDelegate DEFAULT_DELEGATE;
	//};



	//struct ParticleUV {
	//	ImVec2 p_uv0 = ImVec2(0.0f, 0.0f);
	//	ImVec2 p_uv1 = ImVec2(1.0f, 1.0f);
	// 
	// static const ParticleUV DEFAULT_UV;
	//};

	//struct ParticleTexture {
	//	/// TODO: file path for texture / Walnut image
	//	ImVec2 p_textureSize = ImVec2(0.0f, 0.0f);
	// 
	// static const ParticleTexture DEFAULT_TEXTURE;
	//};
//}