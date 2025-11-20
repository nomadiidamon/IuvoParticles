#pragma once
#include <vector>

namespace Particles {

	constexpr float EPS = 1e-6f;
	static bool FloatEquals(float a, float b) {
		return fabsf(a - b) <= EPS;
	}

	static constexpr float PI_F = 3.14159265358979323846f;
	inline float DegToRad(float d) { return d * (PI_F / 180.0f); }
}