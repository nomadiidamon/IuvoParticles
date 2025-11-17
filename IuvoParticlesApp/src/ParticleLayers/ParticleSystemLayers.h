#pragma once
#include "Walnut/Application.h"
#include "Walnut/Image.h"
#include "../Particles/RectParticle/RectParticleUtils.h"
#include "../Particles/RectParticle/RectParticleSystem/RectParticleEmitter.h"
#include <fstream>

namespace Particles {

	class IMGUI_2D_PARTICLE_LAYER : public Walnut::Layer
	{
	public:
		ImDrawList* draw_list;
		ImVec2 view_pos = ImVec2(0.0f, 0.0f);
		ImVec2 view_size = ImVec2(800.0f, 600.0f);
		ImVec2 min_view_size = ImVec2(200, 200);
		ImVec2 mouse_pos = ImVec2(0.0f, 0.0f);
		ImVec2 lastClickPos = ImVec2(0.0f, 0.0f);

		std::vector<RectParticle> particles;
		int defaultParticleCount = 1000;
		int maxParticleCount = 100000;
		int psMaxParticles = 100000;

		// hover / click
		bool is_hovered = false;
		bool emit_on_hover = false;
		bool was_clicked = false;
		int hoverBurstCount = 25;
		float hoverBurstInterval = 0.05f;
		float hoverBurstTimer = 0.0f;

		// global runtime controls
		bool paused = false;
		bool stepOnce = false;
		float globalSpeedMultiplier = 1.0f;
		bool initialized = false;

		// physics
		ImVec2 gravity = ImVec2(0.0f, 200.0f); // px / s^2 downward by default (positive Y down)
		float drag = 0.1f; // simple damping per second (0=no drag, 1=stop instantly)

		// blending / visuals
		BlendMode blendMode = BlendMode::Alpha;

		RectParticle defaultParticle;
		BackgroundParticle bg_particle;
		RectParticleEmitter rp_emitter;
		bool useDefaultParticle = true;


		// emitter UI/runtime
		bool emitterAutoEmit = true;
		EmitterShape emitterShape = EmitterShape::Circle;
		float emitterSpawnRadius = 0.3f; // meaningful when circle shape selected
		int emitterManualEmitCount = 10;

		Walnut::Image* particleTexture = nullptr;
		std::string particleTexturePath;

		// helper: clamp
		template<typename T>
		static T Clamp(T v, T lo, T hi) { if (v < lo) return lo; if (v > hi) return hi; return v; }

		// helper: emit N particles at position in view-local coords
		void EmitAt(const ImVec2& pos, int count)
		{
			if (count <= 0) return;
			int allowed = psMaxParticles - (int)particles.size();
			int createCount = std::min(count, std::max(0, allowed));
			if (createCount <= 0) return;
			particles.reserve(particles.size() + createCount);

			for (int i = 0; i < createCount; ++i) {
				RectParticle p;
				if (useDefaultParticle) {
					p = RectParticleUtils::CreateParticle(
						rp_emitter.system.spawnParticle.rp_transform,
						rp_emitter.system.spawnParticle.rp_colorData,
						rp_emitter.system.spawnParticle.rp_lifetimeData
					);
				}
				else {
					p = RectParticleUtils::CreateRandomParticle(view_size);
				}

				// position: if circle emitter, random point inside radius
				if (emitterShape == EmitterShape::Circle && emitterSpawnRadius > 0.0f) {
					float a = (static_cast<float>(rand()) / RAND_MAX) * 2.0f * 3.14159265f;
					float r = sqrtf(static_cast<float>(rand()) / RAND_MAX) * emitterSpawnRadius;
					ImVec2 offs = ImVec2(cosf(a) * r, sinf(a) * r);
					p.rp_transform.p_position = ImVec2(pos.x + offs.x, pos.y + offs.y);
				}
				else {
					p.rp_transform.p_position = pos;
				}

				particles.push_back(std::move(p));
			}
		}

		void ClearParticles()
		{
			particles.clear();
			rp_emitter.system.activeParticles.clear();
		}


