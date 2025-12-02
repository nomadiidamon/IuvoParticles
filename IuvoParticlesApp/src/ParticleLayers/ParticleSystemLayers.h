#pragma once
#include "Walnut/Application.h"
#include "Walnut/Image.h"
#include "../Particles/RectParticle/RectParticleUtils.h"
#include "../Particles/RectParticle/RectParticleSystem/RectParticleEmitter.h"
#include "LayerConfig.h"
#include <fstream>

namespace Particles {

	class IMGUI_2D_PARTICLE_LAYER : public Walnut::Layer
	{
	public:
		ImDrawList* draw_list;
		LayerConfig& l_cfg = GetLayerConfig();
		ParticleConfig& p_cfg = GetParticleConfig();
		std::vector<RectParticle> particles;

		Walnut::Image* particleTexture = nullptr;
		std::string particleTexturePath;
		bool initialized = false;


		// helper: clamp
		template<typename T>
		static T Clamp(T v, T lo, T hi) { if (v < lo) return lo; if (v > hi) return hi; return v; }

		// helper: emit N particles at position in view-local coords
		void EmitAt(const ImVec2& pos, int count)
		{
			if (count <= 0) return;
			int allowed = l_cfg.maxParticleCount - (int)particles.size();
			int createCount = std::min(count, std::max(0, allowed));
			if (createCount <= 0) return;
			particles.reserve(particles.size() + createCount);

			for (int i = 0; i < createCount; ++i) {
				RectParticle p;
				if (l_cfg.useDefaultParticle) {
					p = RectParticleUtils::CreateParticle(
						l_cfg.defaultParticle.transform,
						l_cfg.defaultParticle.color,
						l_cfg.defaultParticle.lifetime
					);
				}
				else {
					p = RectParticleUtils::CreateRandomParticle(l_cfg.view_size);
				}
				p.transform.position = pos;
				particles.push_back(std::move(p));
			}
		}

		void ClearParticles()
		{
			particles.clear();
		}

		void HandleHoverBurst(float ts)
		{
			if (!l_cfg.emitOnHover) return;
			if (l_cfg.isHovering) {
				l_cfg.hoverBurstTimer += ts;
				if (l_cfg.hoverBurstTimer >= l_cfg.hoverBurstInterval) {
					l_cfg.hoverBurstTimer = 0.0f;
					EmitAt(l_cfg.mouse_pos, l_cfg.hoverBurstCount);
				}
			}
		}

		/// TODO: convert this function to a RectParticleUtils func
		// Click handling — now uses EmitAt and velocity radial burst logic optionally
		void HandleClick()
		{
			if (!l_cfg.wasClicked) return;

			// Create a radial burst of particles exploding from the clicked point
			const int burstCount = 100;
			int allowed = l_cfg.maxParticleCount - (int)particles.size();
			int createCount = std::min(burstCount, std::max(0, allowed));
			particles.reserve(particles.size() + createCount);
			l_cfg.wasClicked = false;

			for (int i = 0; i < createCount; ++i) {
				RectParticle p = RectParticleUtils::CreateParticle(
					l_cfg.defaultParticle.transform,
					l_cfg.defaultParticle.color,
					l_cfg.defaultParticle.lifetime
				);

				p.transform.position = l_cfg.lastClickPos;

				// Random radial velocity
				float angle = (static_cast<float>(rand()) / RAND_MAX) * 2.0f * 3.14159265358979323846f;
				float minSpeed = 50.0f;
				float maxSpeed = 300.0f;
				float speed = minSpeed + (static_cast<float>(rand()) / RAND_MAX) * (maxSpeed - minSpeed);
				p.animation.p_velocity = ImVec2(cosf(angle), sinf(angle));
				p.animation.p_movementSpeed = speed;
				p.animation.canMove = true;

				// random lifetime around base
				float baseLifetime = p.lifetime.maxLifetime > 0.0f ? p.lifetime.maxLifetime : 1.0f;
				float randLifetime = baseLifetime * (0.75f + (static_cast<float>(rand()) / RAND_MAX) * 0.75f);
				p.lifetime.maxLifetime = randLifetime;
				p.lifetime.lifetime = 0.0f;

				if (p.transform.randomRotation) {
					p.transform.rotation.x = (static_cast<float>(rand()) / RAND_MAX) * 360.0f;
				}

				particles.push_back(std::move(p));
			}


			l_cfg.currentParticleCount = (int)particles.size();
		}

