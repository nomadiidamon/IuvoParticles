#pragma once
#include "RectParticleColorController.h"

namespace Particles {

	void RectParticleColorController::UpdateColor(RectParticle& particle, float& lifetimeRatio) {
		if (particle.rp_colorData.p_lerpColor) {
			LerpColor(particle);
		}
		else {
			FadeColor(particle);
		}
	}

	ImU32 RectParticleColorController::FadeColor(RectParticle& particle) {
		float lifeRatio = particle.rp_lifetimeData.p_lifetime / std::max(1e-6f, particle.rp_lifetimeData.p_maxLifetime);
		if (particle.rp_colorData.p_useAlpha) {
			particle.rp_colorData.p_currColor.w = 1.0f - lifeRatio;
		}
		return ImGui::ColorConvertFloat4ToU32(particle.rp_colorData.p_currColor);
	}

	ImU32 RectParticleColorController::LerpColor(RectParticle& particle) {
		float lifeRatio = particle.rp_lifetimeData.p_lifetime / std::max(1e-6f, particle.rp_lifetimeData.p_maxLifetime);
		particle.rp_colorData.p_currColor = ImVec4(
			particle.rp_colorData.p_startColor.x + (particle.rp_colorData.p_endColor.x - particle.rp_colorData.p_startColor.x) * lifeRatio,
			particle.rp_colorData.p_startColor.y + (particle.rp_colorData.p_endColor.y - particle.rp_colorData.p_startColor.y) * lifeRatio,
			particle.rp_colorData.p_startColor.z + (particle.rp_colorData.p_endColor.z - particle.rp_colorData.p_startColor.z) * lifeRatio,
			particle.rp_colorData.p_useAlpha ? (particle.rp_colorData.p_startColor.w + (particle.rp_colorData.p_endColor.w - particle.rp_colorData.p_startColor.w) * lifeRatio) : 1.0f
		);
		return ImGui::ColorConvertFloat4ToU32(particle.rp_colorData.p_currColor);
	}
}