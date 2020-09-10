// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#pragma once
#include <DirectXMath.h>
#include "../../Point.h"
#include "../../Color.h"
#include "../../LinePoint.h"
using namespace DirectX;

// 1 class per input. Identical to LinePoint so we prepare a constructor for it.
struct lp_ColorPosition
{
	Point Position;
	Color Color;
	float Thickness;

	lp_ColorPosition() noexcept :Color(), Thickness(0)
	{}

	lp_ColorPosition(LinePoint& lp) noexcept :Position(lp.Position), Color(lp.PointColor), Thickness(lp.Thickness)
	{}
};