		// minimal preset save/load (very simple CSV-like)
		bool SavePreset(const std::string& filename)
		{
			std::ofstream ofs(filename, std::ios::out);
			if (!ofs.good()) return false;
			// save some fields (expand as needed)
			ofs << "useDefaultParticle " << (l_cfg.useDefaultParticle ? 1 : 0) << "\n";
			ofs << "default_count " << l_cfg.defaultParticleCount << "\n";
			ofs << "max_count " << l_cfg.maxParticleCount << "\n";
			ofs << "hover_count " << l_cfg.hoverBurstCount << "\n";
			ofs << "hover_interval " << l_cfg.hoverBurstInterval << "\n";
			ofs << "gravity " << l_cfg.gravity.x << " " << l_cfg.gravity.y << "\n";
			ofs << "drag " << l_cfg.drag << "\n";
			// particle color example
			ofs << "start_color " << l_cfg.defaultParticle.color.startColor.x << " " << l_cfg.defaultParticle.color.startColor.y << " " << l_cfg.defaultParticle.color.startColor.z << " " << l_cfg.defaultParticle.color.startColor.w << "\n";
			ofs.close();
			return true;
		}

		bool LoadPreset(const std::string& filename)
		{
			std::ifstream ifs(filename, std::ios::in);
			if (!ifs.good()) return false;
			std::string key;
			while (ifs >> key) {
				if (key == "useDefaultParticle") { int v; ifs >> v; l_cfg.useDefaultParticle = (v != 0); }
				else if (key == "default_count") { ifs >> l_cfg.defaultParticleCount; }
				else if (key == "max_count") { ifs >> l_cfg.maxParticleCount; }
				else if (key == "hover_count") { ifs >> l_cfg.hoverBurstCount; }
				else if (key == "hover_interval") { ifs >> l_cfg.hoverBurstInterval; }
				else if (key == "gravity") { ifs >> l_cfg.gravity.x >> l_cfg.gravity.y; }
				else if (key == "drag") { ifs >> l_cfg.drag; }
				else if (key == "start_color") { ifs >> l_cfg.defaultParticle.color.startColor.x >> l_cfg.defaultParticle.color.startColor.y >> l_cfg.defaultParticle.color.startColor.z >> l_cfg.defaultParticle.color.startColor.w; }
				else {
					// unknown key: try skip line
					std::string rest;
					std::getline(ifs, rest);
				}
			}
			ifs.close();
			return true;
		}

		virtual void OnAttach() override {
			// If view_size is not initialized yet, use a reasonable fallback.
			ImVec2 initialSize = l_cfg.view_size;
			if (initialSize.x <= 0.0f || initialSize.y <= 0.0f) {
				initialSize = l_cfg.min_view_size;
				l_cfg.view_size = initialSize;
			}
		}

		// OnUIRender: keep most existing rendering & add new controls
		virtual void OnUIRender() override
		{
			// if empty, populate initial set (preserve old behavior)
			if (particles.empty()) {

				if (!initialized) {
					Initialize();
				}
			}

			// Main window
			if (ImGui::Begin("2D Particle Layer")) {
				draw_list = ImGui::GetWindowDrawList();
				ImVec2 win_pos = ImGui::GetWindowPos();
				ImVec2 win_size = ImGui::GetWindowSize();
				l_cfg.view_pos = win_pos;
				l_cfg.view_size = win_size;
				if (l_cfg.view_size.x < l_cfg.min_view_size.x) l_cfg.view_size.x = l_cfg.min_view_size.x;
				if (l_cfg.view_size.y < l_cfg.min_view_size.y) l_cfg.view_size.y = l_cfg.min_view_size.y;

				ImGui::InvisibleButton("viewport", GetLayerConfig().view_size);
				if (ImGui::IsItemHovered()) {
					l_cfg.isHovering = true;
					l_cfg.mouse_pos = ImVec2(ImGui::GetIO().MousePos.x - l_cfg.view_pos.x, ImGui::GetIO().MousePos.y - l_cfg.view_pos.y);

					if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
						l_cfg.wasClicked = true;
						l_cfg.lastClickPos = l_cfg.mouse_pos;
					}

					// Right click menu for quick actions
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
						ImGui::OpenPopup("viewport_context");
					}
					if (ImGui::BeginPopup("viewport_context")) {
						if (ImGui::MenuItem("Emit 10 at cursor")) { EmitAt(l_cfg.mouse_pos, 10); }
						if (ImGui::MenuItem("Emit 100 at cursor")) { EmitAt(l_cfg.mouse_pos, 100); }
						if (ImGui::MenuItem("Clear Particles")) { ClearParticles(); }
						ImGui::EndPopup();
					}
				}
				else {
					l_cfg.isHovering = false;
				}

