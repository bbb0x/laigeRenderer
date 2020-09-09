#include "GeoFunctions.hlsli"

struct TransformMatrix
{

	float4x4 Proj;

};

struct TwoDMatrix
{
    float Left;
    float Top;
    float Right;
    float Bottom;
    
    bool IsSet()
    {
        return Left != 0 || Top != 0 || Right != 0 || Bottom != 0;
    }

    
};

cbuffer Matrices: register(b0)
{
    TwoDMatrix valueMatrix;
    TwoDMatrix screenMatrix;
};

TwoDMatrix getNativeMatrix()
{
    TwoDMatrix n;
    n.Left = -1;
    n.Top = 1;
    n.Right = 1;
    n.Bottom = -1;
    
    return n;
}





float4 matrix_transform(float4 position, TwoDMatrix sourceMatrix, TwoDMatrix destinationMatrix)
{
    float sourceRangeX = sourceMatrix.Right - sourceMatrix.Left;
    float sourcePercX = (position.x - sourceMatrix.Left) / sourceRangeX;
    float targetRangeX = destinationMatrix.Right - destinationMatrix.Left;
    float targetX = (sourcePercX * targetRangeX) + destinationMatrix.Left;
    
    float sourceRangeY = sourceMatrix.Bottom - sourceMatrix.Top;
    float sourcePercY =   (position.y - sourceMatrix.Top) / sourceRangeY;
    float targetRangeY = destinationMatrix.Bottom - destinationMatrix.Top;
    float targetY = (sourcePercY * targetRangeY) + destinationMatrix.Top;
    
    return float4(targetX, targetY, position.z, position.w);
    
  

}

float4 matrix_transform(float4 position, TransformMatrix tMatrix)
{

	position = mul(position, tMatrix.Proj);
	
	return position;
}