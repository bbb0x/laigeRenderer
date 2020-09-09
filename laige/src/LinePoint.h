// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#pragma once


#include <cinttypes>
#include <cstdio>
#include "Color.h"
#include "Point.h"


struct LinePoint
{
	Point Position;
	Color PointColor;
	float Thickness;

	LinePoint() noexcept : Position(Point()), PointColor(Color()), Thickness(0)
	{}
	LinePoint(Point& p, Color& color, float thickness) noexcept: Position(p), PointColor(color), Thickness(thickness)
	{}
};