#pragma once
#include "imgui.h"
#include <vector>

namespace Particles {

	enum class ParticleColorPreset {
		NONE,
		RED_TO_YELLOW_FADE_OUT,
		RED_TO_BLUE_FADE_OUT,
		RED_TO_TRANSPARENT_FADE_OUT,
		ORANGE_TO_YELLOW_FADE_OUT,
		ORANGE_TO_TRANSPARENT_FADE_OUT,
		YELLOW_TO_GREEN_FADE_OUT,
		YELLOW_TO_TRANSPARENT_FADE_OUT,
		BLUE_TO_CYAN_FADE_OUT,
		BLUE_TO_TRANSPARENT_FADE_OUT,
		FIRE_FADE_OUT,
		ICE_FADE_OUT,
		TOXIC_FADE_OUT,
		MAGIC_PURPLE_FADE_OUT
	};
	struct ParticleColor {
		ImVec4 p_startColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		ImVec4 p_endColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
		ImVec4 p_currColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		float  p_lerpSpeed = 0.25f;

		bool p_lerpColor = true;
		bool p_fadeColor = true;
		bool p_useAlpha = true;
		bool p_randomizeColor = false;

		static const ParticleColor DEFAULT_COLOR;

		static const ParticleColor RED_TO_YELLOW_FADE_OUT;
		static const ParticleColor RED_TO_BLUE_FADE_OUT;
		static const ParticleColor RED_TO_TRANSPARENT_FADE_OUT;
		static const ParticleColor ORANGE_TO_YELLOW_FADE_OUT;
		static const ParticleColor ORANGE_TO_TRANSPARENT_FADE_OUT;
		static const ParticleColor YELLOW_TO_GREEN_FADE_OUT;
		static const ParticleColor YELLOW_TO_TRANSPARENT_FADE_OUT;
		static const ParticleColor BLUE_TO_CYAN_FADE_OUT;
		static const ParticleColor BLUE_TO_TRANSPARENT_FADE_OUT;
		static const ParticleColor FIRE_FADE_OUT;
		static const ParticleColor ICE_FADE_OUT;
		static const ParticleColor TOXIC_FADE_OUT;
		static const ParticleColor MAGIC_PURPLE_FADE_OUT;

		static ParticleColor CreateRandomColor();
		static ParticleColor CreateRandomColorInRange(const ImVec4& minColor, const ImVec4& maxColor);
	};
	struct ParticleColorRange {
		bool enforceConstraints = false;

		ImVec4 startColorMin = ImVec4(0.1f, 0.1f, 0.1f, 0.1f);
		ImVec4 startColorMax = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImVec4 defaultStartColor = ParticleColor::ICE_FADE_OUT.p_startColor;

		ImVec4 endColorMin = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		ImVec4 endColorMax = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		ImVec4 defaultEndColor = ParticleColor::TOXIC_FADE_OUT.p_startColor;
		ImVec4 defaultCurrColor = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);


		float colorFadeSpeedMin = 0.01f;
		float colorFadeSpeedMax = 2.0f;
		float defaultColorFadeSpeed = 0.25f;
		static const ParticleColorRange DEFAULT_COLOR_RANGE;
	};

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

	enum class ParticleTransformPreset {
		NONE,
		SMALL_FLOATING,
		EXPLOSION_CHUNK
	};
	struct ParticleTransform {
		ImVec2 p_position = ImVec2(0.0f, 0.0f);
		ImVec2 p_center = ImVec2(0.0f, 0.0f);

		ImVec2 p_size = ImVec2(10.0f, 10.0f);
		ImVec2 p_baseSize = ImVec2(10.0f, 10.0f); // store base size to compute non-destructive scaling each frame
		ImVec2 p_rotation = ImVec2(67.0f, 0.0f); // using x as angle in degrees for 2D rotation

		/// TODO: reevaluate these randomization flags
		bool p_randomPosition = false;
		bool p_randomSize = false;
		bool p_randomRotation = false;

		static const ParticleTransform DEFAULT_TRANSFORM;
		static ParticleTransform CreateRandomTransform();
		static const ParticleTransform SMALL_FLOATING;
		static const ParticleTransform EXPLOSION_CHUNK;

	};
	struct ParticleTransformRange {
		bool enforceConstraints = false;

		ImVec2 minPosition = ImVec2(0.0f, 0.0f);
		ImVec2 maxPosition = ImVec2(100.0f, 100.0f);
		ImVec2 randomPositionMin = ImVec2(0.0f, 0.0f);		
		ImVec2 randomPositionMax = ImVec2(100.0f, 100.0f);  
		ImVec2 defaultPosition = ImVec2(150.0f, 150.0f);

		ImVec2 minSize = ImVec2(5.0f, 5.0f);
		ImVec2 maxSize = ImVec2(20.0f, 20.0f);
		ImVec2 randomSizeMin = ImVec2(5.0f, 5.0f);
		ImVec2 randomSizeMax = ImVec2(20.0f, 20.0f);
		ImVec2 defaultSize = ImVec2(10.0f, 10.0f);

		ImVec2 minRotation = ImVec2(0.0f, 0.0f);
		ImVec2 maxRotation = ImVec2(0.0f, 0.0f);
		ImVec2 randomRotationMin = ImVec2(0.0f, 0.0f);
		ImVec2 randomRotationMax = ImVec2(360.0f, 0.0f);
		ImVec2 defaultRotation = ImVec2(0.0f, 0.0f);

		static const ParticleTransformRange DEFAULT_TRANSFORM_RANGE;
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