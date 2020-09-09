// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#pragma once
#include <exception>
#include <winerror.h>
#include <stdexcept>
namespace Utils
{

    class com_exception : public std::exception
    {
    public:
        com_exception(HRESULT hr) : result(hr) {}

        virtual const char* what() const override
        {
            static char s_str[64] = {};
            sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));
            return s_str;
        }

    private:
        HRESULT result;
    };

    // Helper utility converts D3D API failures into exceptions.
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw com_exception(hr);
        }
    }

    inline void Throw(HRESULT hr)
    {
        throw com_exception(hr);
    }
}