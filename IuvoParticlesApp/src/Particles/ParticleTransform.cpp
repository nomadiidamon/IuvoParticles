#pragma once
#include "ParticleTransform.h"
#include "ParticleUtils.h"

namespace Particles {
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

	bool ParticleTransform::IsEqual(const ParticleTransform& a, const ParticleTransform& b) {
		return FloatEquals(a.position.x, b.position.x) &&
			FloatEquals(a.position.y, b.position.y) &&
			FloatEquals(a.size.x, b.size.x) &&
			FloatEquals(a.size.y, b.size.y) &&
			FloatEquals(a.rotation.x, b.rotation.x) &&
			FloatEquals(a.rotation.y, b.rotation.y) &&
			(a.randomSize == b.randomSize) &&
			(a.randomRotation == b.randomRotation) &&
			(a.randomPosition == b.randomPosition);
	}

	nlohmann::json ParticleTransformSerializer::Serialize(const ParticleTransform& transform) {
		nlohmann::json j;
		j["position"] = { transform.position.x, transform.position.y };
		j["size"] = { transform.size.x, transform.size.y };
		j["rotation"] = { transform.rotation.x, transform.rotation.y };
		j["randomSize"] = transform.randomSize;
		j["randomRotation"] = transform.randomRotation;
		j["randomPosition"] = transform.randomPosition;
		return j;
	}
	ParticleTransform ParticleTransformSerializer::Deserialize(const nlohmann::json& j) {
		ParticleTransform transform;
		transform.position = ImVec2(j["position"][0], j["position"][1]);
		transform.size = ImVec2(j["size"][0], j["size"][1]);
		transform.baseSize = transform.size;
		transform.rotation = ImVec2(j["rotation"][0], j["rotation"][1]);
		transform.randomSize = j["randomSize"];
		transform.randomRotation = j["randomRotation"];
		transform.randomPosition = j["randomPosition"];
		return transform;
	}
}