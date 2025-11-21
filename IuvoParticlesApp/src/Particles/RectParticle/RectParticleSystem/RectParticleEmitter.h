#pragma once
#include "../RectParticle.h"
#include "../RectParticleUtils.h"
#include "../RectParticleCache.h"

namespace Particles {

    struct ParticleSystem {
        RectParticle emitterParticle;
        RectParticle spawnParticle;
        std::vector<RectParticle> activeParticles;
        std::vector<RectParticleCache> particleCache;

        // exterior position array
        float s_position[2] = { 0.0f, 0.0f };

        // Emitter configuration
        ImVec2 viewSize = ImVec2(800.0f, 600.0f);
        int maxParticles = 200;
        bool useDefaultParticle = false;
        float emissionRate = 10.0f; // particles per second
        float emissionAccumulator = 0.0f; // internal timing accumulator
    };

    enum class EmissionMode {
        EMIT_CONTINUOUS_DEFAULT,
        EMIT_CONTINUOUS_RANDOM,
        EMIT_BURST_DEFUALT,
        EMIT_BURST_RANDOM
    };
    enum class EmitterShape { Point = 0, Circle = 1 };
    enum class BlendMode { Alpha = 0, Additive = 1, Multiply = 2 };

    class RectParticleEmitter {
    public:
        ParticleSystem system;
        ImVec2 emitterPosition;
        EmissionMode emissionMode = EmissionMode::EMIT_CONTINUOUS_DEFAULT;

        RectParticleEmitter(const ImVec2& viewSize = ImVec2(800, 600),
            const RectParticle* emitterParticle = nullptr,
            const RectParticle* spawnParticle = nullptr)
        {
            system.viewSize = viewSize;
            if (emitterParticle)
                system.emitterParticle = *emitterParticle;
            if (spawnParticle)
                system.spawnParticle = *spawnParticle;
        }

        void SetPosition(const ImVec2& viewSize, const ImVec2& pos) {
			SetViewportSize(viewSize);
			system.emitterParticle.rp_transform.p_position = pos;
        }

        void SetViewportSize(const ImVec2& viewSize) {
            system.viewSize = viewSize;
        }

        void SetEmissionMode(EmissionMode mode) {
            emissionMode = mode;
        }

        void Update(float dt, const ImVec2& viewSize, const ImVec2& pos) {
            SetPosition(viewSize, pos);
            system.viewSize = viewSize;
            Emit(dt);
            for (auto& p : system.activeParticles)
                RectParticleUtils::UpdateParticle(p, dt, viewSize, system.activeParticles, system.useDefaultParticle, &system.spawnParticle);
            RectParticleUtils::RestrictParticles(system.activeParticles, system.maxParticles, viewSize);
        }

        void Draw(ImDrawList* drawList, const ImVec2& viewPos) {
            for (const auto& p : system.activeParticles)
                RectParticleUtils::DrawParticleAgnostic(drawList, p, viewPos);
        }

        void Emit(float dt) {
            system.emissionAccumulator += dt * system.emissionRate;
            while (system.emissionAccumulator >= 1.0f &&
                (int)system.activeParticles.size() < system.maxParticles)
            {
                switch (emissionMode) {
                case EmissionMode::EMIT_CONTINUOUS_DEFAULT:
                    //system.activeParticles.push_back(RectParticleUtils::
                    //    CreateDefaultParticle(system.viewSize, emitterPosition, &system.spawnParticle));

					//system.activeParticles.push_back(RectParticleUtils::CreateParticle(system.emitterParticle.rp_transform, system.emitterParticle.rp_colorData, system.emitterParticle.rp_lifetimeData));	

					system.activeParticles.push_back(RectParticleUtils.CreateDefaultParticle(system.viewSize, emitterPosition, &system.emitterParticle));	

                    break;
                case EmissionMode::EMIT_CONTINUOUS_RANDOM:
                    //system.activeParticles.push_back(RectParticleUtils::
                    //    CreateRandomParticle(system.viewSize, emitterPosition, &system.spawnParticle));

					system.activeParticles.push_back(RectParticleUtils::CreateParticle(system.emitterParticle.rp_transform, system.emitterParticle.rp_colorData, system.emitterParticle.rp_lifetimeData));

                    break;
                case EmissionMode::EMIT_BURST_DEFUALT:
                    //system.activeParticles.push_back(RectParticleUtils::
                    //    CreateDefaultParticle(system.viewSize, emitterPosition, &system.spawnParticle));

					system.activeParticles.push_back(RectParticleUtils::CreateParticle(system.emitterParticle.rp_transform, system.emitterParticle.rp_colorData, system.emitterParticle.rp_lifetimeData));

                    break;
                case EmissionMode::EMIT_BURST_RANDOM:
                    //system.activeParticles.push_back(RectParticleUtils::
                    //    CreateRandomParticle(system.viewSize, emitterPosition, &system.spawnParticle));

					system.activeParticles.push_back(RectParticleUtils::CreateParticle(system.emitterParticle.rp_transform, system.emitterParticle.rp_colorData, system.emitterParticle.rp_lifetimeData));

                    break;
                }
                system.emissionAccumulator -= 1.0f;
            }
        }

