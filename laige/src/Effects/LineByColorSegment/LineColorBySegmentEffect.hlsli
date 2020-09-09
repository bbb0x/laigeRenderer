#include "../Functions/Functions.hlsli"

struct VS_IN
{
    float2 Position : POSITION;
    float4 Color : COLOR;
    float Thickness : THICKNESS;
};

struct GEO_IN
{
    float4 Position : POSITION0;
    float4 Color : COLOR0;
    float Thickness : THICKNESS;
};

struct GEO_OUT
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR0;
    float Thickness : THICKNESS;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 Color : COLOR0;
      
};



GEO_IN VS(VS_IN vsIn)
{
    GEO_IN result;

    result.Position = valueMatrix.IsSet() ? matrix_transform(float4(vsIn.Position.x, vsIn.Position.y, 0.5, 1), valueMatrix, screenMatrix) : float4(vsIn.Position.x, vsIn.Position.y, 0.5,1);
    result.Color = vsIn.Color;
    result.Thickness = vsIn.Thickness;
    return result;
}



[maxvertexcount(6)]
void GS(lineadj GEO_IN points[4], inout TriangleStream<GEO_OUT> triStream)
{
	geo_lineWithThicknessResult result = geo_lineWithThickness(points[0].Position.xy, points[1].Position.xy, points[2].Position.xy, points[3].Position.xy, points[0].Thickness);
	for (int i = 0; i < result.length;++i)
    {
        //if(i == result.restartStripIndex)
        //    triStream.RestartStrip();
        
        GEO_OUT output;
        output.Position = result.trianglePoints[i]; 
        output.Position = matrix_transform(output.Position, screenMatrix, getNativeMatrix());
        
        // Index 1 = the starting point defines these.
        // Index 2 = the end point defines these
        // Do not use 0 and 3, as they are part of another line point and just used for the math here.
        int index = 2;
        output.Color = points[index].Color;
        output.Thickness = points[index].Thickness;
        triStream.Append(output);
  

    }
   // triStream.RestartStrip();

}


float4 PS(PS_IN input) : SV_Target
{
    float4 c = input.Color;
    return c;
}

