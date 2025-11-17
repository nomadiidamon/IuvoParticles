#pragma once
#include "imgui.h"

namespace Particles {
	struct ParticleColor {
		ImVec4 p_startColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		ImVec4 p_endColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
		ImVec4 p_currColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		float  p_lerpSpeed = 0.25f;

		bool p_lerpColor = false;
		bool p_fadeColor = true;
		bool p_useAlpha = true;
		bool p_randomizeColor = false;
	};
	/// TODO: add static const ParticleColor DEFAULT_COLOR{}


	struct ParticleLifetime {
		float p_lifetime = 0.0f;
		float p_maxLifetime = 0.0f;
		bool p_randomizeLifetime = false;
		bool p_loop = false;
	};
	/// TODO: add static const ParticleLifetime DEFAULT_LIFETIME{}


	struct ParticleTransform {
		ImVec2 p_position = ImVec2(0.0f, 0.0f);
		ImVec2 p_center = ImVec2(0.0f, 0.0f);

		ImVec2 p_size = ImVec2(50.0f, 50.0f);
		ImVec2 p_baseSize = ImVec2(50.0f, 50.0f); // store base size to compute non-destructive scaling each frame
		ImVec2 p_rotation = ImVec2(0.0f, 0.0f); // using x as angle in degrees for 2D rotation

		bool p_randomPosition = false;
		bool p_randomSize = false;
		bool p_randomRotation = false;
	};
	/// TODO: add static const ParticleTransform DEFAULT_TRANSFORM{}


	struct ParticleAnimation2D {
		ImVec2 p_velocity = ImVec2(0.0f, 0.0f);

		float p_movementSpeed = 0.35f;
		float p_rotationSpeed = 0.35f; // degrees per second
		float p_scaleSpeed = 0.35f;

		bool canMove = true;
		bool canRotate = true;
		bool canScale = false;
	};
	/// TODO: add static const ParticleAnimation2D DEFAULT_ANIMATION{}


	//struct ParticleUV {
	//	ImVec2 p_uv0 = ImVec2(0.0f, 0.0f);
	//	ImVec2 p_uv1 = ImVec2(1.0f, 1.0f);
	//};
	/// TODO: add static const ParticleUV DEFAULT_UV{}

	//struct ParticleTexture {
	//	/// TODO: file path for texture / Walnut image
	//	ImVec2 p_textureSize = ImVec2(0.0f, 0.0f);
	//};
	/// TODO: add static const ParticleTexture DEFAULT_TEXTURE{}
}