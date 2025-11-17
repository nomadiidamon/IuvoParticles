#pragma once
#include <random>

namespace Particles
{
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

	constexpr float EPS = 1e-6f;
	static constexpr float PI_F = 3.14159265358979323846f;
	inline float DegToRad(float d) { return d * (PI_F / 180.0f); }
}