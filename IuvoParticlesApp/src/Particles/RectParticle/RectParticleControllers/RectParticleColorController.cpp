#pragma once
#include "RectParticleColorController.h"

namespace Particles {

	void RectParticleColorController::UpdateColor(RectParticle& particle, float& lifetimeRatio) {
		if (particle.color.lerpColor) {
			LerpColor(particle);
		}
		else {
			FadeColor(particle);
		}
	}

	ImU32 RectParticleColorController::FadeColor(RectParticle& particle) {
		float lifeRatio = particle.lifetime.lifetime / std::max(1e-6f, particle.lifetime.maxLifetime);
		if (particle.color.useAlpha) {
			particle.color.currColor.w = 1.0f - lifeRatio;
		}
		return ImGui::ColorConvertFloat4ToU32(particle.color.currColor);
	}

	ImU32 RectParticleColorController::LerpColor(RectParticle& particle) {
		float lifeRatio = particle.lifetime.lifetime / std::max(1e-6f, particle.lifetime.maxLifetime);
		particle.color.currColor = ImVec4(
			particle.color.startColor.x + (particle.color.endColor.x - particle.color.startColor.x) * lifeRatio,
			particle.color.startColor.y + (particle.color.endColor.y - particle.color.startColor.y) * lifeRatio,
			particle.color.startColor.z + (particle.color.endColor.z - particle.color.startColor.z) * lifeRatio,
			particle.color.useAlpha ? (particle.color.startColor.w + (particle.color.endColor.w - particle.color.startColor.w) * lifeRatio) : 1.0f
		);
		return ImGui::ColorConvertFloat4ToU32(particle.color.currColor);
	}
}