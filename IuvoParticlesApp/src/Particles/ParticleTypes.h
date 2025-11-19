#pragma once
#include "imgui.h"

namespace Particles {

	enum class ParticleColorPreset {
		NONE,
		RED_TO_YELLOW_FADE_OUT,
		RED_TO_BLUE_FADE_OUT,
		RED_TO_TRANSPARENT_FADE_OUT,
		ORANGE_TO_YELLOW_FADE_OUT,
		ORANGE_TO_TRANSPARENT_FADE_OUT,
		YELLOW_TO_GREEN_FADE_OUT,
		YELLOW_TO_TRANSPARENT_FADE_OUT,
		BLUE_TO_CYAN_FADE_OUT,
		BLUE_TO_TRANSPARENT_FADE_OUT,
		FIRE_FADE_OUT,
		ICE_FADE_OUT,
		TOXIC_FADE_OUT,
		MAGIC_PURPLE_FADE_OUT
	};
	struct ParticleColor {
		ImVec4 p_startColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		ImVec4 p_endColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
		ImVec4 p_currColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		float  p_lerpSpeed = 0.25f;

		bool p_lerpColor = true;
		bool p_fadeColor = true;
		bool p_useAlpha = true;
		bool p_randomizeColor = false;

		static const ParticleColor DEFAULT_COLOR;

		static const ParticleColor RED_TO_YELLOW_FADE_OUT;
		static const ParticleColor RED_TO_BLUE_FADE_OUT;
		static const ParticleColor RED_TO_TRANSPARENT_FADE_OUT;
		static const ParticleColor ORANGE_TO_YELLOW_FADE_OUT;
		static const ParticleColor ORANGE_TO_TRANSPARENT_FADE_OUT;
		static const ParticleColor YELLOW_TO_GREEN_FADE_OUT;
		static const ParticleColor YELLOW_TO_TRANSPARENT_FADE_OUT;
		static const ParticleColor BLUE_TO_CYAN_FADE_OUT;
		static const ParticleColor BLUE_TO_TRANSPARENT_FADE_OUT;
		static const ParticleColor FIRE_FADE_OUT;
		static const ParticleColor ICE_FADE_OUT;
		static const ParticleColor TOXIC_FADE_OUT;
		static const ParticleColor MAGIC_PURPLE_FADE_OUT;

		static ParticleColor CreateRandomColor();
		static ParticleColor CreateRandomColorInRange(const ImVec4& minColor, const ImVec4& maxColor);
	};

	enum class ParticleLifetimePreset {
		NONE,
		SHORT_BURST,
		MEDIUM_BURST,
		LONG_BURST,
		LONG_FLOATING
	};
	struct ParticleLifetime {
		float p_lifetime = 0.0f;
		float p_maxLifetime = 5.0f;
		bool p_randomizeLifetime = false;
		bool p_loop = false;

		static const ParticleLifetime DEFAULT_LIFETIME;
		static ParticleLifetime CreateRandomLifetime();
		static const ParticleLifetime SHORT_BURST;
		static const ParticleLifetime MEDIUM_BURST;
		static const ParticleLifetime LONG_BURST;
		static const ParticleLifetime LONG_FLOATING;

	};

	enum class ParticleTransformPreset {
		NONE,
		SMALL_FLOATING,
		EXPLOSION_CHUNK
	};
	struct ParticleTransform {
		ImVec2 p_position = ImVec2(0.0f, 0.0f);
		ImVec2 p_center = ImVec2(0.0f, 0.0f);

		ImVec2 p_size = ImVec2(10.0f, 10.0f);
		ImVec2 p_baseSize = ImVec2(10.0f, 10.0f); // store base size to compute non-destructive scaling each frame
		ImVec2 p_rotation = ImVec2(67.0f, 0.0f); // using x as angle in degrees for 2D rotation

		/// TODO: reevaluate these randomization flags
		bool p_randomPosition = false;
		bool p_randomSize = false;
		bool p_randomRotation = false;

		static const ParticleTransform DEFAULT_TRANSFORM;
		static ParticleTransform CreateRandomTransform();
		static const ParticleTransform SMALL_FLOATING;
		static const ParticleTransform EXPLOSION_CHUNK;

	};

	enum class ParticleAnimation2DPreset {
		NONE,
		SMALL_RISING,
		EXPLOSION,
		DRIFTING,
		SPINNING,
		LIGHTWIEGHT_DRIFT
	};
	struct ParticleAnimation2D {
		ImVec2 p_velocity = ImVec2(-3.0f, -1.5f);

		float p_movementSpeed = 85.5f;
		float p_rotationSpeed = 149.67f; // degrees per second
		float p_scaleSpeed = 0.35f;

		bool canMove = true;
		bool canRotate = true;
		bool canScale = false;

		static const ParticleAnimation2D DEFAULT_ANIMATION;
		static ParticleAnimation2D CreateRandomAnimation2D();
		static const ParticleAnimation2D SMALL_RISING;
		static const ParticleAnimation2D EXPLOSION;
		static const ParticleAnimation2D DRIFTING;
		static const ParticleAnimation2D SPINNING;
		static const ParticleAnimation2D LIGHTWIEGHT_DRIFT;

	};


	//struct ParticleUV {
	//	ImVec2 p_uv0 = ImVec2(0.0f, 0.0f);
	//	ImVec2 p_uv1 = ImVec2(1.0f, 1.0f);
	// 
	// static const ParticleUV DEFAULT_UV;
	//};

	//struct ParticleTexture {
	//	/// TODO: file path for texture / Walnut image
	//	ImVec2 p_textureSize = ImVec2(0.0f, 0.0f);
	// 
	// static const ParticleTexture DEFAULT_TEXTURE;
	//};
}