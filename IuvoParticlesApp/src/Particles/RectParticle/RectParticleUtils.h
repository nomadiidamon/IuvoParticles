#pragma once
#include "RectParticle.h"
#include "RectParticleController.h"
#include "../ParticleConfig.h"
#include "../ParticleRandom.h"

namespace Particles {

    static class RectParticleUtils {
    public:
        // --- Creation API
        // New: large "full" entry point with many parameters for creation

        static RectParticle CreateParticleFull(
            const ImVec2& view_size,
            const ImVec2& position,
            const ImVec2& size,
            const ImVec2& baseSize,
            bool randomSize,
            bool randomRotation,
            const ImVec2& velocity,
            float movementSpeed,
            float rotationSpeed,
            float scaleSpeed,
            bool canMove,
            bool canRotate,
            bool canScale,
            const ImVec4& startColor,
            const ImVec4& endColor,
            const ImVec4& currColor,
            bool lerpColor,
            bool fadeColor,
            bool useAlpha,
            bool randomizeColor,
            float lifetime,
            float maxLifetime,
            bool randomizeLifetime,
            bool loopLifetime)
        {
            RectParticle p;
            p.rp_transform.p_position = position;
            p.rp_transform.p_size = size;
            p.rp_transform.p_baseSize = (baseSize.x > 0.0f && baseSize.y > 0.0f) ? baseSize : size;
            p.rp_transform.p_randomSize = randomSize;
            p.rp_transform.p_randomRotation = randomRotation;
            p.rp_animation.p_velocity = velocity;
            p.rp_animation.p_movementSpeed = movementSpeed;
            p.rp_animation.p_rotationSpeed = rotationSpeed;
            p.rp_animation.p_scaleSpeed = scaleSpeed;
            p.rp_animation.canMove = canMove;
            p.rp_animation.canRotate = canRotate;
            p.rp_animation.canScale = canScale;
            p.rp_colorData.p_startColor = startColor;
            p.rp_colorData.p_endColor = endColor;
            p.rp_colorData.p_currColor = currColor;
            p.rp_colorData.p_lerpColor = lerpColor;
            p.rp_colorData.p_fadeColor = fadeColor;
            p.rp_colorData.p_useAlpha = useAlpha;
            p.rp_colorData.p_randomizeColor = randomizeColor;
            p.rp_lifetimeData.p_lifetime = lifetime;
            p.rp_lifetimeData.p_maxLifetime = maxLifetime;
            p.rp_lifetimeData.p_randomizeLifetime = randomizeLifetime;
            p.rp_lifetimeData.p_loop = loopLifetime;

            p.rp_transform.p_center = ImVec2(p.rp_transform.p_position.x + p.rp_transform.p_size.x * 0.5f,
                p.rp_transform.p_position.y + p.rp_transform.p_size.y * 0.5f);

            return p;
        }

        // Existing convenience functions retained, now delegating to CreateParticleFull or controller helpers

        static RectParticle CreateRandomParticle(const ImVec2& view_size,
            const ImVec2& position = ImVec2(0.0f, 0.0f),
            const RectParticle* templateParticle = nullptr)
        {
            // If caller supplied a position use it; otherwise CreateRandomInternal will pick a random position
            if (position.x != 0.0f || position.y != 0.0f) {
                RectParticle p = RectParticleController::CreateRandomInternal(view_size, templateParticle);
                p.rp_transform.p_position = position;
                p.rp_transform.p_center = ImVec2(position.x + p.rp_transform.p_size.x * 0.5f, position.y + p.rp_transform.p_size.y * 0.5f);
                return p;
            }
            else {
                return RectParticleController::CreateRandomInternal(view_size, templateParticle);
            }
        }

        static RectParticle CreateDefaultParticle(const ImVec2& view_size, const RectParticle* defaultParticle = nullptr) {
            const auto& cfg = GetParticleConfig();
            if (defaultParticle) {
                return RectParticleController::CreateFromTemplate(*defaultParticle);
            }
            else {
                // If a configured spawn template exists, use it
                if (cfg.defaultSpawnTemplate.rp_transform.p_size.x > 0.0f && cfg.defaultSpawnTemplate.rp_transform.p_size.y > 0.0f) {
                    return RectParticleController::CreateFromTemplate(cfg.defaultSpawnTemplate);
                }

                // fallback to a basic default that uses config defaults
                RectParticle out;
                out.rp_transform.p_position = ImVec2(view_size.x * 0.5f, view_size.y * 0.5f);
                out.rp_transform.p_size = cfg.defaultSpawnSize;
                out.rp_transform.p_baseSize = out.rp_transform.p_size;
                out.rp_transform.p_randomSize = false;
                out.rp_transform.p_randomRotation = false;
                out.rp_animation.p_velocity = cfg.defaultVelocity;
                out.rp_animation.p_movementSpeed = cfg.defaultMovementSpeed;
                out.rp_animation.canMove = cfg.defaultCanMove;
                out.rp_colorData.p_startColor = cfg.defaultStartColor;
                out.rp_colorData.p_endColor = cfg.defaultEndColor;
                out.rp_colorData.p_lerpColor = true;
                out.rp_colorData.p_useAlpha = true;
                out.rp_lifetimeData.p_maxLifetime = (cfg.lifetimeMin + cfg.lifetimeMax) * 0.5f;
                out.rp_lifetimeData.p_randomizeLifetime = false;
                out.rp_transform.p_center = ImVec2(out.rp_transform.p_position.x + out.rp_transform.p_size.x * 0.5f,
                    out.rp_transform.p_position.y + out.rp_transform.p_size.y * 0.5f);
                return out;
            }
        }

