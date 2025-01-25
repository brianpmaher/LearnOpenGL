#pragma once

#include "Type.hpp"

class Image
{
public:
	Image(const char* path, bool flipY = false);

	~Image();

	inline Uchar* GetData() const
	{
		return data;
	}

	inline int GetWidth() const
	{
		return width;
	}

	inline int GetHeight() const
	{
		return height;
	}

	inline int GetNumChannels() const
	{
		return numChannels;
	}

private:
	Uchar* data;

	int width;
	int height;

	int numChannels;
};
