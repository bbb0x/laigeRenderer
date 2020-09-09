// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#include "Renderer.h"
#include "DxManager.h"
#include "Effects\InputLayouts\Ip_ColorPosition.h"
Renderer::Renderer()
{
	// Load all shaders on initialization
    m_lineColorBySegmentS.Load();
    m_triangleTestS.Load();  
}

void Renderer::DrawLine(const Line& line, const TwoDMatrix* matrix)
{
    DrawLines(&line, 1, matrix);
}

void  Renderer::DrawLines(const Line* lines, uint32_t length, const TwoDMatrix* matrix)
{
    m_lineColorBySegmentS.SetMatrices(DxManager::GetScreenMatrix(), matrix);

    uint32_t vertexLength = 0;
    for (uint32_t i = 0; i < length; ++i)
    {
        vertexLength += (lines[i].Length - 1) * 4;
    }


    auto s_vertexData = new lp_ColorPosition[vertexLength];

    // Verfify input test
   /* for (int i = 0; i < length; ++i)
    {
        std::stringstream ss;
        ss << std::to_string(points[i].x) << "_" << std::to_string(points[i].y) << "\n";
        OutputDebugString(ss.str().c_str());
    }*/

    uint32_t c = 0;
    for (uint32_t k = 0; k < length; ++k)
    {
        auto line = lines[k];
        auto points = line.Points;

#pragma omp parallel for
        for (uint32_t i = 0; i < line.Length - 1; ++i)
        {
            auto p = points[i];
            int j = c * 4;

            s_vertexData[j] = i == 0 ? lp_ColorPosition(points[i]) : lp_ColorPosition(points[i - 1]);
            s_vertexData[j + 1] = lp_ColorPosition(points[i]);
            s_vertexData[j + 2] = lp_ColorPosition(points[i + 1]);
            s_vertexData[j + 3] = i == line.Length - 2 ? lp_ColorPosition(points[i + 1]) : lp_ColorPosition(points[i + 2]);
            ++c;

        }
    }
    ////    Verify vertex test
    //   for (int i = 0; i < vertexLength; ++i)
    //   {
    //       std::stringstream ss;
    //       ss << std::to_string(s_vertexData[i].Position.x) << "_" << std::to_string(s_vertexData[i].Position.y) << "\n";
    //       OutputDebugString(ss.str().c_str());
    //   }


    m_lineColorBySegmentS.Update(s_vertexData, c, vertexLength, vertexLength * sizeof(lp_ColorPosition));

    m_lineColorBySegmentS.Draw();
}

void Renderer::DrawDebugTriangle()
{
    m_triangleTestS.Update(nullptr, 0,0, 0);
    m_triangleTestS.Draw();
}