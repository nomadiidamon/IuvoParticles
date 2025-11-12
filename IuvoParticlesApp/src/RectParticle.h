#pragma once
#include <algorithm>
#include <random>

namespace Particles {

	static std::mt19937& GetRNG() {
		static std::random_device rd;
		static std::mt19937 mt(rd());
		return mt;
	}
	static float RandFloat(float a, float b) {
		std::uniform_real_distribution<float> dist(a, b);
		return dist(GetRNG());
	}
	static int RandInt(int a, int b) {
		std::uniform_int_distribution<int> dist(a, b);
		return dist(GetRNG());
	}

	struct ParticleColor {
		ImVec4 p_startColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		ImVec4 p_endColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
		ImVec4 p_currColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		float  p_lerpSpeed = 0.25f;

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
		ImVec2 p_position = ImVec2(0.0f, 0.0f);
		ImVec2 p_center = ImVec2(0.0f, 0.0f);

		ImVec2 p_size = ImVec2(50.0f, 50.0f);
		ImVec2 p_baseSize = ImVec2(50.0f, 50.0f); // store base size to compute non-destructive scaling each frame
		ImVec2 p_rotation = ImVec2(0.0f, 0.0f); // using x as angle in degrees for 2D rotation

		bool p_randomSize = false;
		bool p_randomRotation = false;
	};

	struct ParticleAnimation2D {
		ImVec2 p_velocity = ImVec2(0.0f, 0.0f);

		float p_movementSpeed = 0.35f;
		float p_rotationSpeed = 0.35f; // degrees per second
		float p_scaleSpeed = 0.35f;

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

    class RectParticleController {
    public:
        RectParticleController(RectParticle& p) : particle(p) {}

        // single-frame update. Mirrors old UpdateParticle semantics.
        void Update(float ts, const ImVec2& view_size, bool useDefaultParticle = false, const RectParticle* defaultParticle = nullptr) {
            if (IsExpired()) {
                Reset(view_size, useDefaultParticle, defaultParticle);
                return;
            }

            // bounds check — account for size
            if (HitViewportBounds(view_size)) {
                ApplyReboundForce(view_size);
            }

            if (particle.rp_animation.canMove) ApplyVelocity(ts);
            if (particle.rp_animation.canRotate) ApplyRotation(ts);
            if (particle.rp_animation.canScale) ApplyScale(ts);

            UpdateLifetime(ts);
            UpdateColor();
        }

        // Expose smaller operations so external code can call granular steps (keeps public for users)
        void ApplyVelocity(float ts) {
            particle.rp_transform.p_position.x += particle.rp_animation.p_velocity.x * ts * particle.rp_animation.p_movementSpeed;
            particle.rp_transform.p_position.y += particle.rp_animation.p_velocity.y * ts * particle.rp_animation.p_movementSpeed;
            UpdateCenter();
        }

        void ApplyRotation(float ts) {
            particle.rp_transform.p_rotation.x += particle.rp_animation.p_rotationSpeed * ts;
            NormalizeRotation();
        }

        void ApplyScale(float ts) {
            // Non-destructive oscillating scale around base size (0.5 .. 1.5)
            float scaleAmount = sinf(particle.rp_lifetimeData.p_lifetime * particle.rp_animation.p_scaleSpeed) * 0.5f + 1.0f;
            particle.rp_transform.p_size.x = particle.rp_transform.p_baseSize.x * scaleAmount;
            particle.rp_transform.p_size.y = particle.rp_transform.p_baseSize.y * scaleAmount;
            UpdateCenter();
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

        // Reset the particle (uses original semantics)
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

        // Expose fade/lerp color result as ImU32 for immediate drawing
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

        // rebound and keep center consistent
        bool ApplyReboundForce(const ImVec2& view_size) {
            bool rebounded = false;
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
            if (rebounded) UpdateCenter();
            return rebounded;
        }

        // helpers
        void UpdateCenter() {
            particle.rp_transform.p_center = ImVec2(particle.rp_transform.p_position.x + particle.rp_transform.p_size.x * 0.5f,
                particle.rp_transform.p_position.y + particle.rp_transform.p_size.y * 0.5f);
        }

        void NormalizeRotation() {
            while (particle.rp_transform.p_rotation.x >= 360.0f) particle.rp_transform.p_rotation.x -= 360.0f;
            while (particle.rp_transform.p_rotation.x < 0.0f) particle.rp_transform.p_rotation.x += 360.0f;
        }

		// Sets the center and offsets position accordingly
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
            return out;
        }

