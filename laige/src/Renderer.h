// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <cstdint>
#include <vector>
#include "Color.h"
#include "Point.h"
#include "Effects/LineByColorSegment/LineColorBySegmentEffect.h"
#include "Effects/TestTriangle/TriangleTestEffect.h"
#include "TwoDMatrix.h"
#include "Line.h"
using namespace std;

class Renderer
{
private:
	LineColorBySegmentEffect m_lineColorBySegmentS;
	TriangleTestEffect		 m_triangleTestS;

public:
	Renderer();
	void DrawLine(const Line& line, const TwoDMatrix* matrix);
	void DrawLines(const Line* lines, uint32_t length, const TwoDMatrix* matrix);
	void DrawDebugTriangle();
};