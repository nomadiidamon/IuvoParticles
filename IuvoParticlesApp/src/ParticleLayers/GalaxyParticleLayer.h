#pragma once
#include "Walnut/Application.h"
#include <vector>
#include <cmath>
#include <string>
#include <cstdlib>

namespace Particles {

	struct GalaxyParticle
	{
		ParticleTransform	transform;
		ParticleAnimation2D	animation;
		ParticleColor		color;
		ParticleLifetime	lifetime;

		float brightness = 1.0f;
	};

	// Simple galaxy layer (IMGUI)
	class IMGUI_GALAXY_LAYER : public Walnut::Layer
	{
	public:
		ImDrawList* draw_list = nullptr;
		ImVec2 view_pos = ImVec2(0.0f, 0.0f);
		ImVec2 view_size = ImVec2(800.0f, 600.0f);

		std::vector<GalaxyParticle> stars;

		// controls / parameters
		ImVec2 centerOffset = ImVec2(0.5f, 0.5f); // relative center (0..1)
		int starCount = 800;
		int arms = 3;
		float radius = 600.0f;
		float armTwist = 2.5f; // how tightly arms twist
		float rotationSpeed = 0.2f; // global spin multiplier
		float starSizeMin = 0.8f;
		float starSizeMax = 2.5f;
		ImVec4 starColorInner = ImVec4(1.0f, 0.95f, 0.8f, 1.0f);
		ImVec4 starColorOuter = ImVec4(0.6f, 0.7f, 1.0f, 1.0f);

		bool initialized = false;
		bool autoRotate = true;
		float globalAngle = 0.0f;

		virtual void OnAttach() override
		{
			// defer initialization until viewport size known
		}

		void ResetAndGenerate()
		{
			stars.clear();
			stars.reserve(starCount);
			ImVec2 center = ImVec2(view_pos.x + view_size.x * centerOffset.x, view_pos.y + view_size.y * centerOffset.y);
			float maxR = std::min(view_size.x, view_size.y) * 0.5f;
			float useRadius = (radius > 0.0f) ? radius : maxR;

			for (int i = 0; i < starCount; ++i) {
				// choose radial distance (bias toward center for denser core)
				float t = static_cast<float>(rand()) / RAND_MAX;
				float r = powf(t, 0.65f) * useRadius; // bias
				// choose arm and angle
				int arm = rand() % std::max(1, arms);
				float armAngleBase = (2.0f * 3.14159265358979323846f * arm) / std::max(1, arms);
				// add twist proportional to radius to make spiral
				float theta = armAngleBase + r / useRadius * armTwist * 2.0f * 3.14159265f;
				// jitter
				theta += ((static_cast<float>(rand()) / RAND_MAX) - 0.5f) * 0.5f;
				// polar to cartesian (local to center)
				float sx = cosf(theta) * r;
				float sy = sinf(theta) * r;

				// visual radius (used to derive rect size)
				float visualRadius = starSizeMin + (static_cast<float>(rand()) / RAND_MAX) * (starSizeMax - starSizeMin);
				float diameter = visualRadius * 2.0f;

				GalaxyParticle g;
				// transform: position is top-left; we want center at center + (sx, sy)
				g.transform.size = ImVec2(diameter, diameter);
				g.transform.baseSize = g.transform.size;
				g.transform.position = ImVec2(center.x + sx - visualRadius, center.y + sy - visualRadius);
				g.transform.center = ImVec2(g.transform.position.x + visualRadius, g.transform.position.y + visualRadius);
				g.transform.randomRotation = false;
				g.transform.randomSize = false;

				// velocity: tangent to radius for orbit-like motion
				ImVec2 radial = ImVec2(sx, sy);
				float radLen = sqrtf(radial.x * radial.x + radial.y * radial.y) + 0.0001f;
				ImVec2 tangent = ImVec2(-radial.y / radLen, radial.x / radLen);
				float speed = (1.0f - (r / useRadius)) * (20.0f + static_cast<float>(rand()) / RAND_MAX * 40.0f);

				g.animation.p_velocity = tangent;
				g.animation.p_movementSpeed = speed;
				g.animation.canMove = true;
				g.animation.canRotate = false;
				g.animation.p_rotationSpeed = 0.0f;

				// color lerp inner->outer by r/useRadius, with jitter
				float lerp = std::clamp(r / useRadius + (((static_cast<float>(rand()) / RAND_MAX) - 0.5f) * 0.1f), 0.0f, 1.0f);
				g.color.startColor = ImVec4(
					starColorInner.x + (starColorOuter.x - starColorInner.x) * lerp,
					starColorInner.y + (starColorOuter.y - starColorInner.y) * lerp,
					starColorInner.z + (starColorOuter.z - starColorInner.z) * lerp,
					1.0f
				);
				g.color.currColor = g.color.startColor;
				g.color.lerpColor = false;
				g.color.useAlpha = true;

				g.lifetime.maxLifetime = FLT_MAX;
				g.lifetime.randomizeLifetime = false;
				g.brightness = 0.6f + (static_cast<float>(rand()) / RAND_MAX) * 0.8f;

				stars.push_back(std::move(g));
			}
			initialized = true;
		}

