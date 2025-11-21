#pragma once
#include "imgui.h"

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


}