        static RectParticle CreateRandomInternal(const ImVec2& view_size, const RectParticle* templateParticle) {
            RectParticle particle;
            if (templateParticle) {
                particle.rp_transform.p_randomSize = templateParticle->rp_transform.p_randomSize;
                particle.rp_transform.p_randomRotation = templateParticle->rp_transform.p_randomRotation;
                particle.rp_transform.p_baseSize = templateParticle->rp_transform.p_baseSize;
                particle.rp_animation = templateParticle->rp_animation;
                particle.rp_colorData = templateParticle->rp_colorData;
                particle.rp_lifetimeData = templateParticle->rp_lifetimeData;
            }

            // position
            particle.rp_transform.p_position = ImVec2(RandFloat(0.0f, view_size.x), RandFloat(0.0f, view_size.y));

            // random size
            if (particle.rp_transform.p_randomSize) {
                float sx = RandFloat(5.0f, 20.0f);
                float sy = RandFloat(5.0f, 20.0f);
                particle.rp_transform.p_baseSize = ImVec2(sx, sy);
                particle.rp_transform.p_size = particle.rp_transform.p_baseSize;
            }
            else {
                if (particle.rp_transform.p_baseSize.x <= 0.0f || particle.rp_transform.p_baseSize.y <= 0.0f)
                    particle.rp_transform.p_baseSize = ImVec2(10.0f, 10.0f);
                particle.rp_transform.p_size = particle.rp_transform.p_baseSize;
            }

            // rotation
            if (particle.rp_transform.p_randomRotation) {
                particle.rp_transform.p_rotation.x = RandFloat(0.0f, 360.0f);
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
            particle.rp_animation.p_movementSpeed = RandFloat(20.0f, 100.0f);
            particle.rp_animation.p_rotationSpeed = RandFloat(-90.0f, 90.0f);

            // lifetime
            particle.rp_lifetimeData.p_lifetime = 0.0f;
            if (!particle.rp_lifetimeData.p_randomizeLifetime && particle.rp_lifetimeData.p_maxLifetime <= 0.0f) {
                particle.rp_lifetimeData.p_maxLifetime = RandFloat(1.0f, 4.0f);
            }
            else if (particle.rp_lifetimeData.p_randomizeLifetime) {
                particle.rp_lifetimeData.p_maxLifetime = RandFloat(0.5f, 4.0f);
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
                    particle.rp_colorData.p_currColor = ImVec4(RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), 1.0f);
                }
            }

            return particle;
        }

    private:
        RectParticle& particle;
    };


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
            if (defaultParticle) {
                return RectParticleController::CreateFromTemplate(*defaultParticle);
            }
            else {
                RectParticle out;
                out.rp_transform.p_position = ImVec2(view_size.x * 0.5f, view_size.y * 0.5f);
                out.rp_transform.p_size = ImVec2(10.0f, 10.0f);
                out.rp_transform.p_baseSize = out.rp_transform.p_size;
                out.rp_transform.p_randomSize = false;
                out.rp_transform.p_randomRotation = false;
                out.rp_animation.p_velocity = ImVec2(0.0f, -1.0f);
                out.rp_animation.p_movementSpeed = 50.0f;
                out.rp_animation.canMove = true;
                out.rp_colorData.p_startColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                out.rp_colorData.p_endColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
                out.rp_colorData.p_lerpColor = true;
                out.rp_colorData.p_useAlpha = true;
                out.rp_lifetimeData.p_maxLifetime = 2.0f;
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

    struct RectParticleCache {
        ImVec2 cachedCenter = ImVec2(0.0f, 0.0f);
        ImVec2 cachedCorners[4];    // rotated rect corners
        ImU32  cachedColor = 0;     // current display color (as U32)
        float  cachedLifetimeRatio = 0.0f;
        bool   isExpired = false;

        void UpdateFromParticle(const RectParticle& particle) {
            // Lifetime ratio
            cachedLifetimeRatio = particle.rp_lifetimeData.p_maxLifetime > 0.0f ?
                particle.rp_lifetimeData.p_lifetime / particle.rp_lifetimeData.p_maxLifetime : 0.0f;
            cachedLifetimeRatio = std::clamp(cachedLifetimeRatio, 0.0f, 1.0f);

            // Expiration
            isExpired = particle.rp_lifetimeData.p_lifetime >= particle.rp_lifetimeData.p_maxLifetime;

            // Center
            cachedCenter = ImVec2(
                particle.rp_transform.p_position.x + particle.rp_transform.p_size.x * 0.5f,
                particle.rp_transform.p_position.y + particle.rp_transform.p_size.y * 0.5f
            );

            // Color (already updated in utils update)
            cachedColor = ImGui::ColorConvertFloat4ToU32(particle.rp_colorData.p_currColor);

            // Rotated corners
            RectParticleUtils::CalculateRotatedRectCorners(particle, cachedCorners);
        }
    };

	struct BackgroundParticle {
		ParticleColor bg_colorData;
		ParticleLifetime bg_lifetimeData;
	};

	static class BackgroundParticleUtils {

	};

	enum EmissionMode {
		EMIT_CONTINUOUS_DEFAULT,
		EMIT_CONTINUOUS_RANDOM,
		EMIT_BURST_DEFUALT,
		EMIT_BURST_RANDOM
	};

    struct ParticleSystem {
        Particles::RectParticle emitterParticle;
        Particles::RectParticle spawnParticle;
        std::vector<Particles::RectParticle> activeParticles;
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

    class RectParticleEmitter {
    public:
        ParticleSystem system;
		EmissionMode emissionMode = EMIT_BURST_DEFUALT;

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
				case EMIT_CONTINUOUS_DEFAULT:
                    system.activeParticles.push_back(RectParticleUtils::CreateDefaultParticle(
						system.viewSize, &system.spawnParticle));
					break;
				case EMIT_CONTINUOUS_RANDOM:
					system.activeParticles.push_back(
						RectParticleUtils::CreateRandomParticle(system.viewSize,
							system.emitterParticle.rp_transform.p_position,
							&system.spawnParticle));
                    break;
				case EMIT_BURST_DEFUALT:
					system.activeParticles.push_back(RectParticleUtils::CreateDefaultParticle(
						system.viewSize, &system.spawnParticle));
					break;
				case EMIT_BURST_RANDOM:
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