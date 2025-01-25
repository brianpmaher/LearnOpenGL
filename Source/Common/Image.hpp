#pragma once

#include "Type.hpp"

class Image
{
public:
	Image(const char* path, bool flipY = false);

	~Image();

	inline Uchar* GetData()
	{
		return data;
	}

	inline int GetWidth()
	{
		return width;
	}

	inline int GetHeight()
	{
		return height;
	}

	inline int GetNumChannels()
	{
		return numChannels;
	}

private:
	Uchar* data;

	int width;
	int height;

	int numChannels;
};
