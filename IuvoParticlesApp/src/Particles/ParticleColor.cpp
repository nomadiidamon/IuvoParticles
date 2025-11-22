#pragma once 
#include "ParticleColor.h"
#include "ParticleUtils.h"

namespace Particles
{
	const ParticleColor ParticleColor::DEFAULT_COLOR = ParticleColor{};
	ParticleColor ParticleColor::CreateRandomColor() {
		auto& startColor = RandColor();

		ParticleColor out = ParticleColor{
			RandColor(), // startColor
			RandColor(), // endColor
			startColor, // currColor
			RandFloat(0.0f, 1.0f), // lerpSpeed
			true,  // lerpColor
			true,  // fadeColor
			true,  // useAlpha
			true  // randomizeColor			
		};
		return out;
	}
	ParticleColor ParticleColor::CreateRandomColorInRange(const ImVec4& minColor, const ImVec4& maxColor) {
		auto& cfg = GetParticleConfig();
		auto& startColor = RandColorInRange(minColor, maxColor);

		ParticleColor out = ParticleColor{
			startColor, // startColor
			RandColorInRange(minColor, maxColor), // endColor
			startColor, // currColor
			RandFloat(0.0f, 1.0f), // lerpSpeed
			true,  // lerpColor
			true,  // fadeColor
			true,  // useAlpha
			true  // randomizeColor			
		};
		return out;
	}
	const ParticleColor ParticleColor::RED_TO_YELLOW_FADE_OUT = ParticleColor{
		ColorRed(),
		SetAlpha(ColorYellow()),
		ColorRed(),
		true,                           // lerpColor
		true,                           // fadeColor
		true,                           // useAlpha
		false                           // randomizeColor
	};
	const ParticleColor ParticleColor::RED_TO_BLUE_FADE_OUT = ParticleColor{
		ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // startColor (Red)
		ImVec4(0.0f, 0.0f, 1.0f, 0.0f), // endColor (Blue, fully transparent)
		ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // currColor (default to start color)
		true,                           // lerpColor
		true,                           // fadeColor
		true,                           // useAlpha
		false                           // randomizeColor
	};
	const ParticleColor ParticleColor::RED_TO_TRANSPARENT_FADE_OUT = ParticleColor{
		ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // startColor (Red)
		ImVec4(1.0f, 0.0f, 0.0f, 0.0f), // endColor (Red, fully transparent)
		ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // currColor  (default to start color)
		true,                          // lerpColor
		true,                           // fadeColor
		true,                           // useAlpha
		false                           // randomizeColor
	};
	const ParticleColor ParticleColor::ORANGE_TO_YELLOW_FADE_OUT = ParticleColor{
		ImVec4(1.0f, 0.5f, 0.0f, 1.0f), // startColor (Orange)
		ImVec4(1.0f, 1.0f, 0.0f, 0.0f), // endColor (Yellow, fully transparent)
		ImVec4(1.0f, 0.5f, 0.0f, 1.0f), // currColor (default to start color)
		true,                           // lerpColor
		true,                           // fadeColor
		true,                           // useAlpha
		false                           // randomizeColor
	};
	const ParticleColor ParticleColor::ORANGE_TO_TRANSPARENT_FADE_OUT = ParticleColor{
		ImVec4(1.0f, 0.5f, 0.0f, 1.0f), // startColor (Orange)
		ImVec4(1.0f, 0.5f, 0.0f, 0.0f), // endColor (Orange, fully transparent)
		ImVec4(1.0f, 0.5f, 0.0f, 1.0f), // currColor (default to start color)
		true,                           // lerpColor
		true,                           // fadeColor
		true,                           // useAlpha
		false                           // randomizeColor
	};
	const ParticleColor ParticleColor::YELLOW_TO_GREEN_FADE_OUT = ParticleColor{
		ImVec4(1.0f, 1.0f, 0.0f, 1.0f), // startColor (Yellow)
		ImVec4(0.0f, 1.0f, 0.0f, 0.0f), // endColor (Green, fully transparent)
		ImVec4(1.0f, 1.0f, 0.0f, 1.0f), // currColor  (default to start color)
		true,                           // lerpColor
		true,                           // fadeColor
		true,                           // useAlpha
		false                           // randomizeColor
	};
	const ParticleColor ParticleColor::YELLOW_TO_TRANSPARENT_FADE_OUT = ParticleColor{
		ImVec4(1.0f, 1.0f, 0.0f, 1.0f), // startColor (Yellow)
		ImVec4(1.0f, 1.0f, 0.0f, 0.0f), // endColor (Yellow, fully transparent)
		ImVec4(1.0f, 1.0f, 0.0f, 1.0f), // currColor (default to start color)
		true,                           // lerpColor
		true,                           // fadeColor
		true,                           // useAlpha
		false                           // randomizeColor
	};
	const ParticleColor ParticleColor::BLUE_TO_CYAN_FADE_OUT = ParticleColor{
		ImVec4(0.0f, 0.0f, 1.0f, 1.0f), // startColor (Blue)
		ImVec4(0.0f, 1.0f, 1.0f, 0.0f), // endColor (Cyan, fully transparent)
		ImVec4(0.0f, 0.0f, 1.0f, 1.0f), // currColor (default to start color)
		true,                           // lerpColor
		true,                           // fadeColor
		true,                           // useAlpha
		false                           // randomizeColor
	};
	const ParticleColor ParticleColor::BLUE_TO_TRANSPARENT_FADE_OUT = ParticleColor{
		ImVec4(0.0f, 0.0f, 1.0f, 1.0f), // startColor (Blue)
		ImVec4(0.0f, 0.0f, 1.0f, 0.0f), // endColor (Blue, fully transparent)
		ImVec4(0.0f, 0.0f, 1.0f, 1.0f), // currColor (default to start color)
		true,                          // lerpColor
		true,                           // fadeColor
		true,                           // useAlpha
		false                           // randomizeColor
	};

