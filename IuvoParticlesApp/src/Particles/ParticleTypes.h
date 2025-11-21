#pragma once
#include "ParticleColor.h"
#include "ParticleTransform.h"
#include "ParticleAnimation2D.h"
#include "ParticleLifetime.h"

namespace Particles {

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


}

