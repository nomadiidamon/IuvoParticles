#pragma once
#include "RectParticleMotionController.h"

namespace Particles {
	void RectParticleMotionController::ApplyVelocity(RectParticle& particle, float ts) {
		particle.transform.position.x += particle.animation.p_velocity.x * ts * particle.animation.p_movementSpeed;
		particle.transform.position.y += particle.animation.p_velocity.y * ts * particle.animation.p_movementSpeed;
	}

	bool RectParticleMotionController::HitBounds(const RectParticle& particle, const ImVec2& view_size) {
		return (particle.transform.position.x < 0.0f ||
			particle.transform.position.x + particle.transform.size.x > view_size.x ||
			particle.transform.position.y < 0.0f ||
			particle.transform.position.y + particle.transform.size.y > view_size.y);
	}

	bool RectParticleMotionController::HitBoundsRotated(const RectParticle& particle, const ImVec2& view_size)
	{
		ImVec2 corners[4];
		ComputeRotatedCorners(particle, corners);

		for (int i = 0; i < 4; i++) {
			if (corners[i].x < 0.0f || corners[i].x > view_size.x ||
				corners[i].y < 0.0f || corners[i].y > view_size.y)
			{
				return true;
			}
		}
		return false;
	}

	bool RectParticleMotionController::ApplyRebound(RectParticle& particle, const ImVec2& view_size) {

		bool rotated = (particle.animation.canRotate && particle.transform.rotation.x != 0.0f);
		bool rebounded = false;

		// non-rotating particles
		if (!rotated) {
			if (particle.transform.position.x < 0.0f) {
				particle.transform.position.x = 0.0f;
				particle.animation.p_velocity.x = -particle.animation.p_velocity.x;
				rebounded = true;
			}
			else if (particle.transform.position.x + particle.transform.size.x > view_size.x) {
				particle.transform.position.x = view_size.x - particle.transform.size.x;
				particle.animation.p_velocity.x = -particle.animation.p_velocity.x;
				rebounded = true;
			}
			if (particle.transform.position.y < 0.0f) {
				particle.transform.position.y = 0.0f;
				particle.animation.p_velocity.y = -particle.animation.p_velocity.y;
				rebounded = true;
			}
			else if (particle.transform.position.y + particle.transform.size.y > view_size.y) {
				particle.transform.position.y = view_size.y - particle.transform.size.y;
				particle.animation.p_velocity.y = -particle.animation.p_velocity.y;
				rebounded = true;
			}
			return rebounded;
		}
		// rotating particles
		else {
			ImVec2 corners[4];
			ComputeRotatedCorners(particle, corners);

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
				particle.transform.position.x += shift;
				particle.animation.p_velocity.x *= -1.0f;
				rebounded = true;
			}
			// Right
			else if (maxX > view_size.x) {
				float shift = view_size.x - maxX;
				particle.transform.position.x += shift;
				particle.animation.p_velocity.x *= -1.0f;
				rebounded = true;
			}

			// Top
			if (minY < 0.0f) {
				float shift = -minY;
				particle.transform.position.y += shift;
				particle.animation.p_velocity.y *= -1.0f;
				rebounded = true;
			}
			// Bottom
			else if (maxY > view_size.y) {
				float shift = view_size.y - maxY;
				particle.transform.position.y += shift;
				particle.animation.p_velocity.y *= -1.0f;
				rebounded = true;
			}
			return rebounded;
		}

	}

	void RectParticleMotionController::ComputeRotatedCorners(const RectParticle& particle, ImVec2 outCorners[4]) {
		ImVec2 center = ImVec2(particle.transform.position.x + particle.transform.size.x * 0.5f,
			particle.transform.position.y + particle.transform.size.y * 0.5f);
		float hw = particle.transform.size.x * 0.5f;
		float hh = particle.transform.size.y * 0.5f;
		ImVec2 local[4] = {
			ImVec2(-hw, -hh),
			ImVec2(hw, -hh),
			ImVec2(hw, hh),
			ImVec2(-hw, hh)
		};
		float rad = particle.transform.rotation.x * (3.14159265358979323846f / 180.0f);
		float c = cosf(rad);
		float s = sinf(rad);
		for (int i = 0; i < 4; ++i) {
			outCorners[i].x = center.x + local[i].x * c - local[i].y * s;
			outCorners[i].y = center.y + local[i].x * s + local[i].y * c;
		}
	}


}