        void Reset(const ImVec2& viewSize = ImVec2(800, 600),
            const RectParticle* emitterParticle = nullptr,
            const RectParticle* spawnParticle = nullptr)
        {
            system.viewSize = viewSize;
            system.activeParticles.clear();
            system.emissionAccumulator = 0.0f;
            if (emitterParticle)
                system.emitterParticle = *emitterParticle;
            if (spawnParticle)
                system.spawnParticle = *spawnParticle;
        }
    };






    ///--------> WALNUT LAYERS <-------///


	class IMGUI_PARTICLE_EMITTER_LAYER : public Walnut::Layer
	{
	public:
		ImDrawList* draw_list;
		ImVec2 view_pos = ImVec2(0.0f, 0.0f);
		ImVec2 view_size = ImVec2(800.0f, 600.0f);

		RectParticleEmitter rp_emitter;
		RectParticle defaultParticle;

		bool emitterAutoEmit = true;
		EmitterShape emitterShape = EmitterShape::Point;
		float emitterSpawnRadius = 0.3f;
		int emitterManualEmitCount = 10;
		int maxParticleCount = 500;

		virtual void OnAttach() override
		{
			rp_emitter = RectParticleEmitter(view_size, &defaultParticle, &defaultParticle);
			rp_emitter.emissionMode = EmissionMode::EMIT_CONTINUOUS_DEFAULT;

		}

		virtual void OnUIRender() override
		{
			if (ImGui::Begin("Particle Emitter Layer")) {

				draw_list = ImGui::GetWindowDrawList();
				ImVec2 win_pos = ImGui::GetWindowPos();
				ImVec2 win_size = ImGui::GetWindowSize();
				view_pos = win_pos;
				view_size = win_size;

				rp_emitter.SetViewportSize(view_size);
				/// TODO: update Draw params with a drawStruct
				rp_emitter.Draw(draw_list, view_pos);

			}
			ImGui::End();
		}

		virtual void OnUpdate(float ts) override
		{
			rp_emitter.system.spawnParticle = defaultParticle;
			rp_emitter.Update(ts, view_size, rp_emitter.emitterPosition);
		}

		// Helper: Emit N particles at a given position
		void EmitAt(const ImVec2& pos, int count)
		{
			for (int i = 0; i < count; ++i) {
				RectParticle p = RectParticleUtils::CreateParticle(rp_emitter.system.spawnParticle.rp_transform, rp_emitter.system.spawnParticle.rp_colorData, rp_emitter.system.spawnParticle.rp_lifetimeData);

				// If emitter is circular, offset particle randomly
				if (emitterShape == EmitterShape::Circle && emitterSpawnRadius > 0.0f) {
					float angle = (static_cast<float>(rand()) / RAND_MAX) * 2.0f * 3.14159265f;
					float radius = sqrtf(static_cast<float>(rand()) / RAND_MAX) * emitterSpawnRadius;
					ImVec2 offset = ImVec2(cosf(angle) * radius, sinf(angle) * radius);
					p.rp_transform.p_position = ImVec2(pos.x + offset.x, pos.y + offset.y);
				}
				else {
					p.rp_transform.p_position = pos;
				}

				rp_emitter.system.activeParticles.push_back(std::move(p));
			}
		}

	};