        static RectParticle CreateParticle(const ParticleTransform& p_transform, const ParticleColor& p_color, const ParticleLifetime& p_lifetime) {
            RectParticle particle;
            particle.rp_transform = p_transform;
            if (particle.rp_transform.p_baseSize.x <= 0.0f || particle.rp_transform.p_baseSize.y <= 0.0f)
                particle.rp_transform.p_baseSize = particle.rp_transform.p_size;
            particle.rp_colorData = p_color;
            particle.rp_lifetimeData = p_lifetime;
            particle.rp_transform.p_center = ImVec2(p_transform.p_position.x + p_transform.p_size.x * 0.5f,
                p_transform.p_position.y + p_transform.p_size.y * 0.5f);
            return particle;
        }

        static void ResetParticle(RectParticle& particle, const ImVec2& view_size, bool useDefaultParticle = false, const RectParticle* defaultParticle = nullptr) {
            RectParticleController controller(particle);
            controller.Reset(view_size, useDefaultParticle, defaultParticle);
        }

        static bool IsExpired(const RectParticle& particle) {
            return particle.rp_lifetimeData.p_lifetime >= particle.rp_lifetimeData.p_maxLifetime;
        }

        static bool IsEqual(const RectParticle& a, const RectParticle& b) {
            constexpr float EPS = 1e-6f;
            auto feq = [&](float x, float y) { return fabsf(x - y) <= EPS; };
            return feq(a.rp_transform.p_position.x, b.rp_transform.p_position.x) &&
                feq(a.rp_transform.p_position.y, b.rp_transform.p_position.y) &&
                feq(a.rp_transform.p_size.x, b.rp_transform.p_size.x) &&
                feq(a.rp_transform.p_size.y, b.rp_transform.p_size.y) &&
                feq(a.rp_colorData.p_startColor.x, b.rp_colorData.p_startColor.x) &&
                feq(a.rp_colorData.p_startColor.y, b.rp_colorData.p_startColor.y) &&
                feq(a.rp_colorData.p_startColor.z, b.rp_colorData.p_startColor.z) &&
                feq(a.rp_colorData.p_endColor.x, b.rp_colorData.p_endColor.x) &&
                feq(a.rp_colorData.p_endColor.y, b.rp_colorData.p_endColor.y) &&
                feq(a.rp_colorData.p_endColor.z, b.rp_colorData.p_endColor.z) &&
                feq(a.rp_lifetimeData.p_maxLifetime, b.rp_lifetimeData.p_maxLifetime);
        }

        // Delegation: apply velocity/rotation/scale via controller
        static void ApplyVelocity(RectParticle& particle, float ts) {
            RectParticleController ctrl(particle);
            ctrl.ApplyVelocity(ts);
        }

        static void ApplyRotation(RectParticle& particle, float ts) {
            RectParticleController ctrl(particle);
            ctrl.ApplyRotation(ts);
        }

        static void ApplyScale(RectParticle& particle, float ts) {
            RectParticleController ctrl(particle);
            ctrl.ApplyScale(ts);
        }

        static void CalculateRotatedRectCorners(const RectParticle& particle, ImVec2 outCorners[4]) {
            RectParticleController::CalculateRotatedRectCorners(particle, outCorners);
        }

        static void UpdateRotation(RectParticle& particle, float ts) {
            if (particle.rp_animation.canRotate) ApplyRotation(particle, ts);
        }

        static void UpdateAnimation2D(RectParticle& particle, float ts) {
            RectParticleController ctrl(const_cast<RectParticle&>(particle));
            if (particle.rp_animation.canMove) ctrl.ApplyVelocity(ts);
            if (particle.rp_animation.canRotate) ctrl.ApplyRotation(ts);
            if (particle.rp_animation.canScale) ctrl.ApplyScale(ts);
        }

        static void UpdateLifetime(RectParticle& particle, float ts) {
            RectParticleController ctrl(particle);
            ctrl.UpdateLifetime(ts);
        }

        static ImU32 FadeColor(RectParticle& particle) {
            RectParticleController ctrl(particle);
            return ctrl.FadeColor();
        }

        static ImU32 LerpColor(RectParticle& particle) {
            RectParticleController ctrl(particle);
            return ctrl.LerpColor();
        }

        static void UpdateColor(RectParticle& particle) {
            RectParticleController ctrl(particle);
            ctrl.UpdateColor();
        }

