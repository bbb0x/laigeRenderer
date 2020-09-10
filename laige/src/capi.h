// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#pragma once
#include "Point.h"
#include "Color.h"
#include "Bounds.h"
#include "Line.h"
#include <cinttypes>
#include <cstdio>

extern "C"
{
	// API for C#

	/// <summary>
	/// Initializes the library.
	/// </summary>
	_declspec(dllexport) void _Initialize();

	/// <summary>
	/// Sets the screen matrix the input coordinates represent.
	/// </summary>
	/// <param name="bounds">The screen matrix.</param>
	_declspec(dllexport) void _SetScreenMatrix(Bounds* bounds);

	/// <summary>
	/// Starts draw operation.
	/// </summary>
	/// <param name="renderTarget">The rendertarget.</param>
	/// <param name="isNewSurface">If the rendertarget is a new surface or not.</param>
	/// <param name="enableGraphicsDebugger">If yes, enables the graphics debugger code which can be used within a graphics debugging session.</param>
	_declspec(dllexport) void _BeginDraw(void* renderTarget, bool isNewSurface, bool enableGraphicsDebugger);

	/// <summary>
	/// Draws a simple line.
	/// </summary>
	/// <param name="points">The points of the line.</param>
	/// <param name="length">Amount of points.</param>
	/// <param name="thickness">Line thickness.</param>
	/// <param name="color">Line color.</param>
	/// <param name="bounds">Optimal: Addtional matrix which will get converted to screen space later.</param>
	_declspec(dllexport) void _DrawLine1(Point* points, uint32_t length, float thickness, Color& color, Bounds* bounds);

	/// <summary>
	/// Draws a line.
	/// </summary>
	/// <param name="line">The line.</param>
	/// <param name="bounds">Optimal: Addtional matrix which will get converted to screen space later.</param>
	_declspec(dllexport) void _DrawLine2(Line& line, Bounds* bounds);

	/// <summary>
	/// Draws one or multiple lines.
	/// </summary>
	/// <param name="lines">The lines.</param>
	/// <param name="length">The amount of lines.</param>
	 /// <param name="bounds">Optimal: Addtional matrix which will get converted to screen space later.</param>
	_declspec(dllexport) void _DrawLines(Line* lines, uint32_t length, Bounds* bounds);

	/// <summary>
	/// Draws a debug triangle which can be used for testing to see if the library has been embedded correctly.
	/// If the triangle renders but no lines there might be an issue with the C# classes, input coordinates or matrix.
	/// </summary>
	_declspec(dllexport) void _DrawDebugTriangle();

	/// <summary>
	/// Ends a drawing operation and presents the rendered content.
	/// </summary>
	_declspec(dllexport) void _EndDraw();

	/// <summary>
	/// Releases all bound resources.
	/// </summary>
	_declspec(dllexport) void _Release();
}
