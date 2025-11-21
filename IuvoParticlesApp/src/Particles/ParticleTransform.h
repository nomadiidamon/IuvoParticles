#pragma once 
#include "imgui.h"

namespace Particles {
	enum class ParticleTransformPreset {
		NONE,
		SMALL_FLOATING,
		EXPLOSION_CHUNK
	};
	struct ParticleTransform {
		ImVec2 position = ImVec2(0.0f, 0.0f);
		ImVec2 center = ImVec2(0.0f, 0.0f);

		ImVec2 size = ImVec2(10.0f, 10.0f);
		ImVec2 baseSize = ImVec2(10.0f, 10.0f); // store base size to compute non-destructive scaling each frame
		ImVec2 rotation = ImVec2(67.0f, 0.0f); // using x as angle in degrees for 2D rotation

		/// TODO: reevaluate these randomization flags
		bool randomPosition = false;
		bool randomSize = false;
		bool randomRotation = false;

		static const ParticleTransform DEFAULT_TRANSFORM;
		static ParticleTransform CreateRandomTransform();
		static const ParticleTransform SMALL_FLOATING;
		static const ParticleTransform EXPLOSION_CHUNK;

	};
	struct ParticleTransformRange {
		bool enforceConstraints = false;

		ImVec2 minPosition = ImVec2(0.0f, 0.0f);
		ImVec2 maxPosition = ImVec2(100.0f, 100.0f);
		ImVec2 randomPositionMin = ImVec2(0.0f, 0.0f);
		ImVec2 randomPositionMax = ImVec2(100.0f, 100.0f);
		ImVec2 defaultPosition = ImVec2(150.0f, 150.0f);

		ImVec2 minSize = ImVec2(5.0f, 5.0f);
		ImVec2 maxSize = ImVec2(20.0f, 20.0f);
		ImVec2 randomSizeMin = ImVec2(5.0f, 5.0f);
		ImVec2 randomSizeMax = ImVec2(20.0f, 20.0f);
		ImVec2 defaultSize = ImVec2(10.0f, 10.0f);

		ImVec2 minRotation = ImVec2(0.0f, 0.0f);
		ImVec2 maxRotation = ImVec2(0.0f, 0.0f);
		ImVec2 randomRotationMin = ImVec2(0.0f, 0.0f);
		ImVec2 randomRotationMax = ImVec2(360.0f, 0.0f);
		ImVec2 defaultRotation = ImVec2(0.0f, 0.0f);

		static const ParticleTransformRange DEFAULT_TRANSFORM_RANGE;
	};
}