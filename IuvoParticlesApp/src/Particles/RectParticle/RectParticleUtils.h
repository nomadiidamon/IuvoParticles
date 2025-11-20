#pragma once
#include "../ParticleUtils.h"
#include "RectParticleController.h"

namespace Particles {

    class RectParticleUtils {
    public:
        static RectParticle CreateRandomParticle(const ImVec2& view_size,
            const ImVec2& position = ImVec2(-FLT_MAX, -FLT_MAX))
        {
            // If caller supplied a position, create a random particle but force that position.
            if (position.x != -FLT_MAX && position.y != -FLT_MAX) {
                RectParticle p = RectParticleSpawner::CreateRandom(view_size);
                p.rp_transform.p_position = position;
                p.rp_transform.p_center = ImVec2(position.x + p.rp_transform.p_size.x * 0.5f,
                    position.y + p.rp_transform.p_size.y * 0.5f);
                return p;
            }
            else {
                // no position supplied => return a particle with internally randomized position
                return RectParticleSpawner::CreateRandom(view_size);
            }
        }

        static RectParticle CreateDefaultParticle(const ImVec2& view_size, const ImVec2& pos = ImVec2(-FLT_MAX, -FLT_MAX), const RectParticle* defaultParticle = nullptr) {
            const auto& cfg = GetParticleConfig();
            RectParticle out;
            
            if (defaultParticle) {
                if (pos.x != -FLT_MAX && pos.y != -FLT_MAX) {
                    out = RectParticleSpawner::FromTemplate(*defaultParticle);
                    out.rp_transform.p_position = pos;
                    out.rp_transform.p_center = ImVec2(pos.x + out.rp_transform.p_size.x * 0.5f,
                        pos.y + out.rp_transform.p_size.y * 0.5f);
                    return out;
                }
                else {
                    // Use provided default template centered in the view
                    out = RectParticleSpawner::FromTemplate(*defaultParticle);
                    out.rp_transform.p_position = ImVec2(view_size.x * 0.5f, view_size.y * 0.5f);
                    // make sure center is synced
                    RectParticleController(out).UpdateCenter();
                    return out;
                }
            }
            else {
                // If a configured spawn template exists, use it
                if (cfg.defaultSpawnTemplate.rp_transform.p_size.x > 0.0f && cfg.defaultSpawnTemplate.rp_transform.p_size.y > 0.0f) {
                    //return RectParticleController::CreateFromTemplate(cfg.defaultSpawnTemplate);
                    if (pos.x != -FLT_MAX && pos.y != -FLT_MAX) {
                        out = RectParticleSpawner::FromTemplate(cfg.defaultSpawnTemplate);
                        out.rp_transform.p_position = pos;
						RectParticleController(out).UpdateCenter();
                    }
                }

                // fallback to a basic default that uses config defaults
                out.rp_transform.p_position = cfg.transform.defaultPosition;
                out.rp_transform.p_size = cfg.transform.defaultSize;
                out.rp_transform.p_baseSize = out.rp_transform.p_size;
				out.rp_transform.p_rotation = cfg.transform.defaultRotation;
                out.rp_transform.p_randomSize = cfg.defaultRandomSize;
                out.rp_transform.p_randomRotation = cfg.defaultRandomRotation;
                out.rp_animation.p_velocity = cfg.animation.defaultVelocity;
                out.rp_animation.p_movementSpeed = cfg.animation.defaultMovementSpeed;
                out.rp_animation.canMove = cfg.defaultCanMove;
                out.rp_colorData.p_startColor = cfg.color.defaultStartColor;
                out.rp_colorData.p_endColor = cfg.color.defaultEndColor;
                out.rp_colorData.p_lerpColor = cfg.defaultLerpColor;
                out.rp_colorData.p_useAlpha = cfg.defaultUseAlpha;
                out.rp_lifetimeData.p_maxLifetime = cfg.lifetime.defaultMaxLifetime;
                out.rp_lifetimeData.p_randomizeLifetime = cfg.defaultRandomizeLifetime;
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

		static RectParticle CreateParticle(const ParticleTransform& p_transform,
			const ParticleAnimation2D& p_animation,
			const ParticleColor& p_color,
			const ParticleLifetime& p_lifetime) {
			RectParticle particle;
			particle.rp_transform = p_transform;
			if (particle.rp_transform.p_baseSize.x <= 0.0f || particle.rp_transform.p_baseSize.y <= 0.0f)
				particle.rp_transform.p_baseSize = particle.rp_transform.p_size;
			particle.rp_animation = p_animation;
			particle.rp_colorData = p_color;
			particle.rp_colorData.p_currColor = p_color.p_startColor;
			particle.rp_lifetimeData = p_lifetime;
			particle.rp_transform.p_center = ImVec2(p_transform.p_position.x + p_transform.p_size.x * 0.5f,
				p_transform.p_position.y + p_transform.p_size.y * 0.5f);
			return particle;
		}

		static void CreateRadialParticleBurst(const ImVec2& viewSize, const ImVec2 burstPos, int burstCount,
            std::vector<RectParticle>& container, const RectParticle* templateParticle = nullptr, bool randomColor = true, bool randColorRange = false)
        {
			if (templateParticle == nullptr) {
				RectParticle defaultParticle = CreateDefaultParticle(viewSize, burstPos);
				templateParticle = &defaultParticle;
			}
			for (int i = 0; i < burstCount; ++i) {
				RectParticle p = CreateParticle(templateParticle->rp_transform, templateParticle->rp_animation, 
                    templateParticle->rp_colorData, templateParticle->rp_lifetimeData);

				// Random radial velocity
				float angle = (static_cast<float>(rand()) / RAND_MAX) * 2.0f * 3.14159265358979323846f;
				float minSpeed = 200.0f;
				float maxSpeed = 300.0f;
				float speed = minSpeed + (static_cast<float>(rand()) / RAND_MAX) * (maxSpeed - minSpeed);
				p.rp_animation.p_velocity = ImVec2(cosf(angle), sinf(angle));
				p.rp_animation.p_movementSpeed = speed;
				p.rp_animation.canMove = true;

                // random lifetime around base
                float baseLifetime = p.rp_lifetimeData.p_maxLifetime > 0.0f ? p.rp_lifetimeData.p_maxLifetime : 10.0f;
				float randLifetime = baseLifetime * RandFloat(0.75f, 5.5f);
                p.rp_lifetimeData.p_maxLifetime = randLifetime;
                p.rp_lifetimeData.p_lifetime = 0.0f;

                if (randColorRange) {
                    auto& p_cfg = GetParticleConfig();
					p.rp_colorData.p_startColor = ParticleColor::CreateRandomColorInRange(
						p_cfg.color.startColorMin, p_cfg.color.startColorMax).p_startColor;
					p.rp_colorData.p_endColor = ParticleColor::CreateRandomColorInRange(
						p_cfg.color.endColorMin, p_cfg.color.endColorMax).p_startColor;
                }
				else if (randomColor) {
					p.rp_colorData.p_startColor = ParticleColor::CreateRandomColor().p_startColor;
					p.rp_colorData.p_endColor = ParticleColor::CreateRandomColor().p_startColor;
				}
				// set color lerp/fade
				p.rp_colorData.p_currColor = p.rp_colorData.p_startColor;
				p.rp_colorData.p_lerpColor = true;
				p.rp_colorData.p_useAlpha = true;
				p.rp_colorData.p_endColor.w = 0.0f; // fade out
                container.push_back(std::move(p));
				//container.push_back(p);
			}
		}
        static bool IsExpired(const RectParticle& particle) {
            return particle.rp_lifetimeData.p_lifetime >= particle.rp_lifetimeData.p_maxLifetime;
        }
		static float LifetimeRatio(const RectParticle& particle) {
			if (particle.rp_lifetimeData.p_maxLifetime <= 0.0f)
				return 0.0f;
			return std::clamp(particle.rp_lifetimeData.p_lifetime / particle.rp_lifetimeData.p_maxLifetime, 0.0f, 1.0f);
		}
        static bool IsEqualKeyAttributes(const RectParticle& a, const RectParticle& b) {
            return FloatEquals(a.rp_transform.p_position.x, b.rp_transform.p_position.x) &&
                FloatEquals(a.rp_transform.p_position.y, b.rp_transform.p_position.y) &&
                FloatEquals(a.rp_transform.p_size.x, b.rp_transform.p_size.x) &&
                FloatEquals(a.rp_transform.p_size.y, b.rp_transform.p_size.y) &&
                FloatEquals(a.rp_colorData.p_startColor.x, b.rp_colorData.p_startColor.x) &&
                FloatEquals(a.rp_colorData.p_startColor.y, b.rp_colorData.p_startColor.y) &&
                FloatEquals(a.rp_colorData.p_startColor.z, b.rp_colorData.p_startColor.z) &&
                FloatEquals(a.rp_colorData.p_endColor.w, b.rp_colorData.p_endColor.w) &&
                FloatEquals(a.rp_colorData.p_endColor.x, b.rp_colorData.p_endColor.x) &&
                FloatEquals(a.rp_colorData.p_endColor.y, b.rp_colorData.p_endColor.y) &&
                FloatEquals(a.rp_colorData.p_endColor.z, b.rp_colorData.p_endColor.z) &&
                FloatEquals(a.rp_colorData.p_lerpSpeed, b.rp_colorData.p_lerpSpeed) &&
                FloatEquals(a.rp_lifetimeData.p_maxLifetime, b.rp_lifetimeData.p_maxLifetime);
        }
        static void CalculateRotatedRectCorners(const RectParticle& particle, ImVec2 outCorners[4]) {
            RectParticleMotionController::ComputeRotatedCorners(particle, outCorners);
        }
        static void UpdateParticle(RectParticle& particle, float ts, const ImVec2& view_size, std::vector<RectParticle>& container, bool useDefaultParticle = false, const RectParticle* defaultParticle = nullptr) {
			RectParticleController(particle).Update(ts, view_size, container, useDefaultParticle, defaultParticle);
        }
		static void DrawParticleAgnostic(ImDrawList* draw_list, const RectParticle& particle, const ImVec2& view_pos) {
			bool rotated = (particle.rp_animation.canRotate && particle.rp_transform.p_rotation.x != 0.0f);

			if (rotated) {
				DrawRotatingParticle(draw_list, particle, view_pos);
			}
			else {
				DrawParticle(draw_list, particle, view_pos);
			}
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
		static bool ContainsParticle(const std::vector<RectParticle>& particles, const RectParticle& target) {
			for (const auto& p : particles) {
				if (IsEqualKeyAttributes(p, target)) {
					return true;
				}
			}
			return false;
		}
		static bool RemoveIfExists(std::vector<RectParticle>& particles, const RectParticle& target) {
			if (ContainsParticle(particles, target)) {
				return true;
			}
			return false;
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

        /// TODO: update this to use the default sub structs in ParticleTypes.h
        static bool IsTrueDefaultParticle(const RectParticle& particle) {
  
            bool transformDefault = FloatEquals(particle.rp_transform.p_position.x, ParticleTransform::DEFAULT_TRANSFORM.p_position.x) &&
                FloatEquals(particle.rp_transform.p_position.y, ParticleTransform::DEFAULT_TRANSFORM.p_position.y) &&
                FloatEquals(particle.rp_transform.p_size.x, ParticleTransform::DEFAULT_TRANSFORM.p_size.x) &&
                FloatEquals(particle.rp_transform.p_size.y, ParticleTransform::DEFAULT_TRANSFORM.p_size.y);

            bool animDefault = FloatEquals(particle.rp_animation.p_velocity.x, ParticleAnimation2D::DEFAULT_ANIMATION.p_velocity.x) &&
                FloatEquals(particle.rp_animation.p_velocity.y, ParticleAnimation2D::DEFAULT_ANIMATION.p_velocity.y) &&
                FloatEquals(particle.rp_animation.p_movementSpeed, ParticleAnimation2D::DEFAULT_ANIMATION.p_movementSpeed);

            bool colorDefault = FloatEquals(particle.rp_colorData.p_currColor.x, ParticleColor::DEFAULT_COLOR.p_currColor.x) &&
                FloatEquals(particle.rp_colorData.p_currColor.y, ParticleColor::DEFAULT_COLOR.p_currColor.y) &&
                FloatEquals(particle.rp_colorData.p_currColor.z, ParticleColor::DEFAULT_COLOR.p_currColor.z) &&
                FloatEquals(particle.rp_colorData.p_currColor.w, ParticleColor::DEFAULT_COLOR.p_currColor.w);

            bool lifetimeDefault = FloatEquals(particle.rp_lifetimeData.p_lifetime, ParticleLifetime::DEFAULT_LIFETIME.p_lifetime) &&
                FloatEquals(particle.rp_lifetimeData.p_maxLifetime, ParticleLifetime::DEFAULT_LIFETIME.p_maxLifetime);

            return transformDefault && animDefault && colorDefault && lifetimeDefault;
        }

		static bool IsSameParticle(const RectParticle* a, const RectParticle& b) {
			return &b == a;
		}


    } RectParticleUtils;

}