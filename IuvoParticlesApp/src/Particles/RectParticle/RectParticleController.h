#pragma once
#include "RectParticle.h"
#include "../ParticleConfig.h"

namespace Particles {

	class RectParticleController {
	public:
		RectParticleController(RectParticle& p) : particle(p) {}

		// single-frame update. Mirrors old UpdateParticle semantics.
		void Update(float ts, const ImVec2& view_size, bool useDefaultParticle = false, const RectParticle* defaultParticle = nullptr) {
			if (IsExpired()) {
				Reset(view_size, useDefaultParticle, defaultParticle);
				return;
			}

			// bounds check — account for size and rotating particles
			bool rotated = (particle.rp_animation.canRotate && particle.rp_transform.p_rotation.x != 0.0f);


			if (rotated ? HitViewportBoundsRotated(view_size)
				: HitViewportBounds(view_size))
			{
				ApplyReboundForce(view_size);
				return;
			}

			if (particle.rp_animation.canMove) ApplyVelocity(ts);
			if (particle.rp_animation.canRotate) ApplyRotation(ts);
			if (particle.rp_animation.canScale) ApplyScale(ts);

			UpdateLifetime(ts);
			UpdateColor();
			UpdateCenter();
		}

		// Expose smaller operations so external code can call granular steps (keeps public for users)
		void ApplyVelocity(float ts) {
			particle.rp_transform.p_position.x += particle.rp_animation.p_velocity.x * ts * particle.rp_animation.p_movementSpeed;
			particle.rp_transform.p_position.y += particle.rp_animation.p_velocity.y * ts * particle.rp_animation.p_movementSpeed;
		}

		void ApplyRotation(float ts) {
			particle.rp_transform.p_rotation.x += particle.rp_animation.p_rotationSpeed * ts;
			NormalizeRotation();
		}

		void ApplyScale(float ts) {
			const auto cfg = GetParticleConfig();
			float scaleOscillation = cfg.scaleSpeedDefault;
			particle.rp_transform.p_size.x = particle.rp_transform.p_baseSize.x * scaleOscillation;
			particle.rp_transform.p_size.y = particle.rp_transform.p_baseSize.y * scaleOscillation;
		}

		void UpdateLifetime(float ts) {
			particle.rp_lifetimeData.p_lifetime += ts;
		}

		void UpdateColor() {
			if (particle.rp_colorData.p_lerpColor) {
				LerpColor();
			}
			else {
				FadeColor();
			}
		}

		bool IsExpired() const {
			return particle.rp_lifetimeData.p_lifetime >= particle.rp_lifetimeData.p_maxLifetime;
		}

