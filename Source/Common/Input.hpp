#pragma once

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

	Status KeyEscape;
	Status KeyW;
	Status KeyA;
	Status KeyS;
	Status KeyD;
};
