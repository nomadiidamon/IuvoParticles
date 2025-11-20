#pragma once
#include <vector>

namespace Particles {

	constexpr float EPS = 1e-6f;
	static bool FloatEquals(float a, float b) {
		return fabsf(a - b) <= EPS;
	}

	static bool ColorEquals(const ImVec4& a, const ImVec4& b) {
		return FloatEquals(a.x, b.x) &&
			FloatEquals(a.y, b.y) &&
			FloatEquals(a.z, b.z) &&
			FloatEquals(a.w, b.w);
	}

	static constexpr float PI_F = 3.14159265358979323846f;
	inline float DegToRad(float d) { return d * (PI_F / 180.0f); }

	static ImVec4 StripAlpha(const ImVec4& color) {
		return ImVec4(color.x, color.y, color.z, 1.0f);
	}
	static ImVec4 SetAlpha(const ImVec4& color, float alpha = 0.0f) {
		return ImVec4(color.x, color.y, color.z, alpha);
	}

	// basic colors
	static ImVec4 ColorWhite() { return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); }
	static ImVec4 ColorBlack() { return ImVec4(0.0f, 0.0f, 0.0f, 1.0f); }
	static ImVec4 ColorGray() { return ImVec4(0.5f, 0.5f, 0.5f, 1.0f); }

	// rainbow colors
	static ImVec4 ColorRed() { return ImVec4(1.0f, 0.0f, 0.0f, 1.0f); }
	static ImVec4 ColorOrange() { return ImVec4(1.0f, 0.5f, 0.0f, 1.0f); }
	static ImVec4 ColorYellow() { return ImVec4(1.0f, 1.0f, 0.0f, 1.0f); }
	static ImVec4 ColorGreen() { return ImVec4(0.0f, 1.0f, 0.0f, 1.0f); }
	static ImVec4 ColorBlue() { return ImVec4(0.0f, 0.0f, 1.0f, 1.0f); }
	static ImVec4 ColorIndigo() { return ImVec4(0.29f, 0.0f, 0.51f, 1.0f); }
	static ImVec4 ColorPurple() { return ImVec4(0.5f, 0.0f, 0.5f, 1.0f); }

	// additional colors
	static ImVec4 ColorCyan() { return ImVec4(0.0f, 1.0f, 1.0f, 1.0f); }
	static ImVec4 ColorMagenta() { return ImVec4(1.0f, 0.0f, 1.0f, 1.0f); }
	static ImVec4 ColorPink() { return ImVec4(1.0f, 0.75f, 0.8f, 1.0f); }
	static ImVec4 ColorBrown() { return ImVec4(0.6f, 0.4f, 0.2f, 1.0f); }
	static ImVec4 ColorTransparent() { return ImVec4(0.0f, 0.0f, 0.0f, 0.0f); }

}