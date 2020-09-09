// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#pragma once
namespace BoxMath
{
    int inline RoundUp(int numToRound, int multiple)
    {
        if (multiple == 0)
            return numToRound;

        int remainder = numToRound % multiple;
        if (remainder == 0)
            return numToRound;

        return numToRound + multiple - remainder;
    }
};