				// Background color rect
				if (l_cfg.backgroundParticle.bg_lifetimeData.maxLifetime <= 0.0f) {
					l_cfg.backgroundParticle.bg_lifetimeData.maxLifetime = 1.0f;
				}
				if (l_cfg.backgroundParticle.bg_lifetimeData.lifetime >= l_cfg.backgroundParticle.bg_lifetimeData.maxLifetime) {
					l_cfg.backgroundParticle.bg_lifetimeData.lifetime = 0.0f;
					l_cfg.backgroundParticle.bg_colorData.currColor = l_cfg.backgroundParticle.bg_colorData.startColor;
				}
				float bg_lifeRatio = l_cfg.backgroundParticle.bg_lifetimeData.lifetime / l_cfg.backgroundParticle.bg_lifetimeData.maxLifetime;
				bg_lifeRatio = std::clamp(bg_lifeRatio, 0.0f, 1.0f);
				l_cfg.backgroundParticle.bg_colorData.currColor = ImVec4(
					l_cfg.backgroundParticle.bg_colorData.startColor.x + (l_cfg.backgroundParticle.bg_colorData.endColor.x - l_cfg.backgroundParticle.bg_colorData.startColor.x) * bg_lifeRatio,
					l_cfg.backgroundParticle.bg_colorData.startColor.y + (l_cfg.backgroundParticle.bg_colorData.endColor.y - l_cfg.backgroundParticle.bg_colorData.startColor.y) * bg_lifeRatio,
					l_cfg.backgroundParticle.bg_colorData.startColor.z + (l_cfg.backgroundParticle.bg_colorData.endColor.z - l_cfg.backgroundParticle.bg_colorData.startColor.z) * bg_lifeRatio,
					1.0f);

				draw_list->AddRectFilled(l_cfg.view_pos, ImVec2(l_cfg.view_pos.x + l_cfg.view_size.x, l_cfg.view_pos.y + l_cfg.view_size.y),
					ImGui::GetColorU32(l_cfg.backgroundParticle.bg_colorData.currColor));

				// draw each particle (respecting rotation)
				for (auto& particle : particles) {
					if (l_cfg.randomizeEverything) {
						auto& pos = particle.transform.position;
						particle = RectParticleUtils::CreateRandomParticle(l_cfg.view_size, pos);
						RectParticleUtils::DrawParticleAgnostic(draw_list, particle, l_cfg.view_pos);
						continue;
					}
					//if (particle.rp_animation.canRotate) {
					//	RectParticleUtils::DrawRotatingParticle(draw_list, particle, l_cfg.view_pos);
					//}
					//else {
					//	RectParticleUtils::DrawParticle(draw_list, particle, l_cfg.view_pos);
					//}
					RectParticleUtils::DrawParticleAgnostic(draw_list, particle, l_cfg.view_pos);

				}
				RectParticleUtils::RestrictParticles(particles, l_cfg.maxParticleCount, l_cfg.view_size);
			}