		void Reset(const ImVec2& view_size, bool useDefaultParticle = false, const RectParticle* defaultParticle = nullptr) {
			if (useDefaultParticle && defaultParticle) {
				particle = CreateFromTemplate(*defaultParticle);
				particle.rp_lifetimeData.p_lifetime = 0.0f;
				return;
			}

			// Randomized reset
			particle = CreateRandomInternal(view_size, nullptr);
			if (!particle.rp_colorData.p_lerpColor) {
				particle.rp_colorData.p_currColor = ImVec4(RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), 1.0f);
			}
			else {
				particle.rp_colorData.p_startColor = ImVec4(RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), 1.0f);
				particle.rp_colorData.p_endColor = ImVec4(RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), 1.0f);
			}
			particle.rp_lifetimeData.p_lifetime = 0.0f;
		}

		// compute rotated rectangle corners
		static void CalculateRotatedRectCorners(const RectParticle& particle, ImVec2 outCorners[4]) {
			ImVec2 center = ImVec2(particle.rp_transform.p_position.x + particle.rp_transform.p_size.x * 0.5f,
				particle.rp_transform.p_position.y + particle.rp_transform.p_size.y * 0.5f);
			float hw = particle.rp_transform.p_size.x * 0.5f;
			float hh = particle.rp_transform.p_size.y * 0.5f;
			ImVec2 local[4] = {
				ImVec2(-hw, -hh),
				ImVec2(hw, -hh),
				ImVec2(hw, hh),
				ImVec2(-hw, hh)
			};
			float rad = particle.rp_transform.p_rotation.x * (3.14159265358979323846f / 180.0f);
			float c = cosf(rad);
			float s = sinf(rad);
			for (int i = 0; i < 4; ++i) {
				outCorners[i].x = center.x + local[i].x * c - local[i].y * s;
				outCorners[i].y = center.y + local[i].x * s + local[i].y * c;
			}
		}

		ImU32 FadeColor() {
			float lifeRatio = particle.rp_lifetimeData.p_lifetime / std::max(1e-6f, particle.rp_lifetimeData.p_maxLifetime);
			if (particle.rp_colorData.p_useAlpha) {
				particle.rp_colorData.p_currColor.w = 1.0f - lifeRatio;
			}
			return ImGui::ColorConvertFloat4ToU32(particle.rp_colorData.p_currColor);
		}

		ImU32 LerpColor() {
			float lifeRatio = particle.rp_lifetimeData.p_lifetime / std::max(1e-6f, particle.rp_lifetimeData.p_maxLifetime);
			particle.rp_colorData.p_currColor = ImVec4(
				particle.rp_colorData.p_startColor.x + (particle.rp_colorData.p_endColor.x - particle.rp_colorData.p_startColor.x) * lifeRatio,
				particle.rp_colorData.p_startColor.y + (particle.rp_colorData.p_endColor.y - particle.rp_colorData.p_startColor.y) * lifeRatio,
				particle.rp_colorData.p_startColor.z + (particle.rp_colorData.p_endColor.z - particle.rp_colorData.p_startColor.z) * lifeRatio,
				particle.rp_colorData.p_useAlpha ? (particle.rp_colorData.p_startColor.w + (particle.rp_colorData.p_endColor.w - particle.rp_colorData.p_startColor.w) * lifeRatio) : 1.0f
			);
			return ImGui::ColorConvertFloat4ToU32(particle.rp_colorData.p_currColor);
		}

		// Hit test viewport
		bool HitViewportBounds(const ImVec2& view_size) const {
			return (particle.rp_transform.p_position.x < 0.0f ||
				particle.rp_transform.p_position.x + particle.rp_transform.p_size.x > view_size.x ||
				particle.rp_transform.p_position.y < 0.0f ||
				particle.rp_transform.p_position.y + particle.rp_transform.p_size.y > view_size.y);
		}

		bool HitViewportBoundsRotated(const ImVec2& view_size) const
		{
			ImVec2 corners[4];
			CalculateRotatedRectCorners(particle, corners);

			for (int i = 0; i < 4; i++) {
				if (corners[i].x < 0.0f || corners[i].x > view_size.x ||
					corners[i].y < 0.0f || corners[i].y > view_size.y)
				{
					return true;
				}
			}
			return false;
		}

		// rebound and keep center consistent
		bool ApplyReboundForce(const ImVec2& view_size) {

			bool rotated = (particle.rp_animation.canRotate && particle.rp_transform.p_rotation.x != 0.0f);
			bool rebounded = false;

			// non-rotating particles
			if (!rotated) {
				if (particle.rp_transform.p_position.x < 0.0f) {
					particle.rp_transform.p_position.x = 0.0f;
					particle.rp_animation.p_velocity.x = -particle.rp_animation.p_velocity.x;
					rebounded = true;
				}
				else if (particle.rp_transform.p_position.x + particle.rp_transform.p_size.x > view_size.x) {
					particle.rp_transform.p_position.x = view_size.x - particle.rp_transform.p_size.x;
					particle.rp_animation.p_velocity.x = -particle.rp_animation.p_velocity.x;
					rebounded = true;
				}
				if (particle.rp_transform.p_position.y < 0.0f) {
					particle.rp_transform.p_position.y = 0.0f;
					particle.rp_animation.p_velocity.y = -particle.rp_animation.p_velocity.y;
					rebounded = true;
				}
				else if (particle.rp_transform.p_position.y + particle.rp_transform.p_size.y > view_size.y) {
					particle.rp_transform.p_position.y = view_size.y - particle.rp_transform.p_size.y;
					particle.rp_animation.p_velocity.y = -particle.rp_animation.p_velocity.y;
					rebounded = true;
				}
				return rebounded;
			}
			// rotating particles
			else {
				ImVec2 corners[4];
				CalculateRotatedRectCorners(particle, corners);

				float minX = FLT_MAX, minY = FLT_MAX;
				float maxX = -FLT_MAX, maxY = -FLT_MAX;

				for (int i = 0; i < 4; i++) {
					minX = std::min(minX, corners[i].x);
					minY = std::min(minY, corners[i].y);
					maxX = std::max(maxX, corners[i].x);
					maxY = std::max(maxY, corners[i].y);
				}

				bool rebounded = false;

				// Left
				if (minX < 0.0f) {
					float shift = -minX;
					particle.rp_transform.p_position.x += shift;
					particle.rp_animation.p_velocity.x *= -1.0f;
					rebounded = true;
				}
				// Right
				else if (maxX > view_size.x) {
					float shift = view_size.x - maxX;
					particle.rp_transform.p_position.x += shift;
					particle.rp_animation.p_velocity.x *= -1.0f;
					rebounded = true;
				}

				// Top
				if (minY < 0.0f) {
					float shift = -minY;
					particle.rp_transform.p_position.y += shift;
					particle.rp_animation.p_velocity.y *= -1.0f;
					rebounded = true;
				}
				// Bottom
				else if (maxY > view_size.y) {
					float shift = view_size.y - maxY;
					particle.rp_transform.p_position.y += shift;
					particle.rp_animation.p_velocity.y *= -1.0f;
					rebounded = true;
				}
				return rebounded;
			}

		}

		// helpers
		void UpdateCenter() {
			particle.rp_transform.p_center = ImVec2(particle.rp_transform.p_position.x + particle.rp_transform.p_size.x * 0.5f,
				particle.rp_transform.p_position.y + particle.rp_transform.p_size.y * 0.5f);
		}

		void NormalizeRotation() {
			particle.rp_transform.p_rotation.x = fmodf(particle.rp_transform.p_rotation.x, 360.0f);
			if (particle.rp_transform.p_rotation.x < 0) particle.rp_transform.p_rotation.x += 360.0f;
		}

		void SetCenter(const ImVec2& newCenter) {
			particle.rp_transform.p_center = newCenter;
			particle.rp_transform.p_position = ImVec2(
				newCenter.x - particle.rp_transform.p_size.x * 0.5f,
				newCenter.y - particle.rp_transform.p_size.y * 0.5f
			);
		}

		// Static convenience creators that the controller uses internally
		static RectParticle CreateFromTemplate(const RectParticle& t) {
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

		static RectParticle CreateRandomInternal(const ImVec2& view_size, const RectParticle* templateParticle) {
			RectParticle particle;
			const auto& cfg = GetParticleConfig();
			if (templateParticle) {
				particle.rp_transform.p_randomSize = templateParticle->rp_transform.p_randomSize;
				particle.rp_transform.p_randomRotation = templateParticle->rp_transform.p_randomRotation;
				particle.rp_transform.p_randomPosition = templateParticle->rp_transform.p_randomPosition;
				particle.rp_transform.p_baseSize = templateParticle->rp_transform.p_baseSize;
				particle.rp_animation = templateParticle->rp_animation;
				particle.rp_colorData = templateParticle->rp_colorData;
				particle.rp_lifetimeData = templateParticle->rp_lifetimeData;
			}
			else
			{
				// use the config defaults
			}


			// position: anywhere inside view
			if (particle.rp_transform.p_randomPosition) {
				particle.rp_transform.p_position = ImVec2(RandFloat(0.0f, view_size.x), RandFloat(0.0f, view_size.y));
			}
			else
			{
				particle.rp_transform.p_position = ImVec2(view_size.x * 0.5f, view_size.y * 0.5f);
			}

			// random size
			if (particle.rp_transform.p_randomSize) {
				float sx = RandFloat(cfg.randomSizeMin.x, cfg.randomSizeMax.x);
				float sy = RandFloat(cfg.randomSizeMin.y, cfg.randomSizeMax.y);
				particle.rp_transform.p_baseSize = ImVec2(sx, sy);
				particle.rp_transform.p_size = particle.rp_transform.p_baseSize;
			}
			else {
				if (particle.rp_transform.p_baseSize.x <= 0.0f || particle.rp_transform.p_baseSize.y <= 0.0f) {
					particle.rp_transform.p_baseSize = cfg.defaultBaseSize;
					particle.rp_transform.p_size = particle.rp_transform.p_baseSize;
				}
			}

			// rotation
			if (particle.rp_transform.p_randomRotation) {
				particle.rp_transform.p_rotation.x = RandFloat(cfg.rotationMin, cfg.rotationMax);
			}
			else {
				particle.rp_transform.p_rotation.x = 0.0f;
			}

			// center
			particle.rp_transform.p_center = ImVec2(
				particle.rp_transform.p_position.x + particle.rp_transform.p_size.x * 0.5f,
				particle.rp_transform.p_position.y + particle.rp_transform.p_size.y * 0.5f
			);

			// velocity direction
			float angle = RandFloat(0.0f, 2.0f * 3.14159265358979323846f);
			particle.rp_animation.p_velocity = ImVec2(cosf(angle), sinf(angle));
			particle.rp_animation.p_movementSpeed = RandFloat(cfg.movementSpeedMin, cfg.movementSpeedMax);
			particle.rp_animation.p_rotationSpeed = RandFloat(cfg.rotationSpeedMin, cfg.rotationSpeedMax);

			// lifetime
			particle.rp_lifetimeData.p_lifetime = 0.0f;
			// if the randomize lifetime flag is false but the max lifetime is too small, randomize it
			if (!particle.rp_lifetimeData.p_randomizeLifetime && particle.rp_lifetimeData.p_maxLifetime <= 0.0f) {
				particle.rp_lifetimeData.p_maxLifetime = RandFloat(cfg.lifetimeMin, cfg.lifetimeMax);
			}
			else if (particle.rp_lifetimeData.p_randomizeLifetime) {
				particle.rp_lifetimeData.p_maxLifetime = RandFloat(cfg.lifetimeRandomMin, cfg.lifetimeRandomMax);
			}

			// color
			if (particle.rp_colorData.p_randomizeColor) {
				particle.rp_colorData.p_currColor = ImVec4(RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), 1.0f);
			}
			else {
				if (particle.rp_colorData.p_lerpColor) {
					particle.rp_colorData.p_startColor = ImVec4(RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), particle.rp_colorData.p_startColor.w);
					particle.rp_colorData.p_endColor = ImVec4(RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), particle.rp_colorData.p_endColor.w);
				}
				else {
					//particle.rp_colorData.p_currColor = ImVec4(RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), 1.0f);
					particle.rp_colorData.p_currColor = particle.rp_colorData.p_startColor;
				}
			}


			// normalize rotation
			particle.rp_transform.p_rotation.x = fmodf(particle.rp_transform.p_rotation.x, 360.0f);
			if (particle.rp_transform.p_rotation.x < 0.0f)
				particle.rp_transform.p_rotation.x += 360.0f;

			return particle;
		}
	private:
		RectParticle& particle;
	};

}