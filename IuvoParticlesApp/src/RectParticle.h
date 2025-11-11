#pragma once
#include "Walnut/Application.h"
#include "Walnut/Image.h"
#include <algorithm>

namespace Particles {

	template <typename T>
	class Range {
	public:
		T min;
		T max;
		T val;

		bool autoClamp = true;

		Range(T minimum, T maximum, T val)
			: min(minimum), max(maximum), val(val) {

		}

		virtual void Clamp() {
			if (autoClamp) {
				if (val < min) val = min;
				if (val > max) val = max;
			}
		}

		virtual void SetValue(T newVal) {
			val = newVal;
			Clamp();
		}

		virtual void Randomize() {
			val = min + static_cast <T> (rand()) / (static_cast <T> (RAND_MAX / (max - min)));
		}

	};


	struct ParticleColor {
		ImVec4 p_startColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		ImVec4 p_endColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		ImVec4 p_currColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		float p_lerpSpeed = 0.0f;

		bool p_lerpColor = false;
		bool p_fadeColor = true;
		bool p_useAlpha = true;
		bool p_randomizeColor = false;
	};

	struct ParticleLifetime {
		float p_lifetime = 0.0f;
		float p_maxLifetime = 0.0f;
		bool p_randomizeLifetime = false;
		bool p_loop = false;
	};

	struct ParticleTransform {
		ImVec2 pt_position = ImVec2(0.0f, 0.0f);
		ImVec2 pt_center = ImVec2(0.0f, 0.0f);

		ImVec2 pt_size = ImVec2(0.0f, 0.0f);
		ImVec2 p_rotation = ImVec2(0.0f, 0.0f); // using x as angle in degrees for 2D rotation

		bool p_randomSize;
		bool p_randomRotation;
	};

	struct ParticleAnimation2D {
		ImVec2 p_velocity = ImVec2(0.0f, 0.0f);

		float p_movementSpeed = 0.0f;
		float p_rotationSpeed = 0.0f;
		float p_scaleSpeed = 0.0f;

		bool canMove = true;
		bool canRotate = true;
		bool canScale = false;
	};

	struct RectParticle {
		ParticleTransform rp_transform;
		ParticleAnimation2D rp_animation;
		ParticleColor rp_colorData;
		ParticleLifetime rp_lifetimeData;
	};

	static class RectParticleUtils {
	public:
		static RectParticle CreateRandomParticle(const ImVec2& view_size, const ImVec2& position = ImVec2(0.0f, 0.0f), const RectParticle& defaultParticle = RectParticle()) {
			RectParticle particle;

#pragma region Particle Transform
			// Particle Position
			if (position.x != 0.0f || position.y != 0.0f) {
				particle.rp_transform.pt_position = position;
			}
			else
			{
				particle.rp_transform.pt_position = ImVec2(rand() % (int)view_size.x, rand() % (int)view_size.y);
			}

			// Particle rotation
			if (particle.rp_transform.p_randomRotation) {
				particle.rp_transform.p_rotation.x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 360.0f));
			}
			else
			{

			}

			// Particle Center
			particle.rp_transform.pt_center = ImVec2(particle.rp_transform.pt_position.x + particle.rp_transform.pt_size.x / 2.0f,
				particle.rp_transform.pt_position.y + particle.rp_transform.pt_size.y / 2.0f);

			// Particle Size
			if (particle.rp_transform.p_randomSize) {
				// Random size between 5.0 and 20.0
				particle.rp_transform.pt_size = ImVec2(10.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 200.0f)),
					10.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 200.0f)));
			}
			else {
				particle.rp_transform.pt_size = ImVec2(10.0f, 10.0f);
			}
#pragma endregion

#pragma region Particle Animation 2D
			// Particle rotation speed between -90.0 and 90.0 degrees per second
			particle.rp_animation.p_rotationSpeed = -45.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 180.0f));

			// Particle velocity direction
			float angle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0f * 3.14159f)));
			particle.rp_animation.p_velocity = ImVec2(cos(angle), sin(angle));

			// Random speed between 20.0 and 100.0
			particle.rp_animation.p_movementSpeed = 20.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 80.0f));
