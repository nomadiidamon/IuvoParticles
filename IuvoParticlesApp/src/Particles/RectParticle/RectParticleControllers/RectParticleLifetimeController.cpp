#pragma once
#include "RectParticleLifetimeController.h"
#include "../../ParticleRandom.h"
#include "../../ParticleUtils.h"

namespace Particles {
	void RectParticleLifetimeController::UpdateLifetime(RectParticle& particle, float ts) {
		particle.rp_lifetimeData.p_lifetime += ts;
	}


	bool RectParticleLifetimeController::IsExpired(const RectParticle& particle, float& outLifeRatio) {
		if (particle.rp_lifetimeData.p_lifetime >= particle.rp_lifetimeData.p_maxLifetime) {
			outLifeRatio = 0.0f;
			return true;
		}
		else {
			outLifeRatio = particle.rp_lifetimeData.p_lifetime / std::max(EPS, particle.rp_lifetimeData.p_maxLifetime);
			return false;
		}
	}

}