#pragma once

#include <glm/vec2.hpp>

struct Input
{
	enum class Key
	{
		Escape,
		W,
		A,
		S,
		D,
	};

	enum class Status
	{
		None,
		Press,
		Release,
	};

	glm::vec2 cursorPosition;
	glm::vec2 cursorDelta;

	glm::vec2 scrollDelta;

	Status keyEscape;
	Status keyW;
	Status keyA;
	Status keyS;
	Status keyD;

	inline void Reset()
	{
		cursorDelta = {0.0f, 0.0f};

		scrollDelta = {0.0f, 0.0f};

		keyEscape = Status::None;
		keyW = Status::None;
		keyA = Status::None;
		keyS = Status::None;
		keyD = Status::None;
	}
};
