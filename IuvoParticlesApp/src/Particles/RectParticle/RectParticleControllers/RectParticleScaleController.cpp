#pragma once
#include "RectParticleScaleController.h"

namespace Particles {
	void RectParticleScaleController::ApplyScale(RectParticle& particle, float ts) {
		ParticleConfig& cfg = GetParticleConfig();
		float scaleOscillation = cfg.animation.defaultScaleSpeed;
		particle.rp_transform.p_size.x = particle.rp_transform.p_baseSize.x * scaleOscillation;
		particle.rp_transform.p_size.y = particle.rp_transform.p_baseSize.y * scaleOscillation;
	}
}