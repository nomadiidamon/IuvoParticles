#pragma once
#include "ParticleUtils.h"
#include "ParticleLifetime.h"

namespace Particles {
	
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


}