		void HandleHoverBurst(float ts)
		{
			if (!emit_on_hover) return;
			if (is_hovered) {
				hoverBurstTimer += ts;
				if (hoverBurstTimer >= hoverBurstInterval) {
					hoverBurstTimer = 0.0f;
					//EmitAt(mouse_pos, hoverBurstCount);
				}
			}
		}

		// Click handling — now uses EmitAt and velocity radial burst logic optionally
		void HandleClick(float ts)
		{
			if (!was_clicked) return;

			// Create a radial burst of particles exploding from the clicked point
			const int burstCount = 100;
			int allowed = psMaxParticles - (int)particles.size();
			int createCount = std::min(burstCount, std::max(0, allowed));
			particles.reserve(particles.size() + createCount);
			was_clicked = false;

			for (int i = 0; i < createCount; ++i) {
				RectParticle p = RectParticleUtils::CreateParticle(
					rp_emitter.system.spawnParticle.rp_transform,
					rp_emitter.system.spawnParticle.rp_colorData,
					rp_emitter.system.spawnParticle.rp_lifetimeData
				);

				p.rp_transform.p_position = lastClickPos;

				// Random radial velocity
				float angle = (static_cast<float>(rand()) / RAND_MAX) * 2.0f * 3.14159265358979323846f;
				float minSpeed = 50.0f;
				float maxSpeed = 300.0f;
				float speed = minSpeed + (static_cast<float>(rand()) / RAND_MAX) * (maxSpeed - minSpeed);
				p.rp_animation.p_velocity = ImVec2(cosf(angle), sinf(angle));
				p.rp_animation.p_movementSpeed = speed;
				p.rp_animation.canMove = true;

				// random lifetime around base
				float baseLifetime = p.rp_lifetimeData.p_maxLifetime > 0.0f ? p.rp_lifetimeData.p_maxLifetime : 1.0f;
				float randLifetime = baseLifetime * (0.75f + (static_cast<float>(rand()) / RAND_MAX) * 0.75f);
				p.rp_lifetimeData.p_maxLifetime = randLifetime;
				p.rp_lifetimeData.p_lifetime = 0.0f;

				if (p.rp_transform.p_randomRotation) {
					p.rp_transform.p_rotation.x = (static_cast<float>(rand()) / RAND_MAX) * 360.0f;
				}

				particles.push_back(std::move(p));
			}

			//RectParticleUtils::CreateRadialParticleBurst(
			//	view_size,
			//	lastClickPos,
			//	100,
			//	particles,
			//	&rp_emitter.system.spawnParticle
			//);
		}

		// minimal preset save/load (very simple CSV-like)
		bool SavePreset(const std::string& filename)
		{
			std::ofstream ofs(filename, std::ios::out);
			if (!ofs.good()) return false;
			// save some fields (expand as needed)
			ofs << "useDefaultParticle " << (useDefaultParticle ? 1 : 0) << "\n";
			ofs << "default_count " << defaultParticleCount << "\n";
			ofs << "max_count " << maxParticleCount << "\n";
			ofs << "ps_max_particles " << psMaxParticles << "\n";
			ofs << "hover_count " << hoverBurstCount << "\n";
			ofs << "hover_interval " << hoverBurstInterval << "\n";
			ofs << "gravity " << gravity.x << " " << gravity.y << "\n";
			ofs << "drag " << drag << "\n";
			// particle color example
			ofs << "start_color " << defaultParticle.rp_colorData.p_startColor.x << " " << defaultParticle.rp_colorData.p_startColor.y << " " << defaultParticle.rp_colorData.p_startColor.z << " " << defaultParticle.rp_colorData.p_startColor.w << "\n";
			ofs.close();
			return true;
		}

