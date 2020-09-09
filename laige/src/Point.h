// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

struct Point
{
	float x;
	float y;

    Point(float x, float y) noexcept :x(x), y(y)
	{}

    Point() noexcept :x(0),y(0)
    {}


    void operator+=(Point const& other);
   

    void operator-=(Point const& other);
   

    Point operator+(Point const& a);
 

    Point operator-(Point const& a);
  
	
};
#pragma once
