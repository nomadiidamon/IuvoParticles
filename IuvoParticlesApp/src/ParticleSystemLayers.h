#pragma once
#include "Walnut/Application.h"
#include "Walnut/Image.h"
#include "RectParticle.h"

namespace Particles {

	class IMGUI_2D_PARTICLE_LAYER : public Walnut::Layer
	{
	public:
		ImDrawList* draw_list;
		ImVec2 view_pos;
		ImVec2 view_size;
		ImVec2 min_view_size = ImVec2(200, 200);
		ImVec2 mouse_pos;
		ImVec2 lastClickPos;
		std::vector<RectParticle> particles;
		int defaultParticleCount = 100;
		int maxParticleCount = 1000;
		int psMaxParticles = 1000;

		bool is_hovered = false;
		bool emit_on_hover = true;
		bool was_clicked = false;
		int hoverBurstCount = 25;
		float hoverBurstInterval = 0.025f;
		float hoverBurstTimer = 0.0f;

		RectParticle defaultParticle;
		BackgroundParticle bg_particle;
		RectParticleEmitter rp_emitter;
		bool useDefaultParticle = false;



		void HandleHoverBurst(float ts)
		{
			if (!emit_on_hover) return;

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

		void HandleClick(float ts) 
		{
			if (was_clicked) {
				
				// set the position of the emitter to the click position
				rp_emitter.SetPosition(view_size, lastClickPos);
				

				// Create a burst of particles exploding from the clicked point
				for (int i = 0; i < 100; ++i) {

					/// TODO: implement radial burst from position

					// for now...
					particles.push_back(RectParticleUtils::CreateRandomParticle(view_size, lastClickPos));
				}
				was_clicked = false;

			}
		}

		virtual void OnAttach() override {
			rp_emitter = RectParticleEmitter(view_size, &defaultParticle, &defaultParticle);
		}


		virtual void OnUIRender() override
		{
			//rp_emitter.SetPosition(view_size, ImVec2(view_size.x / 4.0f, view_size.y / 4.0f));

			if (ImGui::Begin("2D Particle Layer")) {

				draw_list = ImGui::GetWindowDrawList();
				ImVec2 win_pos = ImGui::GetWindowPos();
				ImVec2 win_size = ImGui::GetWindowSize();
				view_pos = win_pos;
				view_size = win_size;
				if (view_size.x < min_view_size.x) view_size.x = min_view_size.x;
				if (view_size.y < min_view_size.y) view_size.y = min_view_size.y;
				ImGui::InvisibleButton("viewport", view_size);
				if (ImGui::IsItemHovered()) {
					is_hovered = true;
					mouse_pos = ImVec2(ImGui::GetIO().MousePos.x - view_pos.x, ImGui::GetIO().MousePos.y - view_pos.y);

					if (ImGui::IsItemClicked()) {
						was_clicked = true;
						lastClickPos = mouse_pos;
					}
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
				rp_emitter.Draw(draw_list, view_pos);


				RectParticleUtils::RestrictParticles(particles, maxParticleCount, view_size);
			}

			ImGui::End();
		}

		virtual void OnUpdate(float ts) override
		{
			for (auto& particle : particles) {
				RectParticleUtils::UpdateParticle(particle, ts, view_size, useDefaultParticle, &defaultParticle);
			}

			/// TODO: move background particle update to its own function
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

			HandleClick(ts);
		}
	};

	class IMGUI_2D_PARTICLE_LAYER_PROPERTIES : public Walnut::Layer
	{
	public:
		float sliderSpeed = 1.0f;
		int defaultParticleCount = 100;
		int maxParticleCount = 300;
		int hoverBurstCount = 25;
		float hoverBurstInterval = 0.025f;
		float hoverBurstTimer = 0.0f;

		RectParticle defaultParticle;
		BackgroundParticle defaultBackground;
		bool useDefaultParticle = false;

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
			p_layer->defaultParticle = defaultParticle;
			p_layer->useDefaultParticle = useDefaultParticle;
			p_layer->rp_emitter = RectParticleEmitter(p_layer->view_size, &p_layer->defaultParticle, &p_layer->defaultParticle);
			p_layer->rp_emitter.emissionMode = EMIT_CONTINUOUS_DEFAULT;

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
				if (ImGui::Button("Clear All Particles"))
				{
					p_layer->particles.clear();
					p_layer->rp_emitter.system.activeParticles.clear();
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Emit On Hover", &p_layer->emit_on_hover)) {
					p_layer->emit_on_hover = true;
				}
				else
				{
					p_layer->emit_on_hover = false;
				}

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
						p_layer->defaultParticle.rp_transform.p_randomSize = defaultParticle.rp_transform.p_randomSize;
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Random Rotation", &defaultParticle.rp_transform.p_randomRotation)) {
						p_layer->defaultParticle.rp_transform.p_randomRotation = defaultParticle.rp_transform.p_randomRotation;
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Use Default Particle", &useDefaultParticle)) {
						p_layer->useDefaultParticle = useDefaultParticle;
						SetupDefaultParticle();
					}
					ImGui::Separator();
					if (ImGui::Button("Reset to Default Values")) {
						if (RectParticleUtils::IsEqual(defaultParticle, p_layer->defaultParticle))
						{
							SetupDefaultParticle();
						}
						else
						{
							defaultParticle = p_layer->defaultParticle;
						}
					}
					ImGui::SameLine();
					if (ImGui::Button("Clear Values")) {
						defaultParticle = RectParticle();
					}
					ImGui::Separator();
					if (ImGui::Button("Save As Default")) {
						
						p_layer->defaultParticle = defaultParticle;
						p_layer->rp_emitter = RectParticleEmitter(p_layer->view_size, &p_layer->defaultParticle, &p_layer->defaultParticle);
					}
					ImGui::SameLine();
					if (ImGui::Button("Center Particle")) {
						defaultParticle.rp_transform.p_position = ImVec2(p_layer->view_size.x / 2.0f, p_layer->view_size.y / 2.0f);
					}
					ImGui::SameLine();
					if (ImGui::Button("Random Values")) {
						defaultParticle = RectParticleUtils::CreateRandomParticle(p_layer->view_size);
					}
				}