		bool LoadPreset(const std::string& filename)
		{
			std::ifstream ifs(filename, std::ios::in);
			if (!ifs.good()) return false;
			std::string key;
			while (ifs >> key) {
				if (key == "useDefaultParticle") { int v; ifs >> v; useDefaultParticle = (v != 0); }
				else if (key == "default_count") { ifs >> defaultParticleCount; }
				else if (key == "max_count") { ifs >> maxParticleCount; }
				else if (key == "ps_max_particles") { ifs >> psMaxParticles; }
				else if (key == "hover_count") { ifs >> hoverBurstCount; }
				else if (key == "hover_interval") { ifs >> hoverBurstInterval; }
				else if (key == "gravity") { ifs >> gravity.x >> gravity.y; }
				else if (key == "drag") { ifs >> drag; }
				else if (key == "start_color") { ifs >> defaultParticle.rp_colorData.p_startColor.x >> defaultParticle.rp_colorData.p_startColor.y >> defaultParticle.rp_colorData.p_startColor.z >> defaultParticle.rp_colorData.p_startColor.w; }
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
			ImVec2 initialSize = view_size;
			if (initialSize.x <= 0.0f || initialSize.y <= 0.0f) {
				initialSize = min_view_size;
			}

			rp_emitter = RectParticleEmitter(initialSize, &defaultParticle, &defaultParticle);
			//Inititialize();
		}


		// OnUIRender: keep most existing rendering & add new controls
		virtual void OnUIRender() override
		{
			if (ImGui::Begin("2D Particle Layer")) {

				draw_list = ImGui::GetWindowDrawList();
				ImVec2 win_pos = ImGui::GetWindowPos();
				ImVec2 win_size = ImGui::GetWindowSize();
				view_pos = win_pos;
				view_size = win_size;
				if (view_size.x < min_view_size.x) view_size.x = min_view_size.x;
				if (view_size.y < min_view_size.y) view_size.y = min_view_size.y;

				rp_emitter.SetViewportSize(view_size);

				ImGui::InvisibleButton("viewport", view_size);
				if (ImGui::IsItemHovered()) {
					is_hovered = true;
					mouse_pos = ImVec2(ImGui::GetIO().MousePos.x - view_pos.x, ImGui::GetIO().MousePos.y - view_pos.y);

					if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
						was_clicked = true;
						lastClickPos = mouse_pos;
					}

					// Right click menu for quick actions
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
						ImGui::OpenPopup("viewport_context");
					}
					if (ImGui::BeginPopup("viewport_context")) {
						if (ImGui::MenuItem("Emit 10 at cursor")) {/* EmitAt(mouse_pos, 10);*/ }
						if (ImGui::MenuItem("Emit 100 at cursor")) { /*EmitAt(mouse_pos, 100);*/ }
						if (ImGui::MenuItem("Clear Particles")) { ClearParticles(); }
						ImGui::EndPopup();
					}
				}
				else {
					is_hovered = false;
				}

				// Background color rect
				if (bg_particle.bg_lifetimeData.p_maxLifetime <= 0.0f) {
					bg_particle.bg_lifetimeData.p_maxLifetime = 1.0f;
				}
				if (bg_particle.bg_lifetimeData.p_lifetime >= bg_particle.bg_lifetimeData.p_maxLifetime) {
					bg_particle.bg_lifetimeData.p_lifetime = 0.0f;
					bg_particle.bg_lifetimeData.p_maxLifetime = 5.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 5.0f));

					bg_particle.bg_colorData.p_startColor = bg_particle.bg_colorData.p_endColor;
					bg_particle.bg_colorData.p_endColor = ImVec4(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
						static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
						static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 1.0f);
				}
				float bg_lifeRatio = bg_particle.bg_lifetimeData.p_lifetime / bg_particle.bg_lifetimeData.p_maxLifetime;
				bg_lifeRatio = std::clamp(bg_lifeRatio, 0.0f, 1.0f);
				bg_particle.bg_colorData.p_currColor = ImVec4(
					bg_particle.bg_colorData.p_startColor.x + (bg_particle.bg_colorData.p_endColor.x - bg_particle.bg_colorData.p_startColor.x) * bg_lifeRatio,
					bg_particle.bg_colorData.p_startColor.y + (bg_particle.bg_colorData.p_endColor.y - bg_particle.bg_colorData.p_startColor.y) * bg_lifeRatio,
					bg_particle.bg_colorData.p_startColor.z + (bg_particle.bg_colorData.p_endColor.z - bg_particle.bg_colorData.p_startColor.z) * bg_lifeRatio,
					1.0f);

				draw_list->AddRectFilled(view_pos, ImVec2(view_pos.x + view_size.x, view_pos.y + view_size.y), ImGui::GetColorU32(bg_particle.bg_colorData.p_currColor));

