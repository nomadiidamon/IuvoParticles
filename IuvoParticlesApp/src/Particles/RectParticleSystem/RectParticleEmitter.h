#pragma once
#include "../RectParticle/RectParticleUtils.h"
#include "../RectParticleSystem/RectParticleSystem.h"

namespace Particles {

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
            system.viewSize = viewSize;
            system.emitterParticle.rp_transform.p_position = pos;
            system.spawnParticle.rp_transform.p_position = pos;
        }

        void SetViewportSize(const ImVec2& viewSize) {
            system.viewSize = viewSize;
        }

        void SetEmissionMode(EmissionMode mode) {
            emissionMode = mode;
        }

        void Update(float dt, const ImVec2& viewSize, const ImVec2& pos) {
            //SetPosition(viewSize, pos);
            system.viewSize = viewSize;
            Emit(dt);
            for (auto& p : system.activeParticles)
                RectParticleUtils::UpdateParticle(p, dt, viewSize, false, &system.spawnParticle);
            RectParticleUtils::RestrictParticles(system.activeParticles, system.maxParticles, viewSize);
        }

        void Draw(ImDrawList* drawList, const ImVec2& viewPos) {
            for (const auto& p : system.activeParticles)
                RectParticleUtils::DrawParticle(drawList, p, viewPos);
        }

        void Emit(float dt) {
            system.emissionAccumulator += dt * system.emissionRate;
            while (system.emissionAccumulator >= 1.0f &&
                (int)system.activeParticles.size() < system.maxParticles)
            {
                switch (emissionMode) {
                case EmissionMode::EMIT_CONTINUOUS_DEFAULT:
                    system.activeParticles.push_back(RectParticleUtils::CreateDefaultParticle(
                        system.viewSize, &system.spawnParticle));
                    break;
                case EmissionMode::EMIT_CONTINUOUS_RANDOM:
                    system.activeParticles.push_back(
                        RectParticleUtils::CreateRandomParticle(system.viewSize,
                            system.emitterParticle.rp_transform.p_position,
                            &system.spawnParticle));
                    break;
                case EmissionMode::EMIT_BURST_DEFUALT:
                    system.activeParticles.push_back(RectParticleUtils::CreateDefaultParticle(
                        system.viewSize, &system.spawnParticle));
                    break;
                case EmissionMode::EMIT_BURST_RANDOM:
                    system.activeParticles.push_back(
                        RectParticleUtils::CreateRandomParticle(system.viewSize,
                            system.emitterParticle.rp_transform.p_position,
                            &system.spawnParticle));
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


}