	const ParticleColor ParticleColor::FIRE_FADE_OUT{
	ImVec4(1.0f, 0.4f, 0.0f, 1.0f),  // fiery orange
	ImVec4(0.8f, 0.0f, 0.0f, 0.0f),  // dark red transparent
	ImVec4(1.0f, 0.4f, 0.0f, 1.0f),
	0.35f, true, true, true, false
	};
	const ParticleColor ParticleColor::ICE_FADE_OUT{
	ImVec4(0.85f, 0.95f, 1.0f, 1.0f),
	ImVec4(0.0f, 0.4f, 1.0f, 0.0f),
	ImVec4(0.85f, 0.95f, 1.0f, 1.0f),
	0.25f, true, true, true, false
	};
	const ParticleColor ParticleColor::TOXIC_FADE_OUT{
	ImVec4(0.0f, 1.0f, 0.3f, 1.0f),
	ImVec4(0.7f, 1.0f, 0.0f, 0.0f),
	ImVec4(0.0f, 1.0f, 0.3f, 1.0f),
	0.3f, true, true, true, false
	};
	const ParticleColor ParticleColor::MAGIC_PURPLE_FADE_OUT{
	ImVec4(0.6f, 0.0f, 1.0f, 1.0f),
	ImVec4(1.0f, 0.3f, 1.0f, 0.0f),
	ImVec4(0.6f, 0.0f, 1.0f, 1.0f),
	0.4f, true, true, true, false
	};

	bool ParticleColor::IsEqual(const ParticleColor& a, const ParticleColor& b) {
		return ColorEquals(a.startColor, b.startColor) &&
			ColorEquals(a.endColor, b.endColor) &&
			ColorEquals(a.currColor, b.currColor) &&
			FloatEquals(a.lerpSpeed, b.lerpSpeed) &&
			(a.lerpColor == b.lerpColor) &&
			(a.fadeColor == b.fadeColor) &&
			(a.useAlpha == b.useAlpha) &&
			(a.randomizeColor == b.randomizeColor);
	}

	nlohmann::json ParticleColorSerializer::Serialize(const ParticleColor& color) {
		nlohmann::json j;
		j["startColor"] = { color.startColor.x, color.startColor.y, color.startColor.z, color.startColor.w };
		j["endColor"] = { color.endColor.x, color.endColor.y, color.endColor.z, color.endColor.w };
		j["currColor"] = { color.currColor.x, color.currColor.y, color.currColor.z, color.currColor.w };
		j["lerpSpeed"] = color.lerpSpeed;
		j["lerpColor"] = color.lerpColor;
		j["fadeColor"] = color.fadeColor;
		j["useAlpha"] = color.useAlpha;
		j["randomizeColor"] = color.randomizeColor;
		return j;
	}
	ParticleColor ParticleColorSerializer::Deserialize(const nlohmann::json& j) {
		ParticleColor color;
		color.startColor = ImVec4(j["startColor"][0], j["startColor"][1], j["startColor"][2], j["startColor"][3]);
		color.endColor = ImVec4(j["endColor"][0], j["endColor"][1], j["endColor"][2], j["endColor"][3]);
		color.currColor = ImVec4(j["currColor"][0], j["currColor"][1], j["currColor"][2], j["currColor"][3]);
		color.lerpSpeed = j["lerpSpeed"];
		color.lerpColor = j["lerpColor"];
		color.fadeColor = j["fadeColor"];
		color.useAlpha = j["useAlpha"];
		color.randomizeColor = j["randomizeColor"];
		return color;
	}
}