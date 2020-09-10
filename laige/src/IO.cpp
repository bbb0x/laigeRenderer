// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#include "IO.h"
#include <iostream>
#include <fstream>
namespace IO
{
	std::vector<char> ReadFile(std::string&& path)
	{
		std::ifstream infile(path,
			std::ifstream::ate | std::ifstream::binary);

		auto length = infile.tellg();

		std::vector<char> buffer;
		buffer.resize(length);
		infile.seekg(0, std::ios::beg);
		infile.read(&buffer[0], length);

		return buffer;
	}
}