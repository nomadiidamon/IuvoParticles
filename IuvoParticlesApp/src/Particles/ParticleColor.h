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
		ImVec4 startColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		ImVec4 endColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
		ImVec4 currColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		float  lerpSpeed = 0.25f;

		bool lerpColor = true;
		bool fadeColor = true;
		bool useAlpha = true;
		bool randomizeColor = false;

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
	struct ParticleColorRange {
		bool enforceConstraints = false;

		ImVec4 startColorMin = ImVec4(0.1f, 0.1f, 0.1f, 0.1f);
		ImVec4 startColorMax = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImVec4 defaultStartColor = ParticleColor::ICE_FADE_OUT.startColor;

		ImVec4 endColorMin = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		ImVec4 endColorMax = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		ImVec4 defaultEndColor = ParticleColor::TOXIC_FADE_OUT.startColor;
		ImVec4 defaultCurrColor = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);


		float colorFadeSpeedMin = 0.01f;
		float colorFadeSpeedMax = 2.0f;
		float defaultColorFadeSpeed = 0.25f;
		static const ParticleColorRange DEFAULT_COLOR_RANGE;
	};
}