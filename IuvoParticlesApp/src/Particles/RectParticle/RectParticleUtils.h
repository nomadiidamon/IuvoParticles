#pragma once
#include "../ParticleUtils.h"
#include "../../ParticleLayers/LayerConfig.h"
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
                p.transform.position = position;
                p.transform.center = ImVec2(position.x + p.transform.size.x * 0.5f,
                    position.y + p.transform.size.y * 0.5f);
                return p;
            }
            else {
                // no position supplied => return a particle with internally randomized position
                return RectParticleSpawner::CreateRandom(view_size);
            }
        }

        static RectParticle CreateDefaultParticle(const ImVec2& view_size, const ImVec2& pos = ImVec2(-FLT_MAX, -FLT_MAX), const RectParticle* defaultParticle = nullptr) {
            const auto& cfg = GetParticleConfig();
			auto& p_cfg = GetLayerConfig();
            RectParticle out;
            
            if (defaultParticle) {
                if (pos.x != -FLT_MAX && pos.y != -FLT_MAX) {
                    out = RectParticleSpawner::FromTemplate(*defaultParticle);
                    out.transform.position = pos;
                    out.transform.center = ImVec2(pos.x + out.transform.size.x * 0.5f,
                        pos.y + out.transform.size.y * 0.5f);
                    return out;
                }
                else {
                    // Use provided default template centered in the view
                    out = RectParticleSpawner::FromTemplate(*defaultParticle);
                    out.transform.position = ImVec2(view_size.x * 0.5f, view_size.y * 0.5f);
                    // make sure center is synced
                    RectParticleController(out).UpdateCenter();
                    return out;
                }
            }
            else {
                // If a configured spawn template exists, use it
                if (p_cfg.defaultParticle.transform.size.x > 0.0f && p_cfg.defaultParticle.transform.size.y > 0.0f) {
                    //return RectParticleController::CreateFromTemplate(cfg.defaultSpawnTemplate);
                    if (pos.x != -FLT_MAX && pos.y != -FLT_MAX) {
                        out = RectParticleSpawner::FromTemplate(p_cfg.defaultParticle);
                        out.transform.position = pos;
						RectParticleController(out).UpdateCenter();
                    }
                }

                // fallback to a basic default that uses config defaults
                out.transform.position = cfg.transform.defaultPosition;
                out.transform.size = cfg.transform.defaultSize;
                out.transform.baseSize = out.transform.size;
				out.transform.rotation = cfg.transform.defaultRotation;
                out.transform.randomSize = cfg.defaultRandomSize;
                out.transform.randomRotation = cfg.defaultRandomRotation;
                out.animation.p_velocity = cfg.animation.defaultVelocity;
                out.animation.p_movementSpeed = cfg.animation.defaultMovementSpeed;
                out.animation.canMove = cfg.defaultCanMove;
                out.color.startColor = cfg.color.defaultStartColor;
                out.color.endColor = cfg.color.defaultEndColor;
                out.color.lerpColor = cfg.defaultLerpColor;
                out.color.useAlpha = cfg.defaultUseAlpha;
                out.lifetime.maxLifetime = cfg.lifetime.defaultMaxLifetime;
                out.lifetime.randomizeLifetime = cfg.defaultRandomizeLifetime;
                out.transform.center = ImVec2(out.transform.position.x + out.transform.size.x * 0.5f,
                    out.transform.position.y + out.transform.size.y * 0.5f);
                return out;
            }
        }

        static RectParticle CreateParticle(const ParticleTransform& p_transform, const ParticleColor& p_color, const ParticleLifetime& p_lifetime) {
            RectParticle particle;
            particle.transform = p_transform;
            if (particle.transform.baseSize.x <= 0.0f || particle.transform.baseSize.y <= 0.0f)
                particle.transform.baseSize = particle.transform.size;
            particle.color = p_color;
            particle.lifetime = p_lifetime;
            particle.transform.center = ImVec2(p_transform.position.x + p_transform.size.x * 0.5f,
                p_transform.position.y + p_transform.size.y * 0.5f);
            return particle;
        }

		static RectParticle CreateParticle(const ParticleTransform& p_transform,
			const ParticleAnimation2D& p_animation,
			const ParticleColor& p_color,
			const ParticleLifetime& p_lifetime) {
			RectParticle particle;
			particle.transform = p_transform;
			if (particle.transform.baseSize.x <= 0.0f || particle.transform.baseSize.y <= 0.0f)
				particle.transform.baseSize = particle.transform.size;
			particle.animation = p_animation;
			particle.color = p_color;
			particle.color.currColor = p_color.startColor;
			particle.lifetime = p_lifetime;
			particle.transform.center = ImVec2(p_transform.position.x + p_transform.size.x * 0.5f,
				p_transform.position.y + p_transform.size.y * 0.5f);
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
				RectParticle p = CreateParticle(templateParticle->transform, templateParticle->animation, 
                    templateParticle->color, templateParticle->lifetime);

				// Random radial velocity
				float angle = (static_cast<float>(rand()) / RAND_MAX) * 2.0f * 3.14159265358979323846f;
				float minSpeed = 200.0f;
				float maxSpeed = 300.0f;
				float speed = minSpeed + (static_cast<float>(rand()) / RAND_MAX) * (maxSpeed - minSpeed);
				p.animation.p_velocity = ImVec2(cosf(angle), sinf(angle));
				p.animation.p_movementSpeed = speed;
				p.animation.canMove = true;

                // random lifetime around base
                float baseLifetime = p.lifetime.maxLifetime > 0.0f ? p.lifetime.maxLifetime : 10.0f;
				float randLifetime = baseLifetime * RandFloat(0.75f, 5.5f);
                p.lifetime.maxLifetime = randLifetime;
                p.lifetime.lifetime = 0.0f;

                if (randColorRange) {
                    auto& p_cfg = GetParticleConfig();
					p.color.startColor = ParticleColor::CreateRandomColorInRange(
						p_cfg.color.startColorMin, p_cfg.color.startColorMax).startColor;
					p.color.endColor = ParticleColor::CreateRandomColorInRange(
						p_cfg.color.endColorMin, p_cfg.color.endColorMax).startColor;
                }
				else if (randomColor) {
					p.color.startColor = ParticleColor::CreateRandomColor().startColor;
					p.color.endColor = ParticleColor::CreateRandomColor().startColor;
				}
				// set color lerp/fade
				p.color.currColor = p.color.startColor;
				p.color.lerpColor = true;
				p.color.useAlpha = true;
				p.color.endColor.w = 0.0f; // fade out
                container.push_back(std::move(p));
				//container.push_back(p);
			}
		}
        static bool IsExpired(const RectParticle& particle) {
            return particle.lifetime.lifetime >= particle.lifetime.maxLifetime;
        }
		static float LifetimeRatio(const RectParticle& particle) {
			if (particle.lifetime.maxLifetime <= 0.0f)
				return 0.0f;
			return std::clamp(particle.lifetime.lifetime / particle.lifetime.maxLifetime, 0.0f, 1.0f);
		}
        static bool IsEqualKeyAttributes(const RectParticle& a, const RectParticle& b) {
            return FloatEquals(a.transform.position.x, b.transform.position.x) &&
                FloatEquals(a.transform.position.y, b.transform.position.y) &&
                FloatEquals(a.transform.size.x, b.transform.size.x) &&
                FloatEquals(a.transform.size.y, b.transform.size.y) &&
                FloatEquals(a.color.startColor.x, b.color.startColor.x) &&
                FloatEquals(a.color.startColor.y, b.color.startColor.y) &&
                FloatEquals(a.color.startColor.z, b.color.startColor.z) &&
                FloatEquals(a.color.endColor.w, b.color.endColor.w) &&
                FloatEquals(a.color.endColor.x, b.color.endColor.x) &&
                FloatEquals(a.color.endColor.y, b.color.endColor.y) &&
                FloatEquals(a.color.endColor.z, b.color.endColor.z) &&
                FloatEquals(a.color.lerpSpeed, b.color.lerpSpeed) &&
                FloatEquals(a.lifetime.maxLifetime, b.lifetime.maxLifetime);
        }
        static void CalculateRotatedRectCorners(const RectParticle& particle, ImVec2 outCorners[4]) {
            RectParticleMotionController::ComputeRotatedCorners(particle, outCorners);
        }
        static void UpdateParticle(RectParticle& particle, float ts, const ImVec2& view_size, std::vector<RectParticle>& container, bool useDefaultParticle = false, const RectParticle* defaultParticle = nullptr) {
			RectParticleController(particle).Update(ts, view_size, container, useDefaultParticle, defaultParticle);
        }
		static void DrawParticleAgnostic(ImDrawList* draw_list, const RectParticle& particle, const ImVec2& view_pos) {
			bool rotated = (particle.animation.canRotate && particle.transform.rotation.x != 0.0f);

			if (rotated) {
				DrawRotatingParticle(draw_list, particle, view_pos);
			}
			else {
				DrawParticle(draw_list, particle, view_pos);
			}
		}
        static void DrawParticle(ImDrawList* draw_list, const RectParticle& particle, const ImVec2& view_pos) {
            ImU32 color = ImGui::GetColorU32(particle.color.currColor);
            draw_list->AddRectFilled(ImVec2(view_pos.x + particle.transform.position.x, view_pos.y + particle.transform.position.y),
                ImVec2(view_pos.x + particle.transform.position.x + particle.transform.size.x, view_pos.y + particle.transform.position.y + particle.transform.size.y),
                color);
        }
        static void DrawParticle(ImDrawList* draw_list, const RectParticle& particle, const ImVec2& view_pos, const ImU32& override_color) {
            draw_list->AddRectFilled(ImVec2(view_pos.x + particle.transform.position.x, view_pos.y + particle.transform.position.y),
                ImVec2(view_pos.x + particle.transform.position.x + particle.transform.size.x, view_pos.y + particle.transform.position.y + particle.transform.size.y),
                override_color);
        }
        static void DrawRotatingParticle(ImDrawList* draw_list, const RectParticle& particle, const ImVec2& view_pos) {
            ImU32 color = ImGui::GetColorU32(particle.color.currColor);
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
                    return a.lifetime.lifetime < b.lifetime.lifetime;
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
  
            bool transformDefault = FloatEquals(particle.transform.position.x, ParticleTransform::DEFAULT_TRANSFORM.position.x) &&
                FloatEquals(particle.transform.position.y, ParticleTransform::DEFAULT_TRANSFORM.position.y) &&
                FloatEquals(particle.transform.size.x, ParticleTransform::DEFAULT_TRANSFORM.size.x) &&
                FloatEquals(particle.transform.size.y, ParticleTransform::DEFAULT_TRANSFORM.size.y);

            bool animDefault = FloatEquals(particle.animation.p_velocity.x, ParticleAnimation2D::DEFAULT_ANIMATION.p_velocity.x) &&
                FloatEquals(particle.animation.p_velocity.y, ParticleAnimation2D::DEFAULT_ANIMATION.p_velocity.y) &&
                FloatEquals(particle.animation.p_movementSpeed, ParticleAnimation2D::DEFAULT_ANIMATION.p_movementSpeed);

            bool colorDefault = FloatEquals(particle.color.currColor.x, ParticleColor::DEFAULT_COLOR.currColor.x) &&
                FloatEquals(particle.color.currColor.y, ParticleColor::DEFAULT_COLOR.currColor.y) &&
                FloatEquals(particle.color.currColor.z, ParticleColor::DEFAULT_COLOR.currColor.z) &&
                FloatEquals(particle.color.currColor.w, ParticleColor::DEFAULT_COLOR.currColor.w);

            bool lifetimeDefault = FloatEquals(particle.lifetime.lifetime, ParticleLifetime::DEFAULT_LIFETIME.lifetime) &&
                FloatEquals(particle.lifetime.maxLifetime, ParticleLifetime::DEFAULT_LIFETIME.maxLifetime);

            return transformDefault && animDefault && colorDefault && lifetimeDefault;
        }

		static bool IsSameParticle(const RectParticle* a, const RectParticle& b) {
			return &b == a;
		}

        static void ApplyColorPreset(RectParticle& particle, const ParticleColor presetData) {
			particle.color.startColor = presetData.startColor;
			particle.color.endColor = presetData.endColor;
			particle.color.lerpSpeed = presetData.lerpSpeed;
			particle.color.lerpColor = presetData.lerpColor;
        }

		static void ApplyColorPreset(RectParticle& particle, ParticleColorPreset preset) {
            switch (preset) {
                case ParticleColorPreset::RED_TO_YELLOW_FADE_OUT:
                    ApplyColorPreset(particle, ParticleColor::RED_TO_YELLOW_FADE_OUT);
                    break;
                case ParticleColorPreset::RED_TO_TRANSPARENT_FADE_OUT:
                    ApplyColorPreset(particle, ParticleColor::RED_TO_TRANSPARENT_FADE_OUT);
                    break;
                case ParticleColorPreset::RED_TO_BLUE_FADE_OUT:
                    ApplyColorPreset(particle, ParticleColor::RED_TO_BLUE_FADE_OUT);
                    break;
                case ParticleColorPreset::ORANGE_TO_YELLOW_FADE_OUT:
                    ApplyColorPreset(particle, ParticleColor::ORANGE_TO_YELLOW_FADE_OUT);
                    break;
                case ParticleColorPreset::ORANGE_TO_TRANSPARENT_FADE_OUT:
                    ApplyColorPreset(particle, ParticleColor::ORANGE_TO_TRANSPARENT_FADE_OUT);
                    break;
                case ParticleColorPreset::YELLOW_TO_GREEN_FADE_OUT:
                    ApplyColorPreset(particle, ParticleColor::YELLOW_TO_GREEN_FADE_OUT);
                    break;
                case ParticleColorPreset::YELLOW_TO_TRANSPARENT_FADE_OUT:
                    ApplyColorPreset(particle, ParticleColor::YELLOW_TO_TRANSPARENT_FADE_OUT);
                    break;
                case ParticleColorPreset::BLUE_TO_CYAN_FADE_OUT:
                    ApplyColorPreset(particle, ParticleColor::BLUE_TO_CYAN_FADE_OUT);
                    break;
                case ParticleColorPreset::BLUE_TO_TRANSPARENT_FADE_OUT:
                    ApplyColorPreset(particle, ParticleColor::BLUE_TO_TRANSPARENT_FADE_OUT);
                    break;
                case ParticleColorPreset::FIRE_FADE_OUT:
                    ApplyColorPreset(particle, ParticleColor::FIRE_FADE_OUT);
                    break;
                case ParticleColorPreset::ICE_FADE_OUT:
                    ApplyColorPreset(particle, ParticleColor::ICE_FADE_OUT);
                    break;
                case ParticleColorPreset::TOXIC_FADE_OUT:
                    ApplyColorPreset(particle, ParticleColor::TOXIC_FADE_OUT);
                    break;
                case ParticleColorPreset::MAGIC_PURPLE_FADE_OUT:
                    ApplyColorPreset(particle, ParticleColor::MAGIC_PURPLE_FADE_OUT);
                    break;
                case ParticleColorPreset::NONE:
                default:
                    // do nothing
                    break;
                
            }

		}


    } RectParticleUtils;

}