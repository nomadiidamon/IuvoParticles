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
}