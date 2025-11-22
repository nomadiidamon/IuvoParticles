#pragma once
#include "ParticleUtils.h"
#include "ParticleLifetime.h"

namespace Particles {
	
#pragma Region Lifetime Definitions
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
	bool ParticleLifetime::IsEqual(const ParticleLifetime& a, const ParticleLifetime& b) {
		return FloatEquals(a.lifetime, b.lifetime) &&
			FloatEquals(a.maxLifetime, b.maxLifetime) &&
			(a.randomizeLifetime == b.randomizeLifetime) &&
			(a.loop == b.loop) &&
			(a.loopCount == b.loopCount);
	}
#pragma endregion

	// Lifetime Range Definition
	const ParticleLifetimeRange DEFAULT_LIFETIME_RANGE = ParticleLifetimeRange{
		false,
		1.0f,
		10.0f,
		1.0f,
		10.0f,
		5.0f,
		0,
		5
	};

	// Serialization
	nlohmann::json ParticleLifetimeSerializer::Serialize(const ParticleLifetime& lifetime) {
		nlohmann::json j;
		j["lifetime"] = lifetime.lifetime;
		j["maxLifetime"] = lifetime.maxLifetime;
		j["randomizeLifetime"] = lifetime.randomizeLifetime;
		j["loop"] = lifetime.loop;
		j["loopCount"] = lifetime.loopCount;
		return j;
	}
	ParticleLifetime ParticleLifetimeSerializer::Deserialize(const nlohmann::json& j) {
		ParticleLifetime lifetime;
		lifetime.lifetime = j.value("lifetime", 0.0f);
		lifetime.maxLifetime = j.value("maxLifetime", 5.0f);
		lifetime.randomizeLifetime = j.value("randomizeLifetime", false);
		lifetime.loop = j.value("loop", false);
		lifetime.loopCount = j.value("loopCount", 0);
		return lifetime;
	}

}