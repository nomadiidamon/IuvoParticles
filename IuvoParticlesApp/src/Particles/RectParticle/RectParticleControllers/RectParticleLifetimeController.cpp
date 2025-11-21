#pragma once
#include "RectParticleLifetimeController.h"
#include "../../ParticleRandom.h"
#include "../../ParticleUtils.h"

namespace Particles {
	void RectParticleLifetimeController::UpdateLifetime(RectParticle& particle, float ts) {
		particle.lifetime.lifetime += ts;
	}


	bool RectParticleLifetimeController::IsExpired(const RectParticle& particle, float& outLifeRatio) {
		if (particle.lifetime.lifetime >= particle.lifetime.maxLifetime) {
			outLifeRatio = 0.0f;
			return true;
		}
		else {
			outLifeRatio = particle.lifetime.lifetime / std::max(EPS, particle.lifetime.maxLifetime);
			return false;
		}
	}

}