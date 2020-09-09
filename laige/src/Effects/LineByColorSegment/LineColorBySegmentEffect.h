// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#pragma once
#include "../baseClasses/Effect.h"
#include "../../TwoDMatrix.h"

class LineColorBySegmentEffect : public Effect
{
	

public:
	void Load() override;
	void Update(void* data, uint32_t length, uint32_t vertexLength, uint32_t byteWidth) override;
	void Draw() override;

	void SetMatrices(const TwoDMatrix& screenMatrix, const TwoDMatrix* valueMatrix = nullptr);

	LineColorBySegmentEffect()
	{

	}
};