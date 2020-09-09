// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#include "Point.h"

void Point::operator+=(Point const& other)
{
    x += other.x;
    y += other.y;
}

void Point::operator-=(Point const& other)
{
    x -= other.x;
    y -= other.y;
}

Point Point::operator+(Point const& a)
{
    Point ret(a);

    ret.x = x + a.x;
    ret.y = y + a.y;
    return ret;
}

Point Point::operator-(Point const& a)
{
    Point ret(a);

    ret.x = x - a.x;
    ret.y = y - a.y;
    return ret;
}