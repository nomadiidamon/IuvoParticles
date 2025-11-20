#pragma once
#include "RectParticleSpawner.h"

namespace Particles {

	RectParticle RectParticleSpawner::FromTemplate(const RectParticle& t) {
		RectParticle out = t;
		out.rp_transform.p_center = ImVec2(out.rp_transform.p_position.x + out.rp_transform.p_size.x * 0.5f,
			out.rp_transform.p_position.y + out.rp_transform.p_size.y * 0.5f);
		if (out.rp_transform.p_baseSize.x <= 0.0f || out.rp_transform.p_baseSize.y <= 0.0f)
			out.rp_transform.p_baseSize = out.rp_transform.p_size;

		// normalize rotation
		out.rp_transform.p_rotation.x = fmodf(out.rp_transform.p_rotation.x, 360.0f);
		if (out.rp_transform.p_rotation.x < 0.0f)
			out.rp_transform.p_rotation.x += 360.0f;
		return out;
	}

	RectParticle RectParticleSpawner::CreateRandom(const ImVec2& view_size) {
		RectParticle particle;
		//auto& cfg = GetParticleConfig();
		//if (templateParticle) {
		//	particle.rp_transform.p_randomSize = templateParticle->rp_transform.p_randomSize;
		//	particle.rp_transform.p_randomRotation = templateParticle->rp_transform.p_randomRotation;
		//	particle.rp_transform.p_randomPosition = templateParticle->rp_transform.p_randomPosition;
		//	particle.rp_transform.p_baseSize = templateParticle->rp_transform.p_baseSize;
		//	particle.rp_animation = templateParticle->rp_animation;
		//	particle.rp_colorData = templateParticle->rp_colorData;
		//	particle.rp_lifetimeData = templateParticle->rp_lifetimeData;
		//}
		//else
		//{
			// use the config defaults
			particle = RectParticle::RANDOM_PARTICLE();
		//}


		//// position: anywhere inside view
		//if (particle.rp_transform.p_randomPosition) {
		//	particle.rp_transform.p_position = ImVec2(RandFloat(0.0f, view_size.x), RandFloat(0.0f, view_size.y));
		//}
		//else
		//{
		//	particle.rp_transform.p_position = ImVec2(view_size.x * 0.5f, view_size.y * 0.5f);
		//}

		//// random size
		//if (particle.rp_transform.p_randomSize) {
		//	float sx = RandFloat(cfg.randomSizeMin.x, cfg.randomSizeMax.x);
		//	float sy = RandFloat(cfg.randomSizeMin.y, cfg.randomSizeMax.y);
		//	particle.rp_transform.p_baseSize = ImVec2(sx, sy);
		//	particle.rp_transform.p_size = particle.rp_transform.p_baseSize;
		//}
		//else {
		//	if (particle.rp_transform.p_baseSize.x <= 0.0f || particle.rp_transform.p_baseSize.y <= 0.0f) {
		//		particle.rp_transform.p_baseSize = cfg.defaultBaseSize;
		//		particle.rp_transform.p_size = particle.rp_transform.p_baseSize;
		//	}
		//}

		//// rotation
		//if (particle.rp_transform.p_randomRotation) {
		//	particle.rp_transform.p_rotation.x = RandFloat(cfg.rotationMin, cfg.rotationMax);
		//}
		//else {
		//	particle.rp_transform.p_rotation.x = 0.0f;
		//}

		//// center
		//particle.rp_transform.p_center = ImVec2(
		//	particle.rp_transform.p_position.x + particle.rp_transform.p_size.x * 0.5f,
		//	particle.rp_transform.p_position.y + particle.rp_transform.p_size.y * 0.5f
		//);

		//// velocity direction
		//float angle = RandFloat(0.0f, 2.0f * 3.14159265358979323846f);
		//particle.rp_animation.p_velocity = ImVec2(cosf(angle), sinf(angle));
		//particle.rp_animation.p_movementSpeed = RandFloat(cfg.movementSpeedMin, cfg.movementSpeedMax);
		//particle.rp_animation.p_rotationSpeed = RandFloat(cfg.rotationSpeedMin, cfg.rotationSpeedMax);

		//// lifetime
		//particle.rp_lifetimeData.p_lifetime = 0.0f;
		//// if the randomize lifetime flag is false but the max lifetime is too small, randomize it
		//if (!particle.rp_lifetimeData.p_randomizeLifetime && particle.rp_lifetimeData.p_maxLifetime <= 0.0f) {
		//	particle.rp_lifetimeData.p_maxLifetime = RandFloat(cfg.lifetimeMin, cfg.lifetimeMax);
		//}
		//else if (particle.rp_lifetimeData.p_randomizeLifetime) {
		//	particle.rp_lifetimeData.p_maxLifetime = RandFloat(cfg.lifetimeRandomMin, cfg.lifetimeRandomMax);
		//}

		//// color
		//if (particle.rp_colorData.p_randomizeColor) {
		//	particle.rp_colorData.p_currColor = ImVec4(RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), 1.0f);
		//}
		//else {
		//	if (particle.rp_colorData.p_lerpColor) {
		//		particle.rp_colorData.p_startColor = ImVec4(RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), particle.rp_colorData.p_startColor.w);
		//		particle.rp_colorData.p_endColor = ImVec4(RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), particle.rp_colorData.p_endColor.w);
		//	}
		//	else {
		//		//particle.rp_colorData.p_currColor = ImVec4(RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), 1.0f);
		//		particle.rp_colorData.p_currColor = particle.rp_colorData.p_startColor;
		//	}
		//}


		//// normalize rotation
		//particle.rp_transform.p_rotation.x = fmodf(particle.rp_transform.p_rotation.x, 360.0f);
		//if (particle.rp_transform.p_rotation.x < 0.0f)
		//	particle.rp_transform.p_rotation.x += 360.0f;

		return particle;
	}

}
