#pragma once 
#include "RectParticleRotationController.h"

namespace Particles {
	void RectParticleRotationController::ApplyRotation(RectParticle& particle, float ts) {
		particle.rp_transform.p_rotation.x += particle.rp_animation.p_rotationSpeed * ts;
		NormalizeRotation(particle);
	}

	void RectParticleRotationController::NormalizeRotation(RectParticle& particle) {
		particle.rp_transform.p_rotation.x = fmodf(particle.rp_transform.p_rotation.x, 360.0f);
		if (particle.rp_transform.p_rotation.x < 0) particle.rp_transform.p_rotation.x += 360.0f;
	}

}