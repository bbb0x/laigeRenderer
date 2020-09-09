// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#include "capi.h"
#include "DxManager.h"



std::unique_ptr<TwoDMatrix> inline getMatrix(Bounds* bounds)
{
	if (bounds)
	{
		return std::make_unique<TwoDMatrix>(*bounds);		
	}
	return nullptr;
}


void _Initialize()
{
    DxManager::Init();
}


void _SetScreenMatrix(Bounds* bounds)
{
	DxManager::SetScreenMatrix(TwoDMatrix(*bounds));
}

void _BeginDraw(void* renderTarget, bool isNewSurface, bool enableGraphicsDebugger)
{
	DxManager::BeginDraw(renderTarget, isNewSurface, enableGraphicsDebugger);
}

void _EndDraw()
{
	DxManager::EndDraw();
}


void _DrawLine1(Point* points, uint32_t length, float thickness, Color& color, Bounds* bounds)
{
	auto renderer = DxManager::GetRenderer();
	const unique_ptr<TwoDMatrix> m = getMatrix(bounds);


	LinePoint* lPoints = new LinePoint[length];
	for (uint32_t i = 0; i < length; ++i)
	{
		LinePoint lp = LinePoint();
		lp.PointColor = color;
		lp.Position = points[i];
		lp.Thickness = thickness;
		lPoints[i] = lp;
	}

	Line line = Line(length,lPoints);

	renderer->DrawLine(line, m.get());

}

void _DrawLine2(Line& line, Bounds* bounds)
{
	auto renderer = DxManager::GetRenderer();
	const unique_ptr<TwoDMatrix> m = getMatrix(bounds);

	renderer->DrawLine(line, m.get());
}

void _DrawLines(Line* lines, uint32_t length, Bounds* bounds)
{
	
	auto renderer = DxManager::GetRenderer();
	const unique_ptr<TwoDMatrix> m = getMatrix(bounds);

	renderer->DrawLines(lines, length, m.get());
}

void _DrawDebugTriangle()
{
	auto renderer = DxManager::GetRenderer();
	renderer->DrawDebugTriangle();
}

void _Release()
{
	DxManager::Release();
}


