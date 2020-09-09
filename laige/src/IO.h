// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#pragma once
#include <vector>
#include <stdexcept>
namespace IO
{
	std::vector<char> ReadFile(std::string&& path);
}