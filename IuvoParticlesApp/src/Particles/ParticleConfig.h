#pragma once
#include "RectParticle/RectParticle.h"
#include "ParticleUtils.h"
#include "ParticleTypes.h"

namespace Particles {

	struct ParticleConfig {

		ParticleTransformRange transform;
		bool defaultRandomPosition = false;
		bool defaultRandomSize = false;
		bool defaultRandomRotation = false;

		ParticleAnimation2DRange animation;
		bool defaultCanMove = true;
		bool defaultCanRotate = true;
		bool defaultCanScale = false;

		ParticleLifetimeRange lifetime;
		bool defaultCanLoop = false;
		bool defaultRandomizeLifetime = false;

		ParticleColorRange color;
		bool defaultLerpColor = true;
		bool defaultFadeColor = true;
		bool defaultUseAlpha = true;
		bool defaultRandomizeColor = false;

		/// ============================
		/// SYSTEM / EMITTER DEFAULTS
		/// ============================
		int   defaultMaxParticles = 200;
		float defaultEmissionRate = 10.0f;  // particles/sec
		ImVec2 defaultViewSize = ImVec2(800.0f, 600.0f);

		//RectParticle defaultEmitterTemplate;
		//RectParticle defaultSpawnTemplate;

		void Initialize() {

		}
	};

    // return a global config instance (modifiable at runtime)
    inline ParticleConfig& GetParticleConfig() {
        static ParticleConfig cfg;
        static bool initialized = false;
        if (!initialized) {
			cfg.Initialize();
            initialized = true;
        }
        return cfg;
    }
}