        static bool HitViewportBounds(const RectParticle& particle, const ImVec2& view_size) {
            RectParticleController ctrl(const_cast<RectParticle&>(particle));
            return ctrl.HitViewportBounds(view_size);
        }

        static bool ApplyReboundForce(RectParticle& particle, const ImVec2& view_size) {
            RectParticleController ctrl(particle);
            return ctrl.ApplyReboundForce(view_size);
        }

        static void UpdateParticle(RectParticle& particle, float ts, const ImVec2& view_size, bool useDefaultParticle = false, const RectParticle* defaultParticle = nullptr) {
            RectParticleController ctrl(particle);
            ctrl.Update(ts, view_size, useDefaultParticle, defaultParticle);
        }

        static void DrawParticle(ImDrawList* draw_list, const RectParticle& particle, const ImVec2& view_pos) {
            ImU32 color = ImGui::GetColorU32(particle.rp_colorData.p_currColor);
            draw_list->AddRectFilled(ImVec2(view_pos.x + particle.rp_transform.p_position.x, view_pos.y + particle.rp_transform.p_position.y),
                ImVec2(view_pos.x + particle.rp_transform.p_position.x + particle.rp_transform.p_size.x, view_pos.y + particle.rp_transform.p_position.y + particle.rp_transform.p_size.y),
                color);
        }

        static void DrawParticle(ImDrawList* draw_list, const RectParticle& particle, const ImVec2& view_pos, const ImU32& override_color) {
            draw_list->AddRectFilled(ImVec2(view_pos.x + particle.rp_transform.p_position.x, view_pos.y + particle.rp_transform.p_position.y),
                ImVec2(view_pos.x + particle.rp_transform.p_position.x + particle.rp_transform.p_size.x, view_pos.y + particle.rp_transform.p_position.y + particle.rp_transform.p_size.y),
                override_color);
        }

        static void DrawRotatingParticle(ImDrawList* draw_list, const RectParticle& particle, const ImVec2& view_pos) {
            ImU32 color = ImGui::GetColorU32(particle.rp_colorData.p_currColor);
            ImVec2 corners[4];
            CalculateRotatedRectCorners(particle, corners);
            for (int i = 0; i < 4; ++i) {
                corners[i].x += view_pos.x;
                corners[i].y += view_pos.y;
            }
            draw_list->AddConvexPolyFilled(corners, 4, color);
        }

        // Sorting/clamping helpers unchanged
        static void SortParticlesByLifetime(std::vector<RectParticle>& particles) {
            std::sort(particles.begin(), particles.end(),
                [](const RectParticle& a, const RectParticle& b) {
                    return a.rp_lifetimeData.p_lifetime < b.rp_lifetimeData.p_lifetime;
                });
        }

        static void ClampParticleCount(std::vector<RectParticle>& particles, int maxCount, const ImVec2& /*view_size*/) {
            while ((int)particles.size() > maxCount) {
                particles.pop_back();
            }
        }

        static void RestrictParticles(std::vector<RectParticle>& particles, int maxCount, const ImVec2& view_size) {
            SortParticlesByLifetime(particles);
            ClampParticleCount(particles, maxCount, view_size);
        }

        static bool IsDefaultParticle(const RectParticle& particle) {
            ParticleColor defaultColor;
            ParticleAnimation2D defaultAnimation;
            ParticleLifetime defaultLifetime;
            ParticleTransform defaultTransform;

            auto feq = [](float a, float b) { return fabsf(a - b) <= 1e-6f; };

            bool transformDefault = feq(particle.rp_transform.p_position.x, defaultTransform.p_position.x) &&
                feq(particle.rp_transform.p_position.y, defaultTransform.p_position.y) &&
                feq(particle.rp_transform.p_size.x, defaultTransform.p_size.x) &&
                feq(particle.rp_transform.p_size.y, defaultTransform.p_size.y);

            bool animDefault = feq(particle.rp_animation.p_velocity.x, defaultAnimation.p_velocity.x) &&
                feq(particle.rp_animation.p_velocity.y, defaultAnimation.p_velocity.y) &&
                feq(particle.rp_animation.p_movementSpeed, defaultAnimation.p_movementSpeed);

            bool colorDefault = feq(particle.rp_colorData.p_currColor.x, defaultColor.p_currColor.x) &&
                feq(particle.rp_colorData.p_currColor.y, defaultColor.p_currColor.y) &&
                feq(particle.rp_colorData.p_currColor.z, defaultColor.p_currColor.z) &&
                feq(particle.rp_colorData.p_currColor.w, defaultColor.p_currColor.w);

            bool lifetimeDefault = feq(particle.rp_lifetimeData.p_lifetime, defaultLifetime.p_lifetime) &&
                feq(particle.rp_lifetimeData.p_maxLifetime, defaultLifetime.p_maxLifetime);

            return transformDefault && animDefault && colorDefault && lifetimeDefault;
        }

    } RectParticleUtils;

}