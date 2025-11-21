#pragma once 
#include "RectParticleRotationController.h"

namespace Particles {
	void RectParticleRotationController::ApplyRotation(RectParticle& particle, float ts) {
		particle.transform.rotation.x += particle.animation.p_rotationSpeed * ts;
		NormalizeRotation(particle);
	}

	void RectParticleRotationController::NormalizeRotation(RectParticle& particle) {
		particle.transform.rotation.x = fmodf(particle.transform.rotation.x, 360.0f);
		if (particle.transform.rotation.x < 0) particle.transform.rotation.x += 360.0f;
	}

}