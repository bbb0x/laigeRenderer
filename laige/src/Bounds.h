// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#pragma once
struct Bounds
{
	float x;
	float y;
	float width;
	float height;

	Bounds(float x, float y, float w, float h) : x(x), y(y), width(w), height(h)
	{
	}
};