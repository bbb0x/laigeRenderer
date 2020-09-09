// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#pragma once
#include "../baseClasses/Effect.h"


class TriangleTestEffect : public Effect
{




public:
	void Load() override;
	void Update(void* data, uint32_t length, uint32_t vertexLength, uint32_t byteWidth) override;
	void Draw() override;

	TriangleTestEffect()
	{

	}
};