				if (ImGui::TreeNode("Particle Transform"))
				{
					if (is_active) {

					}
					ImGui::DragFloat("Position X", &defaultParticle.rp_transform.p_position.x, sliderSpeed, 0.0f, p_layer->view_size.x);
					ImGui::DragFloat("Position Y", &defaultParticle.rp_transform.p_position.y, sliderSpeed, 0.0f, p_layer->view_size.y);
					ImGui::Separator();
					ImGui::DragFloat("Size X", &defaultParticle.rp_transform.p_size.x, sliderSpeed, 1.0f, p_layer->view_size.x);
					ImGui::DragFloat("Size Y", &defaultParticle.rp_transform.p_size.y, sliderSpeed, 1.0f, p_layer->view_size.y);
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
						p_layer->defaultParticle.rp_colorData.p_startColor = defaultParticle.rp_colorData.p_startColor;
						ImGui::ColorPicker4("End Color", (float*)&defaultParticle.rp_colorData.p_endColor);
						p_layer->defaultParticle.rp_colorData.p_endColor = defaultParticle.rp_colorData.p_endColor;
						ImGui::DragFloat("Lerp Speed", &defaultParticle.rp_colorData.p_lerpSpeed, sliderSpeed, 0.01f, 1.0f);
						p_layer->defaultParticle.rp_colorData.p_lerpSpeed = defaultParticle.rp_colorData.p_lerpSpeed;
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
				p_layer->defaultParticle = defaultParticle;
				p_layer->useDefaultParticle = useDefaultParticle;
			}

			if (ImGui::TreeNode("Background Particle")) {



				ImGui::Separator();
				ImGui::TreePop();
				ImGui::Spacing();
			}

			if (ImGui::TreeNode("Particle Emitter"))
			{
				auto& emitter = p_layer->rp_emitter;
				auto& sys = emitter.system;

				// Position Controls
				if (ImGui::DragFloat2("Emitter Position", (float*)&sys.s_position,
					sliderSpeed, 0.0f, p_layer->view_size.x))
				{
					emitter.SetPosition(p_layer->view_size, sys.emitterParticle.rp_transform.p_position);
				}

				ImGui::Separator();
				ImGui::DragFloat("Emission Rate (particles/sec)", &sys.emissionRate, 0.1f, 0.1f, 200.0f);
				ImGui::DragInt("Max Particles", &sys.maxParticles, 1, 10, 5000);

				// Option to toggle emission on/off
				static bool autoEmit = true;
				ImGui::Checkbox("Auto Emit", &autoEmit);

				// Manual Emit button
				if (ImGui::Button("Emit Once")) {
					emitter.Emit(1.0f); // emit a single batch
				}

				// Reset all particles
				if (ImGui::Button("Reset Emitter")) {
					emitter.Reset(p_layer->view_size, &p_layer->defaultParticle, &p_layer->defaultParticle);
				}
				ImGui::SameLine();
				if (ImGui::Button("Clear Current Children")) {
					sys.activeParticles.clear();
				}

				ImGui::Separator();
				ImGui::Text("Linked particle color: (Start %.2f, %.2f, %.2f)",
					sys.spawnParticle.rp_colorData.p_startColor.x,
					sys.spawnParticle.rp_colorData.p_startColor.y,
					sys.spawnParticle.rp_colorData.p_startColor.z);

				ImGui::TreePop();
				ImGui::Spacing();

				// Update emitter runtime link
				emitter.system.spawnParticle = p_layer->defaultParticle;
				emitter.system.emitterParticle = p_layer->defaultParticle;
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
			p_layer->defaultParticle = defaultParticle;
			p_layer->rp_emitter.system.emitterParticle = p_layer->defaultParticle;
			p_layer->rp_emitter.system.spawnParticle = p_layer->defaultParticle;
			ImGui::End();
		}

		virtual void OnUpdate(float ts) override {
			p_layer->rp_emitter.Update(ts, p_layer->view_size, p_layer->lastClickPos);

		}
	};

}