		virtual void OnUIRender() override
		{
			if (ImGui::Begin("Galaxy Layer")) {
				draw_list = ImGui::GetWindowDrawList();
				ImVec2 win_pos = ImGui::GetWindowPos();
				ImVec2 win_size = ImGui::GetWindowSize();
				view_pos = win_pos;
				view_size = win_size;

				// controls (basic)
				if (ImGui::Button("Generate Galaxy")) {
					ResetAndGenerate();
				}
				ImGui::SameLine();
				if (ImGui::Button("Clear")) {
					stars.clear();
				}
				ImGui::Checkbox("Auto Rotate", &autoRotate);
				ImGui::DragInt("Stars", &starCount, 10, 0, 10000);
				ImGui::DragInt("Arms", &arms, 1, 1, 12);
				ImGui::DragFloat("Arm Twist", &armTwist, 0.01f, 0.0f, 10.0f);
				ImGui::DragFloat("Radius", &radius, 1.0f, 10.0f, std::max(10.0f, std::min(view_size.x, view_size.y)));
				ImGui::DragFloat("Rotation Speed", &rotationSpeed, 0.01f, -5.0f, 5.0f);
				ImGui::DragFloatRange2("Star Size Min/Max", &starSizeMin, &starSizeMax, 0.1f, 0.1f, 10.0f);
				ImGui::ColorEdit4("Inner Color", (float*)&starColorInner);
				ImGui::ColorEdit4("Outer Color", (float*)&starColorOuter);

				// viewport background (simple)
				draw_list->AddRectFilled(view_pos, ImVec2(view_pos.x + view_size.x, view_pos.y + view_size.y), ImGui::GetColorU32(ImVec4(0, 0, 0, 1)));

				// draw stars
				for (auto& s : stars) {
					float px = s.transform.position.x;
					float py = s.transform.position.y;
					float halfSize = s.transform.size.x * 0.5f;
					ImVec2 centerPt = ImVec2(px + halfSize, py + halfSize);

					// skip if off-screen (cheap check)
					if (centerPt.x < view_pos.x - 10.0f || centerPt.x > view_pos.x + view_size.x + 10.0f ||
						centerPt.y < view_pos.y - 10.0f || centerPt.y > view_pos.y + view_size.y + 10.0f)
						continue;

					// build colors with brightness
					ImU32 col = ImGui::GetColorU32(ImVec4(s.color.currColor.x * s.brightness, s.color.currColor.y * s.brightness, s.color.currColor.z * s.brightness, 1.0f));
					// soft glow: outer faint, mid glow, core
					draw_list->AddCircleFilled(ImVec2(centerPt.x, centerPt.y), halfSize * 3.0f, ImGui::GetColorU32(ImVec4(s.color.currColor.x, s.color.currColor.y, s.color.currColor.z, 0.06f)), 12);
					draw_list->AddCircleFilled(ImVec2(centerPt.x, centerPt.y), halfSize * 1.6f, ImGui::GetColorU32(ImVec4(s.color.currColor.x, s.color.currColor.y, s.color.currColor.z, 0.28f)), 12);
					draw_list->AddCircleFilled(ImVec2(centerPt.x, centerPt.y), halfSize * 0.6f, col, 12);
				}

			}
				ImGui::End();
		}