			ImGui::End();
		}

		void Initialize() {
			if (initialized) return;
			particles.clear();


			if (l_cfg.useDefaultParticle) {
				particles.reserve(l_cfg.defaultParticleCount);
				for (int i = 0; i < l_cfg.defaultParticleCount; ++i) {
					particles.push_back(RectParticleUtils::CreateParticle(
						l_cfg.defaultParticle.transform,
						l_cfg.defaultParticle.color,
						l_cfg.defaultParticle.lifetime
					));
				}
			}
			else {
				particles.reserve(l_cfg.defaultParticleCount);
				for (int i = 0; i < l_cfg.defaultParticleCount; ++i) {
					particles.push_back(RectParticleUtils::CreateRandomParticle(l_cfg.view_size, l_cfg.defaultParticle.transform.position));
				}
			}

			l_cfg.backgroundParticle.bg_colorData.startColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
			l_cfg.backgroundParticle.bg_colorData.endColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
			initialized = true;

		}

		// central update respects pause, applies forces, updates emitter and particles
		virtual void OnUpdate(float ts) override
		{
			// apply global speed multiplier & paused/step logic
			float simTs = ts * l_cfg.globalSpeedMultiplier;
			if (l_cfg.paused && !l_cfg.stepOnce) {

				/// TODO: add bg update even when paused?
				l_cfg.backgroundParticle.bg_lifetimeData.lifetime += ts;
				if (l_cfg.backgroundParticle.bg_lifetimeData.maxLifetime <= 0.0f)
					l_cfg.backgroundParticle.bg_lifetimeData.maxLifetime = 1.0f;
				float bg_lifeRatio = l_cfg.backgroundParticle.bg_lifetimeData.lifetime / l_cfg.backgroundParticle.bg_lifetimeData.maxLifetime;
				bg_lifeRatio = std::clamp(bg_lifeRatio, 0.0f, 1.0f);
				l_cfg.backgroundParticle.bg_colorData.currColor = ImVec4(
					l_cfg.backgroundParticle.bg_colorData.startColor.x + (l_cfg.backgroundParticle.bg_colorData.endColor.x - l_cfg.backgroundParticle.bg_colorData.startColor.x) * bg_lifeRatio,
					l_cfg.backgroundParticle.bg_colorData.startColor.y + (l_cfg.backgroundParticle.bg_colorData.endColor.y - l_cfg.backgroundParticle.bg_colorData.startColor.y) * bg_lifeRatio,
					l_cfg.backgroundParticle.bg_colorData.startColor.z + (l_cfg.backgroundParticle.bg_colorData.endColor.z - l_cfg.backgroundParticle.bg_colorData.startColor.z) * bg_lifeRatio,
					1.0f);
				// consume a single step if requested, then return
				if (l_cfg.stepOnce) {
					l_cfg.stepOnce = false;
				}
				else {
					return;
				}
			}

			// Update particles: apply gravity and drag, then call existing UpdateParticle
			for (auto& particle : particles) {
				if (!&particle) continue;

				// apply gravity (as acceleration) to the velocity vector
				if (particle.animation.canMove) {
					// convert to world velocity vector
					ImVec2 v = ImVec2(particle.animation.p_velocity.x * particle.animation.p_movementSpeed,
						particle.animation.p_velocity.y * particle.animation.p_movementSpeed);

					// acceleration
					v.x += l_cfg.gravity.x * simTs;
					v.y += l_cfg.gravity.y * simTs;

					// apply a simple drag (scale down speed)
					float dragFactor = Clamp(1.0f - l_cfg.drag * simTs, 0.0f, 1.0f);
					v.x *= dragFactor;
					v.y *= dragFactor;

					// convert back into direction + magnitude
					float speed = sqrtf(v.x * v.x + v.y * v.y);
					if (speed > 0.0001f) {
						particle.animation.p_velocity = ImVec2(v.x / speed, v.y / speed);
						particle.animation.p_movementSpeed = speed;
					}
					else {
						particle.animation.p_movementSpeed = 0.0f;
					}
				}
				// call your utility that updates lifetime, transform, rotation, color etc.
				RectParticleUtils::UpdateParticle(particle, simTs, l_cfg.view_size, particles, l_cfg.useDefaultParticle, &l_cfg.defaultParticle);
			}

			// background anim
			l_cfg.backgroundParticle.bg_lifetimeData.lifetime += ts;
			if (l_cfg.backgroundParticle.bg_lifetimeData.maxLifetime <= 0.0f)
				l_cfg.backgroundParticle.bg_lifetimeData.maxLifetime = 1.0f;
			float bg_lifeRatio = l_cfg.backgroundParticle.bg_lifetimeData.lifetime / l_cfg.backgroundParticle.bg_lifetimeData.maxLifetime;
			bg_lifeRatio = std::clamp(bg_lifeRatio, 0.0f, 1.0f);
			l_cfg.backgroundParticle.bg_colorData.currColor = ImVec4(
				l_cfg.backgroundParticle.bg_colorData.startColor.x + (l_cfg.backgroundParticle.bg_colorData.endColor.x - l_cfg.backgroundParticle.bg_colorData.startColor.x) * bg_lifeRatio,
				l_cfg.backgroundParticle.bg_colorData.startColor.y + (l_cfg.backgroundParticle.bg_colorData.endColor.y - l_cfg.backgroundParticle.bg_colorData.startColor.y) * bg_lifeRatio,
				l_cfg.backgroundParticle.bg_colorData.startColor.z + (l_cfg.backgroundParticle.bg_colorData.endColor.z - l_cfg.backgroundParticle.bg_colorData.startColor.z) * bg_lifeRatio,
				1.0f);

			HandleHoverBurst(simTs);
			HandleClick();

			// limit particles
			RectParticleUtils::RestrictParticles(particles, l_cfg.maxParticleCount, l_cfg.view_size);

			// Step mode consumes once
			if (l_cfg.stepOnce) {
				l_cfg.stepOnce = false;
				l_cfg.paused = true;
			}
		}
	};

	class IMGUI_2D_PARTICLE_LAYER_PROPERTIES : public Walnut::Layer
	{
	public:

		LayerConfig& l_cfg = GetLayerConfig();
		ParticleConfig& p_cfg = GetParticleConfig();


		IMGUI_2D_PARTICLE_LAYER* p_layer = nullptr;
		bool is_active = false;

		float readyTimer = 0.25f;
		bool startEventTriggered = false;

		void SetParticleLayer(IMGUI_2D_PARTICLE_LAYER* layer) {
			p_layer = layer;
		}

		void SetupDefaultParticle() {
			if (!p_layer)
				return;

			// Center of Viewport
			l_cfg.defaultParticle.transform.position = ImVec2(l_cfg.view_size.x / 2.0f, l_cfg.view_size.y / 2.0f);
			p_cfg.transform.defaultPosition = l_cfg.defaultParticle.transform.position;
			// Size 10x10
			l_cfg.defaultParticle.transform.size = ImVec2(10.0f, 10.0f);
			// No Size Randomization
			l_cfg.defaultParticle.transform.randomSize = false;
			// No Rotation			
			l_cfg.defaultParticle.transform.randomRotation = false;

			// Downward Velocity
			l_cfg.defaultParticle.animation.p_velocity = ImVec2(-1.0f, -3.0f);
			// Moderate Speed			
			l_cfg.defaultParticle.animation.p_movementSpeed = 50.0f;
			// Enable Movement
			l_cfg.defaultParticle.animation.canMove = true;

			// Start Color Red
			l_cfg.defaultParticle.color.startColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
			// End Color Yellow
			l_cfg.defaultParticle.color.endColor = ImVec4(1.0f, 1.0f, 0.0f, 0.0f);
			// Lerp Color Enabled
			l_cfg.defaultParticle.color.lerpColor = true;
			// Use Alpha Channel
			l_cfg.defaultParticle.color.useAlpha = true;
			// Lifetime 2 Seconds
			l_cfg.defaultParticle.lifetime.maxLifetime = 2.0f;
			// No Lifetime Randomization
			l_cfg.defaultParticle.lifetime.randomizeLifetime = false;
		}

		virtual void OnAttach() override {
			SetupDefaultParticle();

			if (p_layer) {

			}
		}

		virtual void OnUIRender() override {
			if (!p_layer)
				return;


			ImGui::Begin("2D Particle Details");
			ImVec2 max = ImVec2(ImGui::GetWindowContentRegionMax().x * 2.0f, ImGui::GetWindowContentRegionMax().y * 2.0f);
			ImVec2 min = ImGui::GetWindowPos();
			ImVec2 propMax = ImVec2(min.x + max.x, min.y + max.y);
			is_active = ImGui::IsMouseHoveringRect(ImGui::GetWindowPos(), max);
			if (is_active)
			{
				ImGui::Text("Simulation");
				ImGui::SameLine();
				if (ImGui::Button(l_cfg.paused ? "Resume" : "Pause")) {
					l_cfg.paused = !l_cfg.paused;
				}
				ImGui::SameLine();
				if (ImGui::Button("Step")) {
					l_cfg.stepOnce = true;
					l_cfg.paused = false;
				}
				ImGui::SameLine();
				if (ImGui::Button("Clear All")) {
					p_layer->ClearParticles();
				}
				ImGui::Separator();

				if (ImGui::TreeNode("Background Particle")) {

					ImGui::ColorEdit4("Start Color", (float*)&l_cfg.backgroundParticle.bg_colorData.startColor);
					ImGui::ColorEdit4("End Color", (float*)&l_cfg.backgroundParticle.bg_colorData.endColor);
					ImGui::Checkbox("Lerp Color", &l_cfg.backgroundParticle.bg_colorData.lerpColor);
					ImGui::DragFloat("Lerp Speed", &l_cfg.backgroundParticle.bg_colorData.lerpSpeed, l_cfg.drag, 0.0f, 5.0f);

					ImGui::DragFloat("Max Lifetime", &l_cfg.backgroundParticle.bg_lifetimeData.maxLifetime, l_cfg.drag, 0.01f, 60.0f);
					ImGui::Checkbox("Randomize Lifetime", &l_cfg.backgroundParticle.bg_lifetimeData.randomizeLifetime);

					ImGui::Separator();
					ImGui::TreePop();
					ImGui::Spacing();
				}

				if (ImGui::TreeNode("Global Particle Config")) {
					if (ImGui::TreeNode("Physics")) {
						ImGui::DragFloat2("Gravity", (float*)&l_cfg.gravity, 1.0f, -2000.0f, 2000.0f);
						ImGui::DragFloat("Drag (damping)", &l_cfg.drag, 0.01f, 0.0f, 5.0f);
						ImGui::TreePop();
					}

					if (ImGui::TreeNode("Transform Ranges")) {

						ImGui::TreePop();
					}
					ImGui::TreePop();
				}
				ImGui::Separator();

				// Speed multiplier
				ImGui::DragFloat("Global Speed Multiplier", &l_cfg.globalSpeedMultiplier, 0.1f, 0.0f, 20.0f);

				// Particle counts
				ImGui::DragInt("Default Particle Count", &l_cfg.defaultParticleCount, 1, 0, 5000);
				ImGui::DragInt("Max Particle Count (layer)", &l_cfg.maxParticleCount, 1, 0, 10000);
				ImGui::Separator();

				if (ImGui::TreeNode("Hover & Click")) {
					ImGui::Checkbox("Emit On Hover", &l_cfg.emitOnHover);
					ImGui::DragInt("Hover Burst Count", &l_cfg.hoverBurstCount, 1, 0, 1000);
					ImGui::DragFloat("Hover Burst Interval", &l_cfg.hoverBurstInterval, 0.001f, 0.001f, 1.0f);
					ImGui::Separator();
					if (ImGui::Button("Emit 50 at center")) {
						p_layer->EmitAt(ImVec2(l_cfg.view_size.x / 2.0f, l_cfg.view_size.y / 2.0f), 50);
					}

					ImGui::TreePop();
				}
				ImGui::Separator();

			}

			if (ImGui::TreeNode("Default Particle")) {
				if (ImGui::Checkbox("Use Default Particle", &l_cfg.useDefaultParticle)) {
					SetupDefaultParticle();
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Randomize Everything", &l_cfg.randomizeEverything)) {

				}
				if (ImGui::Button("Random Default Particle")) {
					auto pos = l_cfg.defaultParticle.transform.position;
					l_cfg.defaultParticle.transform = ParticleTransform::CreateRandomTransform();
					l_cfg.defaultParticle.transform.position = pos;
					l_cfg.defaultParticle.animation = ParticleAnimation2D::CreateRandomAnimation2D();
					l_cfg.defaultParticle.color = ParticleColor::CreateRandomColor();
					l_cfg.defaultParticle.lifetime = ParticleLifetime::CreateRandomLifetime();
				}
				if (ImGui::Combo("Full Particle Presets", (int*)&l_cfg.rectParticlePreset, "None\0Red To Yellow Fade\0Red To Transparent Fade\0Yellow To Green Fade\0Yellow To Transparent Fade\0Blue To Cyan Fade\0Blue To Transparent Fade\0Fire Fade\0Smoke Puff\0Spark Ember\0Explosion Debris\0Rainbow Cycle\0Rain Streak\0\0")) {
					switch (l_cfg.rectParticlePreset) {
					case RectParticlePreset::RED_TO_YELLOW_FADE_OUT:
						l_cfg.ApplyParticlePreset(RED_TO_YELLOW_FADE_OUT);
						break;
					case RectParticlePreset::RED_TO_TRANSPARENT_FADE_OUT:
						l_cfg.ApplyParticlePreset(RED_TO_TRANSPARENT_FADE_OUT);
						break;
					case RectParticlePreset::YELLOW_TO_GREEN_FADE_OUT:
						l_cfg.ApplyParticlePreset(YELLOW_TO_GREEN_FADE_OUT);
						break;
					case RectParticlePreset::YELLOW_TO_TRANSPARENT_FADE_OUT:
						l_cfg.ApplyParticlePreset(YELLOW_TO_TRANSPARENT_FADE_OUT);
						break;
					case RectParticlePreset::BLUE_TO_CYAN_FADE_OUT:
						l_cfg.ApplyParticlePreset(BLUE_TO_CYAN_FADE_OUT);
						break;
					case RectParticlePreset::BLUE_TO_TRANSPARENT_FADE_OUT:
						l_cfg.ApplyParticlePreset(BLUE_TO_TRANSPARENT_FADE_OUT);
						break;
					case RectParticlePreset::FIRE_FADE_OUT:
						l_cfg.ApplyParticlePreset(FIRE_FADE_OUT);
						break;
					case RectParticlePreset::SMOKE_PUFF:
						l_cfg.ApplyParticlePreset(SMOKE_PUFF);
						break;
					case RectParticlePreset::SPARK_EMBER:
						l_cfg.ApplyParticlePreset(SPARK_EMBER);
						break;
					case RectParticlePreset::EXPLOSION_DEBRIS:
						l_cfg.ApplyParticlePreset(EXPLOSION_DEBRIS);
						break;
					case RectParticlePreset::RAINBOW_CYCLE:
						l_cfg.ApplyParticlePreset(RAINBOW_CYCLE);
						break;
					case RectParticlePreset::RAIN_STREAK:
						l_cfg.ApplyParticlePreset(RAIN_STREAK);
						break;
					case RectParticlePreset::NONE:
					default:
						// do nothing
						break;
					}
				}
				ImGui::Separator();

				if (ImGui::TreeNode("Transform")) {
					if (ImGui::Button("Random Transform")) {
						l_cfg.defaultParticle.transform = ParticleTransform::CreateRandomTransform();
					}
					ImGui::SameLine();
					if (ImGui::Button("Center in Viewport"))
					{
						l_cfg.defaultParticle.transform.position = ImVec2(l_cfg.view_size.x / 2.0f, l_cfg.view_size.y / 2.0f);
					}
					if (ImGui::Combo("Transform Preset", (int*)&l_cfg.transformPreset, "None\0Small Floating\0Explosion Chunks\0\0")) {
						switch (l_cfg.transformPreset) {
						case ParticleTransformPreset::SMALL_FLOATING:
							l_cfg.ApplyTransformPreset(ParticleTransform::SMALL_FLOATING);
							break;
						case ParticleTransformPreset::EXPLOSION_CHUNK:
							l_cfg.ApplyTransformPreset(ParticleTransform::EXPLOSION_CHUNK);
							break;
						case ParticleTransformPreset::NONE:
						default:
							// do nothing
							break;
						}
					}

					ImGui::DragFloat2("Position", (float*)&l_cfg.defaultParticle.transform.position, l_cfg.drag, 0.0f, l_cfg.view_size.x);
					ImGui::DragFloat2("Size", (float*)&l_cfg.defaultParticle.transform.size, l_cfg.drag, 1.0f, l_cfg.view_size.x);
					ImGui::Checkbox("Random Size", &l_cfg.defaultParticle.transform.randomSize);
					ImGui::Checkbox("Random Rotation", &l_cfg.defaultParticle.transform.randomRotation);
					ImGui::DragFloat("Rotation", &l_cfg.defaultParticle.transform.rotation.x, 1.0f, 0.0f, 360.0f);
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Animation")) {
					if (ImGui::Button("Random Animation")) {
						l_cfg.defaultParticle.animation = ParticleAnimation2D::CreateRandomAnimation2D();
					}
					if (ImGui::Combo("Animation Preset", (int*)&l_cfg.animationPreset, "None\0Small Rising\0Explosion\0Drifting\0Spinning\0Lightweight Drifting\0\0")) {
						switch (l_cfg.animationPreset) {
						case ParticleAnimation2DPreset::SMALL_RISING:
							l_cfg.ApplyAnimationPreset(ParticleAnimation2D::SMALL_RISING);
							break;
						case ParticleAnimation2DPreset::EXPLOSION:
							l_cfg.ApplyAnimationPreset(ParticleAnimation2D::EXPLOSION);
							break;
						case ParticleAnimation2DPreset::DRIFTING:
							l_cfg.ApplyAnimationPreset(ParticleAnimation2D::DRIFTING);
							break;
						case ParticleAnimation2DPreset::SPINNING:
							l_cfg.ApplyAnimationPreset(ParticleAnimation2D::SPINNING);
							break;
						case ParticleAnimation2DPreset::LIGHTWIEGHT_DRIFT:
							l_cfg.ApplyAnimationPreset(ParticleAnimation2D::LIGHTWIEGHT_DRIFT);
							break;
						case ParticleAnimation2DPreset::NONE:
						default:
							// do nothing
							break;
						}
					}
					ImGui::DragFloat2("Velocity Dir", (float*)&l_cfg.defaultParticle.animation.p_velocity, l_cfg.drag, -10.0f, 10.0f);
					ImGui::DragFloat("Speed", &l_cfg.defaultParticle.animation.p_movementSpeed, l_cfg.drag, 0.0f, 2000.0f);
					ImGui::Checkbox("Can Move", &l_cfg.defaultParticle.animation.canMove);
					ImGui::Checkbox("Can Rotate", &l_cfg.defaultParticle.animation.canRotate);
					ImGui::DragFloat("Rotation Speed", &l_cfg.defaultParticle.animation.p_rotationSpeed, 0.1f, -1000.0f, 1000.0f);
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Color")) {
					if (ImGui::Button("Random Color")) {
						l_cfg.defaultParticle.color = ParticleColor::CreateRandomColor();
					}
					ImGui::SameLine();
					if (ImGui::Button("Random Start Color")) {
						l_cfg.defaultParticle.color.startColor = ParticleColor::CreateRandomColor().startColor;
					}
					ImGui::SameLine();
					if (ImGui::Button("Random End Color")) {
						l_cfg.defaultParticle.color.endColor = ParticleColor::CreateRandomColor().endColor;
					}
					if (ImGui::Combo("Color Preset", (int*)&l_cfg.colorPreset, "None\0Red to Yellow Fade\0Red to Blue Fade\0Red to Transparent Fade\0Orange to Yellow Fade\0Orange to Transparent Fade\0Yellow to Green Fade\0Yellow to Transparent Fade\0Blue to Cyan Fade\0Blue to Transparent Fade\0Fire Fade Out\0Ice Fade Out\0Toxic Fade Out\0Magic Purple Fade Out\0\0")) {
						RectParticleUtils::ApplyColorPreset(l_cfg.defaultParticle, l_cfg.colorPreset);
					}
					ImGui::ColorEdit4("Start Color", (float*)&l_cfg.defaultParticle.color.startColor);
					ImGui::ColorEdit4("End Color", (float*)&l_cfg.defaultParticle.color.endColor);
					ImGui::Checkbox("Lerp Color", &l_cfg.defaultParticle.color.lerpColor);
					ImGui::DragFloat("Lerp Speed", &l_cfg.defaultParticle.color.lerpSpeed, l_cfg.drag, 0.0f, 5.0f);
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Lifetime")) {
					if (ImGui::Button("Random Lifetime")) {
						l_cfg.defaultParticle.lifetime = ParticleLifetime::CreateRandomLifetime();
					}
					if (ImGui::Combo("Lifetime Preset", (int*)&l_cfg.lifetimePreset, "None\0Short Burst\0Medium Burst\0Long Burst\0Long Floating\0\0")) {
						switch (l_cfg.lifetimePreset) {
						case ParticleLifetimePreset::SHORT_BURST:
							l_cfg.ApplyLifetimePreset(ParticleLifetime::SHORT_BURST);
							break;
						case ParticleLifetimePreset::MEDIUM_BURST:
							l_cfg.ApplyLifetimePreset(ParticleLifetime::MEDIUM_BURST);
							break;
						case ParticleLifetimePreset::LONG_BURST:
							l_cfg.ApplyLifetimePreset(ParticleLifetime::LONG_BURST);
							break;
						case ParticleLifetimePreset::LONG_FLOATING:
							l_cfg.ApplyLifetimePreset(ParticleLifetime::LONG_FLOATING);
							break;
						case ParticleLifetimePreset::NONE:
						default:
							// do nothing
							break;
						}
					}
					ImGui::DragFloat("Max Lifetime", &l_cfg.defaultParticle.lifetime.maxLifetime, 0.01f, 0.01f, 60.0f);
					ImGui::Checkbox("Randomize Lifetime", &l_cfg.defaultParticle.lifetime.randomizeLifetime);
					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Separator();


			// Preset save/load (simple)
			if (ImGui::Button("Save Preset")) {
				std::string fn = "particles_preset.txt";
				bool ok = p_layer->SavePreset(fn);
				(void)ok;
			}
			ImGui::SameLine();
			if (ImGui::Button("Load Preset")) {
				std::string fn = "particles_preset.txt";
				bool ok = p_layer->LoadPreset(fn);
				(void)ok;
			}

			ImGui::End();
		}

		virtual void OnUpdate(float ts) override {

			// Trigger a click event at the center of the viewport after readyTimer elapses
			// ensures the particle pool is initialized
			/// TODO: figure out cause of no auto emit on first run. Shouldnt need trigger event
			if (!startEventTriggered) {
				readyTimer -= ts;
				if (readyTimer <= 0.0f) {
					l_cfg.lastClickPos = ImVec2(l_cfg.view_size.x / 2.0f, l_cfg.view_size.y / 2.0f);
					l_cfg.wasClicked = true;
					startEventTriggered = true;
				}
			}

			if (l_cfg.emitOnHover && l_cfg.isHovering) {
				p_layer->EmitAt(l_cfg.mouse_pos, l_cfg.hoverBurstCount);
			}

		}
	};

}