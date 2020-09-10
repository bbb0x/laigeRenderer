// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#pragma once
#include <cstdint>
#include "LinePoint.h"
struct Line
{
	uint32_t Length;
	LinePoint* Points;

	Line(uint32_t length, LinePoint* points) noexcept : Length(length), Points(points)
	{
	}
};