				// if empty, populate initial set (preserve old behavior)
				if (particles.empty()) {

					if (!initialized) {
						Inititialize();
						initialized = true;
					}
					else {

					}

				}

				// draw each particle (respecting rotation)
				for (const auto& particle : particles) {
					if (particle.rp_animation.canRotate) {
						RectParticleUtils::DrawRotatingParticle(draw_list, particle, view_pos);
					}
					else {
						RectParticleUtils::DrawParticle(draw_list, particle, view_pos);
					}
				}

				//rp_emitter.Draw(draw_list, view_pos);

				RectParticleUtils::RestrictParticles(particles, maxParticleCount, view_size);
			}

			ImGui::End();
		}

		void Inititialize() {
			if (initialized) return;
			particles.clear();

			if (useDefaultParticle) {
				particles.reserve(defaultParticleCount);
				for (int i = 0; i < defaultParticleCount; ++i) {
					particles.push_back(RectParticleUtils::CreateParticle(
						defaultParticle.rp_transform,
						defaultParticle.rp_colorData,
						defaultParticle.rp_lifetimeData
					));
				}
			}
			else {
				//particles.reserve(defaultParticleCount);
				//for (int i = 0; i < defaultParticleCount; ++i) {
				//	particles.push_back(RectParticleUtils::CreateRandomParticle(view_size));
				//}

				particles.reserve(defaultParticleCount);
				for (int i = 0; i < defaultParticleCount; ++i) {
					particles.push_back(RectParticleUtils::CreateParticle(
						defaultParticle.rp_transform,
						defaultParticle.rp_colorData,
						defaultParticle.rp_lifetimeData
					));
				}
			}

			initialized = true;

		}

		// central update respects pause, applies forces, updates emitter and particles
		virtual void OnUpdate(float ts) override
		{
			// apply global speed multiplier & paused/step logic
			float simTs = ts * globalSpeedMultiplier;
			if (paused && !stepOnce) {
				// no simulation update, but maybe we still want to process emitter UI updates
				// Keep background color animation though
				bg_particle.bg_lifetimeData.p_lifetime += ts;
				// update bg color (same as in your original OnUpdate)
				if (bg_particle.bg_lifetimeData.p_maxLifetime <= 0.0f)
					bg_particle.bg_lifetimeData.p_maxLifetime = 1.0f;
				float bg_lifeRatio = bg_particle.bg_lifetimeData.p_lifetime / bg_particle.bg_lifetimeData.p_maxLifetime;
				bg_lifeRatio = std::clamp(bg_lifeRatio, 0.0f, 1.0f);
				bg_particle.bg_colorData.p_currColor = ImVec4(
					bg_particle.bg_colorData.p_startColor.x + (bg_particle.bg_colorData.p_endColor.x - bg_particle.bg_colorData.p_startColor.x) * bg_lifeRatio,
					bg_particle.bg_colorData.p_startColor.y + (bg_particle.bg_colorData.p_endColor.y - bg_particle.bg_colorData.p_startColor.y) * bg_lifeRatio,
					bg_particle.bg_colorData.p_startColor.z + (bg_particle.bg_colorData.p_endColor.z - bg_particle.bg_colorData.p_startColor.z) * bg_lifeRatio,
					1.0f);
				// consume a single step if requested, then return
				if (stepOnce) {
					stepOnce = false;
				}
				else {
					return;
				}
			}

			// Update particles: apply gravity and drag, then call existing UpdateParticle
			for (auto& particle : particles) {
				// apply gravity (as acceleration) to the velocity vector
				if (particle.rp_animation.canMove) {
					// convert to world velocity vector
					ImVec2 v = ImVec2(particle.rp_animation.p_velocity.x * particle.rp_animation.p_movementSpeed,
						particle.rp_animation.p_velocity.y * particle.rp_animation.p_movementSpeed);

					// acceleration
					v.x += gravity.x * simTs;
					v.y += gravity.y * simTs;

					// apply a simple drag (scale down speed)
					float dragFactor = Clamp(1.0f - drag * simTs, 0.0f, 1.0f);
					v.x *= dragFactor;
					v.y *= dragFactor;

					// convert back into direction + magnitude
					float speed = sqrtf(v.x * v.x + v.y * v.y);
					if (speed > 0.0001f) {
						particle.rp_animation.p_velocity = ImVec2(v.x / speed, v.y / speed);
						particle.rp_animation.p_movementSpeed = speed;
					}
					else {
						particle.rp_animation.p_movementSpeed = 0.0f;
					}
				}
				// call your utility that updates lifetime, transform, rotation, color etc.
				RectParticleUtils::UpdateParticle(particle, simTs, view_size, useDefaultParticle, &defaultParticle);
			}

			// background anim
			bg_particle.bg_lifetimeData.p_lifetime += ts;
			if (bg_particle.bg_lifetimeData.p_maxLifetime <= 0.0f)
				bg_particle.bg_lifetimeData.p_maxLifetime = 1.0f;
			float bg_lifeRatio = bg_particle.bg_lifetimeData.p_lifetime / bg_particle.bg_lifetimeData.p_maxLifetime;
			bg_lifeRatio = std::clamp(bg_lifeRatio, 0.0f, 1.0f);
			bg_particle.bg_colorData.p_currColor = ImVec4(
				bg_particle.bg_colorData.p_startColor.x + (bg_particle.bg_colorData.p_endColor.x - bg_particle.bg_colorData.p_startColor.x) * bg_lifeRatio,
				bg_particle.bg_colorData.p_startColor.y + (bg_particle.bg_colorData.p_endColor.y - bg_particle.bg_colorData.p_startColor.y) * bg_lifeRatio,
				bg_particle.bg_colorData.p_startColor.z + (bg_particle.bg_colorData.p_endColor.z - bg_particle.bg_colorData.p_startColor.z) * bg_lifeRatio,
				1.0f);

			// emitter update (let the emitter produce particles if it handles emission internally)
			//rp_emitter.Update(simTs, view_size, lastClickPos);

			// If the emitter has its own logic we also respect emitterAutoEmit: if it's disabled we won't let emitter produce new particles
			// (Assuming RectParticleEmitter::Update uses system.emissionRate; if it doesn't, you'd call rp_emitter.Emit(...) from here instead)
			if (!emitterAutoEmit) {
				// attempt to clear emitter's generated particles so it doesn't add automatically.
				// This depends on emitter design. If emitter adds to system.activeParticles inside Update, you may need to prevent that upstream.
				// As a simple fallback, we won't clear here to not break the emitter internals.
			}

			HandleHoverBurst(simTs);
			HandleClick(simTs);

			// limit particles
			RectParticleUtils::RestrictParticles(particles, maxParticleCount, view_size);

			// Step mode consumes once
			if (stepOnce) {
				stepOnce = false;
				paused = true;
			}
		}
	};

	class IMGUI_2D_PARTICLE_LAYER_PROPERTIES : public Walnut::Layer
	{
	public:
		float sliderSpeed = 1.0f;
		int defaultParticleCount = 100;
		int maxParticleCount = 300;
		int hoverBurstCount = 25;
		float hoverBurstInterval = 0.05f;
		float hoverBurstTimer = 0.0f;
		float readyTimer = 0.5f;
		bool startEventTriggered = false;
		RectParticle defaultParticle;
		BackgroundParticle defaultBackground;
		bool useDefaultParticle = true;
		RectParticlePreset preset = RectParticlePreset::RED_TO_YELLOW_FADE;

		IMGUI_2D_PARTICLE_LAYER* p_layer = nullptr;
		bool is_active = false;

		void SetParticleLayer(IMGUI_2D_PARTICLE_LAYER* layer) {
			p_layer = layer;
		}

		void SetupDefaultParticle() {
			if (!p_layer)
				return;

			// Center of Viewport
			defaultParticle.rp_transform.p_position = ImVec2(p_layer->view_size.x / 2.0f, p_layer->view_size.y / 2.0f);
			// Size 10x10
			defaultParticle.rp_transform.p_size = ImVec2(10.0f, 10.0f);
			// No Size Randomization
			defaultParticle.rp_transform.p_randomSize = false;
			// No Rotation			
			defaultParticle.rp_transform.p_randomRotation = false;

			// Downward Velocity
			defaultParticle.rp_animation.p_velocity = ImVec2(-1.0f, -3.0f);
			// Moderate Speed			
			defaultParticle.rp_animation.p_movementSpeed = 50.0f;
			// Enable Movement
			defaultParticle.rp_animation.canMove = true;

			// Start Color Red
			defaultParticle.rp_colorData.p_startColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
			// End Color Yellow
			defaultParticle.rp_colorData.p_endColor = ImVec4(1.0f, 1.0f, 0.0f, 0.0f);
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

			if (p_layer) {
				p_layer->defaultParticle = defaultParticle;
				p_layer->useDefaultParticle = useDefaultParticle;
				p_layer->rp_emitter = RectParticleEmitter(p_layer->view_size, &p_layer->defaultParticle, &p_layer->defaultParticle);
				p_layer->rp_emitter.emissionMode = EmissionMode::EMIT_CONTINUOUS_DEFAULT;
			}
		}

		void ApplyColorPreset(const RectParticle& presetData) {
			defaultParticle.rp_colorData = presetData.rp_colorData;
			p_layer->defaultParticle = defaultParticle;
		}

		virtual void OnUIRender() override {
			if (!p_layer)
				return;

			p_layer->defaultParticle = defaultParticle;
			p_layer->useDefaultParticle = useDefaultParticle;

			ImGui::Begin("2D Particle Details");
			ImVec2 max = ImVec2(ImGui::GetWindowContentRegionMax().x * 2.0f, ImGui::GetWindowContentRegionMax().y * 2.0f);
			ImVec2 min = ImGui::GetWindowPos();
			ImVec2 propMax = ImVec2(min.x + max.x, min.y + max.y);
			is_active = ImGui::IsMouseHoveringRect(ImGui::GetWindowPos(), max);
			if (is_active)
			{
				ImGui::Text("Simulation");
				ImGui::SameLine();
				if (ImGui::Button(p_layer->paused ? "Resume" : "Pause")) {
					p_layer->paused = !p_layer->paused;
				}
				ImGui::SameLine();
				if (ImGui::Button("Step")) {
					p_layer->stepOnce = true;
					p_layer->paused = false;
				}
				ImGui::SameLine();
				if (ImGui::Button("Clear All")) {
					p_layer->ClearParticles();
				}
				ImGui::Separator();

				if (ImGui::TreeNode("Background Particle")) {



					ImGui::Separator();
					ImGui::TreePop();
					ImGui::Spacing();
				}

				// Speed multiplier
				ImGui::DragFloat("Global Speed Multiplier", &p_layer->globalSpeedMultiplier, 0.1f, 0.0f, 10.0f);

				// Particle counts
				ImGui::DragInt("Default Particle Count", &defaultParticleCount, 1, 0, 5000);
				ImGui::DragInt("Max Particle Count (layer)", &p_layer->maxParticleCount, 1, 0, 10000);
				ImGui::DragInt("System Max Particles (psMax)", &p_layer->psMaxParticles, 1, 0, 20000);

				ImGui::Separator();


				// Hover/click controls
				if (ImGui::TreeNode("Hover & Click")) {
					ImGui::Checkbox("Emit On Hover", &p_layer->emit_on_hover);
					ImGui::DragInt("Hover Burst Count", &p_layer->hoverBurstCount, 1, 0, 1000);
					ImGui::DragFloat("Hover Burst Interval", &p_layer->hoverBurstInterval, 0.001f, 0.001f, 1.0f);
					ImGui::Separator();
					if (ImGui::Button("Emit 50 at center")) {
						//p_layer->EmitAt(ImVec2(p_layer->view_size.x / 2.0f, p_layer->view_size.y / 2.0f), 50);
					}
					ImGui::TreePop();
				}
				ImGui::Separator();

			}


			// set min size from properties window to fit default particle controls
			ImVec2 propMin = ImVec2();
			// get the length of all checkbox labels
			propMin = ImGui::CalcTextSize("Random Size Random Rotation Use Default Particle", nullptr, true, 0.0f);


			// make sure the default particle tree is always minimum size to fit all controls
			ImGui::SetNextItemWidth(propMin.x + 50.0f);
			if (ImGui::TreeNode("Default Particle")) {
				if (ImGui::Checkbox("Use Default Particle", &useDefaultParticle)) {
					p_layer->useDefaultParticle = useDefaultParticle;
					SetupDefaultParticle();
				}
				ImGui::SameLine();
				if (ImGui::Button("Save As Layer Default")) {
					p_layer->defaultParticle = defaultParticle;
					//p_layer->rp_emitter = RectParticleEmitter(p_layer->view_size, &p_layer->defaultParticle, &p_layer->defaultParticle);
				}
				ImGui::Separator();

				if (ImGui::TreeNode("Transform")) {
					if (ImGui::Button("Center in Viewport"))
					{
						defaultParticle.rp_transform.p_position = ImVec2(p_layer->view_size.x / 2.0f, p_layer->view_size.y / 2.0f);
						p_layer->defaultParticle = defaultParticle;
					}
					ImGui::DragFloat2("Position", (float*)&defaultParticle.rp_transform.p_position, sliderSpeed, 0.0f, p_layer->view_size.x);
					ImGui::DragFloat2("Size", (float*)&defaultParticle.rp_transform.p_size, sliderSpeed, 1.0f, p_layer->view_size.x);
					ImGui::Checkbox("Random Size", &defaultParticle.rp_transform.p_randomSize);
					ImGui::Checkbox("Random Rotation", &defaultParticle.rp_transform.p_randomRotation);
					ImGui::DragFloat("Rotation", &defaultParticle.rp_transform.p_rotation.x, 1.0f, 0.0f, 360.0f);
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Animation")) {
					ImGui::DragFloat2("Velocity Dir", (float*)&defaultParticle.rp_animation.p_velocity, sliderSpeed, -10.0f, 10.0f);
					ImGui::DragFloat("Speed", &defaultParticle.rp_animation.p_movementSpeed, sliderSpeed, 0.0f, 2000.0f);
					ImGui::Checkbox("Can Move", &defaultParticle.rp_animation.canMove);
					ImGui::Checkbox("Can Rotate", &defaultParticle.rp_animation.canRotate);
					ImGui::DragFloat("Rotation Speed", &defaultParticle.rp_animation.p_rotationSpeed, 0.1f, -1000.0f, 1000.0f);
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Color")) {
					if (ImGui::Combo("Color Preset", (int*)&preset, "None\0Red to Yellow Fade\0Red to Transparent Fade\0Yellow to Green Fade\0Yellow to Transparent Fade\0Blue to Cyan Fade\0Blue to Transparent Fade\0\0")) {
						switch (preset) {
						case RectParticlePreset::RED_TO_YELLOW_FADE:
							ApplyColorPreset(RED_TO_YELLOW_FADE_OUT);
							break;
						case RectParticlePreset::RED_TO_TRANSPARENT_FADE:
							ApplyColorPreset(RED_TO_TRANSPARENT_FADE_OUT);
							break;
						case RectParticlePreset::YELLOW_TO_GREEN_FADE:
							ApplyColorPreset(YELLOW_TO_GREEN_FADE_OUT);
							break;
						case RectParticlePreset::YELLOW_TO_TRANSPARENT_FADE:
							ApplyColorPreset(YELLOW_TO_TRANSPARENT_FADE_OUT);
							break;
						case RectParticlePreset::BLUE_TO_CYAN_FADE:
							ApplyColorPreset(BLUE_TO_CYAN_FADE_OUT);
							break;
						case RectParticlePreset::BLUE_TO_TRANSPARENT_FADE:
							ApplyColorPreset(BLUE_TO_TRANSPARENT_FADE_OUT);
							break;
						case RectParticlePreset::NONE:
						default:
							// do nothing
							break;
						}

					}
					ImGui::ColorEdit4("Start Color", (float*)&defaultParticle.rp_colorData.p_startColor);
					ImGui::ColorEdit4("End Color", (float*)&defaultParticle.rp_colorData.p_endColor);
					ImGui::Checkbox("Lerp Color", &defaultParticle.rp_colorData.p_lerpColor);
					ImGui::DragFloat("Lerp Speed", &defaultParticle.rp_colorData.p_lerpSpeed, sliderSpeed, 0.0f, 5.0f);
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Lifetime")) {
					ImGui::DragFloat("Max Lifetime", &defaultParticle.rp_lifetimeData.p_maxLifetime, 0.01f, 0.01f, 60.0f);
					ImGui::Checkbox("Randomize Lifetime", &defaultParticle.rp_lifetimeData.p_randomizeLifetime);
					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			ImGui::Separator();
			ImGui::Spacing();
			p_layer->defaultParticle = defaultParticle;
			p_layer->useDefaultParticle = useDefaultParticle;

			// Emitter panel
			if (ImGui::TreeNode("Particle Emitter")) {
				auto& emitter = p_layer->rp_emitter;
				auto& sys = emitter.system;

				ImGui::Text("Emitter Controls");
				ImGui::DragFloat2("Emitter Position", (float*)&sys.s_position, sliderSpeed, 0.0f, p_layer->view_size.x);
				ImGui::DragFloat("Spawn Radius", &p_layer->emitterSpawnRadius, 1.0f, 0.0f, std::max(p_layer->view_size.x, p_layer->view_size.y));
				ImGui::Combo("Emitter Shape", (int*)&p_layer->emitterShape, "Point\0Circle\0\0");

				ImGui::DragFloat("Emission Rate (particles/sec)", &sys.emissionRate, 0.1f, 0.0f, 2000.0f);
				ImGui::DragInt("Emitter Max Particles", &sys.maxParticles, 1, 0, 10000);

				ImGui::Checkbox("Auto Emit (emitter)", &p_layer->emitterAutoEmit);
				ImGui::DragInt("Manual Emit Count", &p_layer->emitterManualEmitCount, 1, 1, 5000);

				if (ImGui::Button("Emit Manual")) {
					// if emitter shape circle -> spawn around sys.s_position
					ImVec2 pos = sys.emitterParticle.rp_transform.p_position;
					p_layer->EmitAt(pos, p_layer->emitterManualEmitCount);
				}
				ImGui::SameLine();
				if (ImGui::Button("Reset Emitter")) {
					emitter.Reset(p_layer->view_size, &p_layer->defaultParticle, &p_layer->defaultParticle);
				}
				ImGui::SameLine();
				if (ImGui::Button("Clear Emitter Children")) {
					sys.activeParticles.clear();
				}

				// show linked color
				ImGui::Text("Linked spawn color: (%.2f, %.2f, %.2f)", sys.spawnParticle.rp_colorData.p_startColor.x, sys.spawnParticle.rp_colorData.p_startColor.y, sys.spawnParticle.rp_colorData.p_startColor.z);

				// keep emitter linked to layer defaults
				emitter.system.spawnParticle = p_layer->defaultParticle;
				emitter.system.emitterParticle = p_layer->defaultParticle;

				ImGui::TreePop();
			}

			ImGui::Separator();

			// Physics / visual controls
			if (ImGui::TreeNode("Physics & Visuals")) {
				ImGui::DragFloat2("Gravity", (float*)&p_layer->gravity, 1.0f, -2000.0f, 2000.0f);
				ImGui::DragFloat("Drag (damping)", &p_layer->drag, 0.01f, 0.0f, 5.0f);
				ImGui::Combo("Blend Mode", (int*)&p_layer->blendMode, "Alpha\0Additive\0Multiply\0\0");
				ImGui::TreePop();
			}

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

			p_layer->defaultParticle = defaultParticle;
			p_layer->useDefaultParticle = useDefaultParticle;
			p_layer->rp_emitter.system.emitterParticle = p_layer->defaultParticle;
			p_layer->rp_emitter.system.spawnParticle = p_layer->defaultParticle;
			ImGui::End();
		}

		virtual void OnUpdate(float ts) override {
			//p_layer->rp_emitter.Update(ts, p_layer->view_size, p_layer->lastClickPos);
			p_layer->rp_emitter.system.spawnParticle = p_layer->defaultParticle;

			if (!startEventTriggered) {
				readyTimer -= ts;
				if (readyTimer <= 0.0f) {
					p_layer->lastClickPos = ImVec2(p_layer->view_size.x / 2.0f, p_layer->view_size.y / 2.0f);
					p_layer->was_clicked = true;
					startEventTriggered = true;
				}
			}



		}
	};

}