#pragma endregion

#pragma region Particle Lifetime
			// Random max lifetime between 1.0 and 4.0 seconds
			particle.rp_lifetimeData.p_lifetime = 0.0f;
			particle.rp_lifetimeData.p_maxLifetime = 1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 3.0f));
#pragma endregion

#pragma region Particle Color
			// Random color
			particle.rp_colorData.p_currColor = ImVec4(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
				static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
				static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 1.0f);
#pragma endregion

			return particle;
		}

		static RectParticle CreateDefaultParticle(ImVec2 view_size, RectParticle& defaultParticle = RectParticle()) {
			RectParticle trueDefaultParticle;
			if (IsEqual(defaultParticle, trueDefaultParticle)) {
				return CreateParticle(ParticleTransform(), ParticleColor(), ParticleLifetime());
			}
			else {
				// Center of Viewport
				defaultParticle.rp_transform.pt_position.x = view_size.x / 2.0f;
				defaultParticle.rp_transform.pt_position.y = view_size.y / 2.0f;
				// Size 10x10
				defaultParticle.rp_transform.pt_size = ImVec2(10.0f, 10.0f);
				// No Size Randomization
				defaultParticle.rp_transform.p_randomSize = false;
				// No Rotation			
				defaultParticle.rp_transform.p_randomRotation = false;

				// Downward Velocity
				defaultParticle.rp_animation.p_velocity = ImVec2(0.0f, -1.0f);
				// Moderate Speed			
				defaultParticle.rp_animation.p_movementSpeed = 50.0f;
				// Enable Movement
				defaultParticle.rp_animation.canMove = true;

				// Start Color Red
				defaultParticle.rp_colorData.p_startColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
				// End Color Yellow
				defaultParticle.rp_colorData.p_endColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
				// Lerp Color Enabled
				defaultParticle.rp_colorData.p_lerpColor = true;
				// Use Alpha Channel
				defaultParticle.rp_colorData.p_useAlpha = true;
				// Lifetime 2 Seconds
				defaultParticle.rp_lifetimeData.p_maxLifetime = 2.0f;
				// No Lifetime Randomization
				defaultParticle.rp_lifetimeData.p_randomizeLifetime = false;
			}
		}

		static RectParticle CreateParticle(ParticleTransform p_transform, ParticleColor p_color, ParticleLifetime p_lifetime) {
			RectParticle particle;
			particle.rp_transform = p_transform;
			particle.rp_colorData = p_color;
			particle.rp_lifetimeData = p_lifetime;
			return particle;
		}

		static void ResetParticle(RectParticle& particle, const ImVec2& view_size, bool useDefaultParticle = false, const RectParticle& defaultParticle = RectParticle()) {

			if (useDefaultParticle) {
				particle = CreateParticle(defaultParticle.rp_transform, defaultParticle.rp_colorData, defaultParticle.rp_lifetimeData);
				return;
			}
			else {

				particle = CreateRandomParticle(view_size);
				if (!particle.rp_colorData.p_lerpColor) {
					// set random color
					particle.rp_colorData.p_currColor = ImVec4(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
						static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
						static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 1.0f);
				}
				else {
					// set random end colors
					particle.rp_colorData.p_startColor = particle.rp_colorData.p_endColor;
					particle.rp_colorData.p_endColor = ImVec4(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
						static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
						static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 1.0f);
				}
			}

		}

		static bool IsExpired(const RectParticle& particle) {
			return particle.rp_lifetimeData.p_lifetime >= particle.rp_lifetimeData.p_maxLifetime;
		}

		static bool IsEqual(RectParticle& p_A, RectParticle& p_B) {
			return (p_A.rp_transform.pt_position.x == p_B.rp_transform.pt_position.x &&
				p_A.rp_transform.pt_position.y == p_B.rp_transform.pt_position.y &&
				p_A.rp_transform.pt_size.x == p_B.rp_transform.pt_size.x &&
				p_A.rp_transform.pt_size.y == p_B.rp_transform.pt_size.y &&
				p_A.rp_colorData.p_startColor.x == p_B.rp_colorData.p_startColor.x &&
				p_A.rp_colorData.p_startColor.y == p_B.rp_colorData.p_startColor.y &&
				p_A.rp_colorData.p_startColor.z == p_B.rp_colorData.p_startColor.z &&
				p_A.rp_colorData.p_endColor.x == p_B.rp_colorData.p_endColor.x &&
				p_A.rp_colorData.p_endColor.y == p_B.rp_colorData.p_endColor.y &&
				p_A.rp_colorData.p_endColor.z == p_B.rp_colorData.p_endColor.z &&
				p_A.rp_lifetimeData.p_maxLifetime == p_B.rp_lifetimeData.p_maxLifetime);
		}

		static void ApplyVelocity(RectParticle& particle, float ts) {
			particle.rp_transform.pt_position.x += particle.rp_animation.p_velocity.x * ts * particle.rp_animation.p_movementSpeed;
			particle.rp_transform.pt_position.y += particle.rp_animation.p_velocity.y * ts * particle.rp_animation.p_movementSpeed;
		}

		static void ApplyRotation(RectParticle& particle, float ts) {
			particle.rp_animation.p_rotationSpeed += particle.rp_animation.p_rotationSpeed * ts;
			if (particle.rp_transform.p_rotation.x >= 360.0f) {
				particle.rp_transform.p_rotation.x -= 360.0f;
			}
			else if (particle.rp_transform.p_rotation.x < 0.0f) {
				particle.rp_transform.p_rotation.x += 360.0f;
			}
		}

		static void CalculateRotatedRectCorners(RectParticle& particle, ImVec2& out_corners) {
			// calculate the four corners of the rotated rectangle
			float rad = particle.rp_transform.p_rotation.x * 3.14159f / 180.0f;
			float cosA = cos(rad);
			float sinA = sin(rad);
			ImVec2 halfSize = ImVec2(particle.rp_transform.pt_size.x / 2.0f, particle.rp_transform.pt_size.y / 2.0f);
			out_corners = ImVec2(
				-particle.rp_transform.pt_position.x * cosA - particle.rp_transform.pt_position.y * sinA + halfSize.x,
				particle.rp_transform.pt_position.x * sinA - particle.rp_transform.pt_position.y * cosA + halfSize.y
			);
			particle.rp_transform.pt_position.x += out_corners.x;
			particle.rp_transform.pt_position.y += out_corners.y;
		}

		static void UpdateRotation(RectParticle& particle, float ts) {
			ApplyRotation(particle, ts);
			CalculateRotatedRectCorners(particle, particle.rp_transform.pt_position);
		}

		static void UpdateAnimation2D(RectParticle& particle, float ts) {

			if (particle.rp_animation.canMove)
				ApplyVelocity(particle, ts);


			//if (particle.rp_animation.canRotate)
			//	ApplyRotation(particle, ts);


			if (particle.rp_animation.canScale) {
				// Implement scaling logic if needed
			}
		}

		static void UpdateLifetime(RectParticle& particle, float ts) {
			particle.rp_lifetimeData.p_lifetime += ts;
		}

		static ImU32 FadeColor(RectParticle& particle) {
			float lifeRatio = particle.rp_lifetimeData.p_lifetime / particle.rp_lifetimeData.p_maxLifetime;
			particle.rp_colorData.p_currColor.w = 1.0f - lifeRatio; // Fade out based on lifetime
			return ImGui::ColorConvertFloat4ToU32(particle.rp_colorData.p_currColor);
		}

		static ImU32 LerpColor(RectParticle& particle) {
			float lifeRatio = particle.rp_lifetimeData.p_lifetime / particle.rp_lifetimeData.p_maxLifetime;
			particle.rp_colorData.p_currColor = ImVec4(
				particle.rp_colorData.p_startColor.x + (particle.rp_colorData.p_endColor.x - particle.rp_colorData.p_startColor.x) * lifeRatio,
				particle.rp_colorData.p_startColor.y + (particle.rp_colorData.p_endColor.y - particle.rp_colorData.p_startColor.y) * lifeRatio,
				particle.rp_colorData.p_startColor.z + (particle.rp_colorData.p_endColor.z - particle.rp_colorData.p_startColor.z) * lifeRatio,
				1.0f);
			return ImGui::ColorConvertFloat4ToU32(particle.rp_colorData.p_currColor);
		}

		static void UpdateColor(RectParticle& particle) {
			if (particle.rp_colorData.p_lerpColor) {
				LerpColor(particle);
			}
			else {
				FadeColor(particle);
			}
		}

		static bool HitViewportBounds(const RectParticle& particle, const ImVec2& view_size) {
			return (particle.rp_transform.pt_position.x < 0 || particle.rp_transform.pt_position.x > view_size.x ||
				particle.rp_transform.pt_position.y < 0 || particle.rp_transform.pt_position.y > view_size.y);
		}

		static bool ApplyReboundForce(RectParticle& particle, const ImVec2& view_size) {
			bool rebounded = false;
			if (particle.rp_transform.pt_position.x < 0 || particle.rp_transform.pt_position.x > view_size.x) {
				particle.rp_animation.p_velocity.x = -particle.rp_animation.p_velocity.x;
				rebounded = true;
			}
			if (particle.rp_transform.pt_position.y < 0 || particle.rp_transform.pt_position.y > view_size.y) {
				particle.rp_animation.p_velocity.y = -particle.rp_animation.p_velocity.y;
				rebounded = true;
			}
			return rebounded;
		}

		static void UpdateParticle(RectParticle& particle, float ts, const ImVec2& view_size, bool useDefaultParticle = false, const RectParticle& defaultParticle = RectParticle()) {
			if (IsExpired(particle)) {
				ResetParticle(particle, view_size, useDefaultParticle, defaultParticle);
			}
			if (HitViewportBounds(particle, view_size)) {
				ApplyReboundForce(particle, view_size);
			}

			ApplyVelocity(particle, ts);
			UpdateRotation(particle, ts);
			UpdateAnimation2D(particle, ts);

			UpdateLifetime(particle, ts);
			UpdateColor(particle);
		}

		static void DrawParticle(ImDrawList* draw_list, const RectParticle& particle, const ImVec2& view_pos) {
			ImU32 color = ImGui::GetColorU32(particle.rp_colorData.p_currColor);
			draw_list->AddRectFilled(ImVec2(view_pos.x + particle.rp_transform.pt_position.x, view_pos.y + particle.rp_transform.pt_position.y),
				ImVec2(view_pos.x + particle.rp_transform.pt_position.x + particle.rp_transform.pt_size.x, view_pos.y + particle.rp_transform.pt_position.y + particle.rp_transform.pt_size.y),
				color);
		}

		static void DrawParticle(ImDrawList* draw_list, const RectParticle& particle, const ImVec2& view_pos, const ImU32& override_color) {
			draw_list->AddRectFilled(ImVec2(view_pos.x + particle.rp_transform.pt_position.x, view_pos.y + particle.rp_transform.pt_position.y),
				ImVec2(view_pos.x + particle.rp_transform.pt_position.x + particle.rp_transform.pt_size.x, view_pos.y + particle.rp_transform.pt_position.y + particle.rp_transform.pt_size.y),
				override_color);
		}

		static void DrawRotatingParticle(ImDrawList* draw_list, const RectParticle& particle, const ImVec2& view_pos) {
			ImU32 color = ImGui::GetColorU32(particle.rp_colorData.p_currColor);
			// calculate the four corners of the rotated rectangle
			float rad = particle.rp_transform.p_rotation.x * (3.14159f / 180.0f) * particle.rp_animation.p_rotationSpeed;
			float cosA = cos(rad);
			float sinA = sin(rad);
			ImVec2 halfSize = ImVec2(particle.rp_transform.pt_size.x / 2.0f, particle.rp_transform.pt_size.y / 2.0f);
			ImVec2 corners[4];
			corners[0] = ImVec2(-halfSize.x * cosA - -halfSize.y * sinA, -halfSize.x * sinA + -halfSize.y * cosA); // Top-left
			corners[1] = ImVec2(halfSize.x * cosA - -halfSize.y * sinA, halfSize.x * sinA + -halfSize.y * cosA);   // Top-right
			corners[2] = ImVec2(halfSize.x * cosA - halfSize.y * sinA, halfSize.x * sinA + halfSize.y * cosA);     // Bottom-right
			corners[3] = ImVec2(-halfSize.x * cosA - halfSize.y * sinA, -halfSize.x * sinA + halfSize.y * cosA);   // Bottom-left
			for (int i = 0; i < 4; ++i) {
				corners[i].x += view_pos.x + particle.rp_transform.pt_position.x + halfSize.x;
				corners[i].y += view_pos.y + particle.rp_transform.pt_position.y + halfSize.y;
			}
			draw_list->AddConvexPolyFilled(corners, 4, color);
		}

		// Sort particles by lifetime (ascending)
		static void SortParticlesByLifetime(std::vector<RectParticle>& particles) {
			std::sort(particles.begin(), particles.end(),
				[](const RectParticle& a, const RectParticle& b) {
					return a.rp_lifetimeData.p_lifetime < b.rp_lifetimeData.p_lifetime;
				});
		}

		static void ClampParticleCount(std::vector<RectParticle>& particles, int maxCount, const ImVec2& view_size) {
			while (particles.size() > maxCount) {
				particles.pop_back();
			}
		}

		static void RestrictParticles(std::vector<RectParticle>& particles, int maxCount, const ImVec2& view_size) {
			SortParticlesByLifetime(particles);
			ClampParticleCount(particles, maxCount, view_size);
		}

		static bool IsDefaultParticle(const RectParticle& particle) {
			bool default = false;
			ParticleColor defaultColor;
			ParticleAnimation2D defaultAnimation;
			ParticleLifetime defaultLifetime;
			ParticleTransform defaultTransform;

			/// Transform checks
			if (particle.rp_transform.pt_position.x == defaultTransform.pt_position.x &&
				particle.rp_transform.pt_position.y == defaultTransform.pt_position.y &&
				particle.rp_transform.pt_size.x == defaultTransform.pt_size.x &&
				particle.rp_transform.pt_size.y == defaultTransform.pt_size.y) {
				default = true;
			}
			/// Animation checks
			if (particle.rp_animation.p_velocity.x == defaultAnimation.p_velocity.x &&
				particle.rp_animation.p_velocity.y == defaultAnimation.p_velocity.y &&
				particle.rp_animation.p_movementSpeed == defaultAnimation.p_movementSpeed) {
				default = true;
			}
			/// Color checks
			if (particle.rp_colorData.p_currColor.x == defaultColor.p_currColor.x &&
				particle.rp_colorData.p_currColor.y == defaultColor.p_currColor.y &&
				particle.rp_colorData.p_currColor.z == defaultColor.p_currColor.z &&
				particle.rp_colorData.p_currColor.w == defaultColor.p_currColor.w) {
				default = true;
			}
			/// Lifetime checks
			if (particle.rp_lifetimeData.p_lifetime == defaultLifetime.p_lifetime &&
				particle.rp_lifetimeData.p_maxLifetime == defaultLifetime.p_maxLifetime) {
				default = true;
			}
			return default;
		}


	} RectParticleUtils;

	struct BackgroundParticle {
		ParticleColor bg_colorData;
		ParticleLifetime bg_lifetimeData;
	};

	static class BackgroundParticleUtils {

	};


	class IMGUI_2D_PARTICLE_LAYER : public Walnut::Layer
	{
	public:
		ImDrawList* draw_list;
		ImVec2 view_pos;
		ImVec2 view_size;
		ImVec2 min_view_size = ImVec2(200, 200);
		ImVec2 mouse_pos;
		std::vector<RectParticle> particles;
		const int defaultParticleCount = 100;
		const int maxParticleCount = 300;

		bool is_hovered = false;
		int hoverBurstCount = 25;
		float hoverBurstInterval = 0.025f;
		float hoverBurstTimer = 0.0f;

		RectParticle defaultParticle;
		BackgroundParticle bg_particle;
		bool useDefaultParticle = false;

		void HandleHoverBurst(float ts)
		{
			if (is_hovered) {
				hoverBurstTimer += ts;
				if (hoverBurstTimer >= hoverBurstInterval) {
					hoverBurstTimer = 0.0f;
					// Create burst particles at mouse position withing viewport 
					for (int i = 0; i < hoverBurstCount; ++i) {
						particles.push_back(RectParticleUtils::CreateRandomParticle(view_size, mouse_pos));
					}
				}
			}
		}

		virtual void OnUIRender() override
		{

			if (ImGui::Begin("2D Particle Layer")) {

				draw_list = ImGui::GetWindowDrawList();
				view_pos = ImGui::GetCursorScreenPos();
				view_size = ImGui::GetContentRegionAvail();
				if (view_size.x < min_view_size.x) view_size.x = min_view_size.x;
				if (view_size.y < min_view_size.y) view_size.y = min_view_size.y;
				ImGui::InvisibleButton("viewport", view_size);
				if (ImGui::IsItemHovered()) {
					is_hovered = true;
					mouse_pos = ImVec2(ImGui::GetIO().MousePos.x - view_pos.x, ImGui::GetIO().MousePos.y - view_pos.y);
				}
				else {
					is_hovered = false;
				}


				/// Background particle
				if (bg_particle.bg_lifetimeData.p_lifetime >= bg_particle.bg_lifetimeData.p_maxLifetime) {
					bg_particle.bg_lifetimeData.p_lifetime = 0.0f;
					bg_particle.bg_lifetimeData.p_maxLifetime = 5.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 5.0f));

					bg_particle.bg_colorData.p_startColor = bg_particle.bg_colorData.p_endColor;
					bg_particle.bg_colorData.p_endColor = ImVec4(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
						static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
						static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 1.0f);
				}
				ImU32 bg_color = ImGui::GetColorU32(bg_particle.bg_colorData.p_currColor);
				draw_list->AddRectFilled(view_pos, ImVec2(view_pos.x + view_size.x, view_pos.y + view_size.y), bg_color);



				/// Basic particles
				if (particles.empty()) {

					if (useDefaultParticle) {
						for (int i = 0; i < defaultParticleCount; ++i) {
							particles.push_back(RectParticleUtils::CreateParticle(
								defaultParticle.rp_transform,
								defaultParticle.rp_colorData,
								defaultParticle.rp_lifetimeData
							));
						}
					}
					else
					{
						for (int i = 0; i < defaultParticleCount; ++i) {
							particles.push_back(RectParticleUtils::CreateRandomParticle(view_size));
						}
					}

				}
				for (const auto& particle : particles) {

					if (particle.rp_animation.canRotate)
					{
						RectParticleUtils::DrawRotatingParticle(draw_list, particle, view_pos);
					}
					else
					{
						RectParticleUtils::DrawParticle(draw_list, particle, view_pos);
					}

				}
				RectParticleUtils::RestrictParticles(particles, maxParticleCount, view_size);
			}

			ImGui::End();
		}

		virtual void OnUpdate(float ts) override
		{
			// Update code for the viewport would go here
			for (auto& particle : particles) {
				RectParticleUtils::UpdateParticle(particle, ts, view_size, useDefaultParticle, defaultParticle);
			}

			// update background particle lifetime
			bg_particle.bg_lifetimeData.p_lifetime += ts;
			// Lerp current color from start color to end color based on lifetime
			float bg_lifeRatio = bg_particle.bg_lifetimeData.p_lifetime / bg_particle.bg_lifetimeData.p_maxLifetime;
			bg_particle.bg_colorData.p_currColor = ImVec4(
				bg_particle.bg_colorData.p_startColor.x + (bg_particle.bg_colorData.p_endColor.x - bg_particle.bg_colorData.p_startColor.x) * bg_lifeRatio,
				bg_particle.bg_colorData.p_startColor.y + (bg_particle.bg_colorData.p_endColor.y - bg_particle.bg_colorData.p_startColor.y) * bg_lifeRatio,
				bg_particle.bg_colorData.p_startColor.z + (bg_particle.bg_colorData.p_endColor.z - bg_particle.bg_colorData.p_startColor.z) * bg_lifeRatio,
				1.0f);

			HandleHoverBurst(ts);
		}
	};

	class IMGUI_2D_PARTICLE_LAYER_PROPERTIES : public Walnut::Layer
	{
	public:
		float sliderSpeed = 1.0f;
		int defaultParticleCount = 100;
		int maxParticleCount = 300;
		bool is_hovered = false;
		int hoverBurstCount = 25;
		float hoverBurstInterval = 0.025f;
		float hoverBurstTimer = 0.0f;

		RectParticle defaultParticle;
		BackgroundParticle defaultBackground;
		bool useDefaultParticle = false;

		IMGUI_2D_PARTICLE_LAYER p_layer;
		bool is_active = false;

		void SetupDefaultParticle() {
			// Center of Viewport
			defaultParticle.rp_transform.pt_position = ImVec2(p_layer.view_size.x / 2.0f, p_layer.view_size.y / 2.0f);
			// Size 10x10
			defaultParticle.rp_transform.pt_size = ImVec2(10.0f, 10.0f);
			// No Size Randomization
			defaultParticle.rp_transform.p_randomSize = false;
			// No Rotation			
			defaultParticle.rp_transform.p_randomRotation = false;

			// Downward Velocity
			defaultParticle.rp_animation.p_velocity = ImVec2(0.0f, -1.0f);
			// Moderate Speed			
			defaultParticle.rp_animation.p_movementSpeed = 50.0f;
			// Enable Movement
			defaultParticle.rp_animation.canMove = true;

			// Start Color Red
			defaultParticle.rp_colorData.p_startColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
			// End Color Yellow
			defaultParticle.rp_colorData.p_endColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
			// Lerp Color Enabled
			defaultParticle.rp_colorData.p_lerpColor = true;
			// Use Alpha Channel
			defaultParticle.rp_colorData.p_useAlpha = true;
			// Lifetime 2 Seconds
			defaultParticle.rp_lifetimeData.p_maxLifetime = 2.0f;
			// No Lifetime Randomization
			defaultParticle.rp_lifetimeData.p_randomizeLifetime = false;
		}

		virtual void OnAttach() override {
			SetupDefaultParticle();
			p_layer.defaultParticle = defaultParticle;
			p_layer.useDefaultParticle = useDefaultParticle;
		}

		virtual void OnUIRender() override {
			p_layer.defaultParticle = defaultParticle;
			p_layer.useDefaultParticle = useDefaultParticle;
			p_layer.OnUIRender();

			ImGui::Begin("2D Particle Details");
			ImVec2 max = ImVec2(ImGui::GetWindowContentRegionMax().x * 2.0f, ImGui::GetWindowContentRegionMax().y * 2.0f);
			ImVec2 min = ImGui::GetWindowPos();
			ImVec2 propMax = ImVec2(min.x + max.x, min.y + max.y);
			is_active = ImGui::IsMouseHoveringRect(ImGui::GetWindowPos(), max);
			if (is_active)
			{


			}


			// set min size from properties window to fit default particle controls
			ImVec2 propMin = ImVec2();
			// get the length of all checkbox labels
			propMin = ImGui::CalcTextSize("Random Size Random Rotation Use Default Particle", nullptr, true, 0.0f);


			// make sure the default particle tree is always minimum size to fit all controls
			ImGui::SetNextItemWidth(propMin.x + 50.0f);
			if (ImGui::TreeNode("Default Particle"))
			{

				if (is_active) {

					if (ImGui::Checkbox("Random Size", &defaultParticle.rp_transform.p_randomSize)) {
						p_layer.defaultParticle.rp_transform.p_randomSize = defaultParticle.rp_transform.p_randomSize;
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Random Rotation", &defaultParticle.rp_transform.p_randomRotation)) {
						p_layer.defaultParticle.rp_transform.p_randomRotation = defaultParticle.rp_transform.p_randomRotation;
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Use Default Particle", &useDefaultParticle)) {
						p_layer.useDefaultParticle = useDefaultParticle;
						SetupDefaultParticle();
					}
					ImGui::Separator();
					if (ImGui::Button("Reset to Default Values")) {
						SetupDefaultParticle();
					}
					ImGui::SameLine();
					if (ImGui::Button("Clear Values")) {
						defaultParticle = RectParticle();
					}
					ImGui::Separator();
					if (ImGui::Button("Save As Default")) {
						defaultParticle = p_layer.defaultParticle;
					}
					ImGui::SameLine();
					if (ImGui::Button("Center Particle")) {
						defaultParticle.rp_transform.pt_position = ImVec2(p_layer.view_size.x / 2.0f, p_layer.view_size.y / 2.0f);
					}
					ImGui::SameLine();
					if (ImGui::Button("Random Values")) {
						defaultParticle = RectParticleUtils::CreateRandomParticle(p_layer.view_size);
					}
				}

				if (ImGui::TreeNode("Particle Transform"))
				{
					if (is_active) {

					}
					ImGui::DragFloat("Position X", &defaultParticle.rp_transform.pt_position.x, sliderSpeed, 0.0f, p_layer.view_size.x);
					ImGui::DragFloat("Position Y", &defaultParticle.rp_transform.pt_position.y, sliderSpeed, 0.0f, p_layer.view_size.y);
					ImGui::Separator();
					ImGui::DragFloat("Size X", &defaultParticle.rp_transform.pt_size.x, sliderSpeed, 1.0f, p_layer.view_size.x);
					ImGui::DragFloat("Size Y", &defaultParticle.rp_transform.pt_size.y, sliderSpeed, 1.0f, p_layer.view_size.y);
					ImGui::Separator();
					ImGui::DragFloat("Rotation", &defaultParticle.rp_transform.p_rotation.x, sliderSpeed, 0.0f, 360.0f);


					ImGui::Separator();
					ImGui::TreePop();
					ImGui::Spacing();
				}

				if (ImGui::TreeNode("Particle Animation"))
				{
					if (is_active) {
						ImGui::DragFloat2("Velocity", (float*)&defaultParticle.rp_animation.p_velocity, sliderSpeed, -10.0f, 10.0f);
					}

					ImGui::Separator();
					ImGui::TreePop();
					ImGui::Spacing();
				}

				if (ImGui::TreeNode("Particle Color"))
				{
					if (is_active) {
						ImGui::ColorPicker4("Start Color", (float*)&defaultParticle.rp_colorData.p_startColor);
						ImGui::ColorPicker4("End Color", (float*)&defaultParticle.rp_colorData.p_endColor);
					}

					ImGui::Separator();
					ImGui::TreePop();
					ImGui::Spacing();
				}

				if (ImGui::TreeNode("Particle Lifetime"))
				{
					if (is_active) {
						ImGui::DragFloat("Max Lifetime", &defaultParticle.rp_lifetimeData.p_maxLifetime, sliderSpeed, 0.1f, 5.0f);
					}

					ImGui::Separator();
					ImGui::TreePop();
					ImGui::Spacing();
				}

				ImGui::Separator();
				ImGui::TreePop();
				ImGui::Spacing();
				p_layer.defaultParticle = defaultParticle;
				p_layer.useDefaultParticle = useDefaultParticle;
			}

			if (ImGui::TreeNode("Background Particle")) {



				ImGui::Separator();
				ImGui::TreePop();
				ImGui::Spacing();
			}

			if (ImGui::TreeNode("Bounds")) {

				if (is_active) {

				}


				if (ImGui::TreeNode("Bounds Shape"))
				{

					ImGui::Separator();
					ImGui::TreePop();
					ImGui::Spacing();
				}

				ImGui::TreePop();
				ImGui::Spacing();
			}
			p_layer.defaultParticle = defaultParticle;

			ImGui::End();
		}

		virtual void OnUpdate(float ts) override {
			p_layer.OnUpdate(ts);
		}
	};

}