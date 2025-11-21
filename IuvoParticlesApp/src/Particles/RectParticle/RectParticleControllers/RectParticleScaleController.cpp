#pragma once
#include "RectParticleScaleController.h"

namespace Particles {
	void RectParticleScaleController::ApplyScale(RectParticle& particle, float ts) {
		ParticleConfig& cfg = GetParticleConfig();
		float scaleOscillation = cfg.animation.defaultScaleSpeed;
		particle.transform.size.x = particle.transform.baseSize.x * scaleOscillation;
		particle.transform.size.y = particle.transform.baseSize.y * scaleOscillation;
	}
}