		virtual void OnUpdate(float ts) override
		{
			if (!initialized) {
				// ensure view_size is valid before generating
				if (view_size.x > 10 && view_size.y > 10)
					ResetAndGenerate();
				else
					return;
			}

			float simTs = ts * rotationSpeed;
			if (autoRotate) {
				globalAngle += simTs * 0.5f;
			}

			// update particles: apply simple orbital + damping
			ImVec2 center = ImVec2(view_pos.x + view_size.x * centerOffset.x, view_pos.y + view_size.y * centerOffset.y);
			for (auto& s : stars) {
				// compute current center of particle (screen coords)
				float halfSize = s.transform.size.x * 0.5f;
				ImVec2 currCenter = ImVec2(s.transform.position.x + halfSize, s.transform.position.y + halfSize);

				// vector to center
				ImVec2 toCenter = ImVec2(center.x - currCenter.x, center.y - currCenter.y);
				float dist2 = toCenter.x * toCenter.x + toCenter.y * toCenter.y + 1.0f;
				float invDist = 1.0f / sqrtf(dist2);

				// current velocity vector (world)
				ImVec2 v = ImVec2(s.animation.p_velocity.x * s.animation.p_movementSpeed, s.animation.p_velocity.y * s.animation.p_movementSpeed);

				// small gravitational attraction toward center (optional, slight)
				float gforce = 10.0f * invDist * 0.02f;
				v.x += toCenter.x * gforce * simTs;
				v.y += toCenter.y * gforce * simTs;

				// apply global spin by rotating velocity slightly
				if (autoRotate) {
					float angle = simTs * 0.1f;
					float vx = v.x * cosf(angle) - v.y * sinf(angle);
					float vy = v.x * sinf(angle) + v.y * cosf(angle);
					v.x = vx;
					v.y = vy;
				}

				// update position (transform.position is top-left)
				s.transform.position.x += v.x * simTs;
				s.transform.position.y += v.y * simTs;

				// convert back into direction + magnitude for animation storage
				float speed = sqrtf(v.x * v.x + v.y * v.y);
				if (speed > 0.0001f) {
					s.animation.p_velocity = ImVec2(v.x / speed, v.y / speed);
					s.animation.p_movementSpeed = speed;
				}
				else {
					s.animation.p_movementSpeed = 0.0f;
				}

				// slight damping to keep things stable
				s.animation.p_movementSpeed *= 0.9995f;

				// maintain center sync
				s.transform.center = ImVec2(s.transform.position.x + halfSize, s.transform.position.y + halfSize);
			}
		}
	};

	class IMGUI_GALAXY_LAYER_PROPERTIES : public Walnut::Layer
	{
	public:
		IMGUI_GALAXY_LAYER* layer = nullptr;
		void SetGalaxyLayer(IMGUI_GALAXY_LAYER* l) { layer = l; }

		virtual void OnUIRender() override
		{
			if (!layer) return;
			ImGui::Begin("Galaxy Properties");
			if (ImGui::Button("Regenerate")) {
				layer->ResetAndGenerate();
			}
			ImGui::SameLine();
			if (ImGui::Button("Clear")) {
				layer->stars.clear();
			}
			ImGui::Separator();

			ImGui::DragInt("Stars", &layer->starCount, 10, 0, 10000);
			ImGui::DragInt("Arms", &layer->arms, 1, 1, 12);
			ImGui::DragFloat("Arm Twist", &layer->armTwist, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("Radius", &layer->radius, 1.0f, 10.0f, std::max(10.0f, std::min(layer->view_size.x, layer->view_size.y)));
			ImGui::Checkbox("Auto Rotate", &layer->autoRotate);
			ImGui::DragFloat("Rotation Speed", &layer->rotationSpeed, 0.01f, -5.0f, 5.0f);
			ImGui::ColorEdit4("Inner Color", (float*)&layer->starColorInner);
			ImGui::ColorEdit4("Outer Color", (float*)&layer->starColorOuter);
			ImGui::DragFloatRange2("Star Size Min/Max", &layer->starSizeMin, &layer->starSizeMax, 0.1f, 0.1f, 30.0f);
			ImGui::End();
		}
	};

} // namespace Particles