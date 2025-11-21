#pragma once
#include "../ParticleConfig.h"
#include "RectParticleControllers/RectParticleSubControllers.h"
#include "RectParticleSpawner.h"

namespace Particles {

	class RectParticleController {
	public:
		RectParticleController(RectParticle& p) : particle(p) {}

		void Update(float ts, const ImVec2& view_size, std::vector<RectParticle>& container, bool useDefaultParticle = false, const RectParticle* defaultParticle = nullptr) {
			float lifetimeRatio = 0.0f;
			if (RectParticleLifetimeController::IsExpired(particle, lifetimeRatio)) {
				Reset(view_size, container, useDefaultParticle, defaultParticle);
				return;
			}

			bool rotated = (particle.animation.canRotate && particle.transform.rotation.x != 0.0f);
			if (rotated ? RectParticleMotionController::HitBoundsRotated(particle, view_size)
				: RectParticleMotionController::HitBounds(particle, view_size))
			{
				RectParticleMotionController::ApplyRebound(particle, view_size);
				return;
			}

			if (particle.animation.canMove) RectParticleMotionController::ApplyVelocity(particle, ts);
			if (particle.animation.canRotate) RectParticleRotationController::ApplyRotation(particle, ts);
			if (particle.animation.canScale) RectParticleScaleController::ApplyScale(particle,ts);
			RectParticleLifetimeController::UpdateLifetime(particle, ts);
			RectParticleColorController::UpdateColor(particle, lifetimeRatio);
			UpdateCenter();
		}

		void Reset(const ImVec2& view_size, std::vector<RectParticle>& container, bool useDefaultParticle = false, const RectParticle* defaultParticle = nullptr) {
			// remove from container
			auto it = std::find_if(container.begin(), container.end(),
				[this](const RectParticle& p) { return &p == &particle; });
			if (it != container.end()) {
				container.erase(it);
			}

			if (useDefaultParticle && defaultParticle) {
				particle = RectParticleSpawner::FromTemplate(*defaultParticle);
				particle.lifetime.lifetime = 0.0f;
				container.push_back(particle);
				return;
			}
			particle = RectParticleSpawner::CreateRandom(view_size);
			container.push_back(particle);
		}

		void UpdateCenter() {
			particle.transform.center = ImVec2(particle.transform.position.x + particle.transform.size.x * 0.5f,
				particle.transform.position.y + particle.transform.size.y * 0.5f);
		}

		void SetCenter(const ImVec2& newCenter) {
			particle.transform.center = newCenter;
			particle.transform.position = ImVec2(
				newCenter.x - particle.transform.size.x * 0.5f,
				newCenter.y - particle.transform.size.y * 0.5f
			);
		}

private:
		RectParticle& particle;
	};

}