	class IMGUI_PARTICLE_EMITTER_PROPERTIES : public Walnut::Layer
	{
	public:
		IMGUI_PARTICLE_EMITTER_LAYER* p_layer = nullptr;
		RectParticle defaultParticle;

		int sliderSpeed = 0.01f;

		void SetEmitterLayer(IMGUI_PARTICLE_EMITTER_LAYER* layer) {
			p_layer = layer;
		}

		virtual void OnUIRender() override
		{
			if (!p_layer) return;

			ImGui::Begin("Emitter Properties");

			auto& emitter = p_layer->rp_emitter;
			auto& sys = emitter.system;

			ImGui::DragInt("Max Particle Count", &p_layer->maxParticleCount, 1, 0, 10000);

			if (ImGui::TreeNode("Default Particle")) {

				if (ImGui::Button("Center in Viewport")) {
					defaultParticle.rp_transform.p_position = ImVec2(p_layer->view_size.x / 2.0f, p_layer->view_size.y / 2.0f);
					emitter.SetPosition(p_layer->view_size, defaultParticle.rp_transform.p_position);
				}

				if (ImGui::TreeNode("Transform")) {
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

			if (ImGui::TreeNode("Emitter Controls"))
			{
				if (ImGui::DragFloat2("Emitter Position", (float*)&sys.s_position, 1.0f, 0.0f, p_layer->view_size.x))
				{
					emitter.emitterPosition = ImVec2(sys.s_position[0], sys.s_position[1]);
					emitter.SetPosition(p_layer->view_size, emitter.emitterPosition);
				}

				if (ImGui::DragFloat("Spawn Radius", &p_layer->emitterSpawnRadius, 1.0f, 0.0f, std::max(p_layer->view_size.x, p_layer->view_size.y))) {
				}

				if (ImGui::Combo("Emitter Shape", (int*)&p_layer->emitterShape, "Point\0Circle\0\0"))
				{

				}

				if (ImGui::Combo("Emission Mode", (int*)&emitter.emissionMode, "Continuous Default\0Continuous Random\0Burst Default\0BurstRandom\0\0"))
				{
					if (emitter.emissionMode == EmissionMode::EMIT_CONTINUOUS_DEFAULT || emitter.emissionMode == EmissionMode::EMIT_BURST_DEFUALT) {
						// link to default particle
						sys.spawnParticle = p_layer->defaultParticle;
					}
				}

				ImGui::Checkbox("Auto Emit", &p_layer->emitterAutoEmit);
				ImGui::DragFloat("Emission Rate (particles/sec)", &sys.emissionRate, 0.1f, 0.0f, 2000.0f);
				ImGui::DragInt("Emitter Max Particles", &sys.maxParticles, 1, 0, 10000);
				ImGui::DragInt("Manual Emit Count", &p_layer->emitterManualEmitCount, 1, 1, p_layer->maxParticleCount / 2);


				if (ImGui::Button("Emit Manual")) {
				}
				ImGui::SameLine();

				if (ImGui::Button("Reset Emitter")) {
					emitter.Reset(p_layer->view_size, &p_layer->defaultParticle, &p_layer->defaultParticle);
				}
				ImGui::SameLine();

				if (ImGui::Button("Clear Emitter Children")) {
					sys.activeParticles.clear();
				}

				ImGui::TreePop();
			}

			// Keep emitter linked to default particle
			emitter.system.spawnParticle = p_layer->defaultParticle;
			emitter.system.emitterParticle = p_layer->defaultParticle;

			ImGui::End();
		}

		virtual void OnUpdate(float ts) override
		{
			if (!p_layer) return;
			p_layer->rp_emitter.system.spawnParticle = p_layer->defaultParticle;
			p_layer->rp_emitter.system.emitterParticle = p_layer->defaultParticle;
		}
	};




}