// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#pragma once
#include "Bounds.h"
struct TwoDMatrix
{
	float Left;
	float Top;
	float Right;
	float Bottom;

	TwoDMatrix() noexcept : Left(0), Top(0), Right(0), Bottom(0)
	{}

	TwoDMatrix(Bounds& bounds) noexcept : Left(bounds.x), Top(bounds.y), Right(bounds.x + bounds.width), Bottom(bounds.y + bounds.height)
	{}

	void SetInvalid() noexcept
	{
		Left = 0;
		Top = 0;
		Right = 0;
		Bottom = 0;
	}
};

struct Matrices
{
	TwoDMatrix valueMatrix;
	TwoDMatrix screenMatrix;
};