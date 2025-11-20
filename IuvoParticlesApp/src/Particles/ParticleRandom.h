#pragma once
#include <random>
#include "imgui.h"

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
	static ImVec2 RandImVec2Int(const ImVec2& min, const ImVec2& max) {
		int x = RandInt(static_cast<int>(min.x), static_cast<int>(max.x));
		int y = RandInt(static_cast<int>(min.y), static_cast<int>(max.y));
		return ImVec2(static_cast<float>(x), static_cast<float>(y));
	}
	static ImVec4 RandImVec4Int(const ImVec4& min, const ImVec4& max) {
		int x = RandInt(static_cast<int>(min.x), static_cast<int>(max.x));
		int y = RandInt(static_cast<int>(min.y), static_cast<int>(max.y));
		int z = RandInt(static_cast<int>(min.z), static_cast<int>(max.z));
		int w = RandInt(static_cast<int>(min.w), static_cast<int>(max.w));
		return ImVec4(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(w));
	}
	static ImVec2 RandImVec2Float(const ImVec2& min, const ImVec2& max) {
		float x = RandFloat(min.x, max.x);
		float y = RandFloat(min.y, max.y);
		return ImVec2(x, y);
	}
	static ImVec4 RandImVec4Float(const ImVec4& min, const ImVec4& max) {
		float x = RandFloat(min.x, max.x);
		float y = RandFloat(min.y, max.y);
		float z = RandFloat(min.z, max.z);
		float w = RandFloat(min.w, max.w);
		return ImVec4(x, y, z, w);
	}

	static ImVec4 RandColor() {
		float r = RandFloat(0.0f, 1.0f);
		float g = RandFloat(0.0f, 1.0f);
		float b = RandFloat(0.0f, 1.0f);
		float a = RandFloat(0.0f, 1.0f);
		return ImVec4(r, g, b, a);
	}

	static ImVec4 RandColorOpaque() {
		float r = RandFloat(0.0f, 1.0f);
		float g = RandFloat(0.0f, 1.0f);
		float b = RandFloat(0.0f, 1.0f);
		return ImVec4(r, g, b, 1.0f);
	}	

	static ImVec4 RandomColorWithAlpha(float alpha) {
		float r = RandFloat(0.0f, 1.0f);
		float g = RandFloat(0.0f, 1.0f);
		float b = RandFloat(0.0f, 1.0f);
		return ImVec4(r, g, b, alpha);
	}

	static ImVec4 RandColorInRange(const ImVec4& min, const ImVec4& max) {
		return RandImVec4Float(min, max);
	}

}