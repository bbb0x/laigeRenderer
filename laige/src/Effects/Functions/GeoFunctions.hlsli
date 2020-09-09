

struct geo_lineWithThicknessResult
{
    float4 trianglePoints[7];
    float length;
    int restartStripIndex;
};


geo_lineWithThicknessResult geo_lineWithThickness(float2 p0, float2 p1, float2 p2, float2 p3, float thickness, float miterLimit = 0.75)
{


    float2 v0 = normalize(p1 - p0);
    float2 v1 = normalize(p2 - p1);
    float2 v2 = normalize(p3 - p2);

    // determine the normal of each of the 3 segments (previous, current, next)
    float2 n0 = { -v0.y, v0.x };
    float2 n1 = { -v1.y, v1.x };
    float2 n2 = { -v2.y, v2.x };

    // determine miter lines by averaging the normals of the 2 segments
    float2 miter_a = normalize(n0 + n1); // miter at start of current segment
    float2 miter_b = normalize(n1 + n2); // miter at end of current segment

    // determine the length of the miter by projecting it onto normal and then inverse it

    float length_a = thickness / dot(miter_a, n1);
    float length_b = thickness / dot(miter_b, n1);


    float2 temp;
	float4 temp4;
	int i = 0;
    float4 trianglePoints[7];
    int restartStripIndex = -1;
    
    //// prevent excessively long miters at sharp corners
    if (dot(v0, v1) < -miterLimit)
    {
        miter_a = n1;
        length_a = thickness;
        restartStripIndex = 4;
        
        // close the gap. Easter Egg comment: MIND THE GAP
        if (dot(v0, n1) > 0)
        {
            temp = (p1 + thickness * n0);
            temp4 = float4(temp, 0, 1.0);
            trianglePoints[6] = temp4;

            temp = (p1 + thickness * n1);
            temp4 = float4(temp, 0, 1.0);
            trianglePoints[5] = temp4;

            temp4 = float4(p1, 0, 1.0);
            trianglePoints[4] = temp4;

            //triStream.RestartStrip();

        }
        else
        {
            temp = (p1 - thickness * n1);
            temp4 = float4(temp, 0, 1.0);
            trianglePoints[6] = temp4;

            temp = (p1 - thickness * n0);
            temp4 = float4(temp, 0, 1.0);
            trianglePoints[5] = temp4;

            temp4 = float4(p1, 0, 1.0);
            trianglePoints[4] = temp4;

            //triStream.RestartStrip();
        }
    }

    if (dot(v1, v2) < -miterLimit)
    {
        miter_b = n1;
        length_b = thickness;
    }
    
    // First line point, the begin is just its thickness away.
    if (p0.x == p1.x && p0.y == p1.y)
    {
    
        length_a = thickness;
        miter_a = n1;
    }
    
    if (p2.x == p3.x && p2.y == p3.y)
    {
    
        length_b = thickness;
        miter_b = n1;
    }

    // generate the triangle strip
	temp = (p1 + length_a * miter_a);
    temp4= float4(temp, 0, 1.0);
    trianglePoints[i++] = temp4;

	temp = (p1 - length_a * miter_a);
	temp4 = float4(temp, 0, 1.0);
    trianglePoints[i++]= temp4;

    temp = (p2 + length_b * miter_b);
	temp4 = float4(temp, 0, 1.0);
    trianglePoints[i++]= temp4;

    temp = (p2 - length_b * miter_b);
	temp4 = float4(temp, 0, 1.0);
    trianglePoints[i++] = temp4;
    
    
    geo_lineWithThicknessResult result;
    result.trianglePoints = trianglePoints;
	result.length =restartStripIndex >= 0 ?7:i; 
    result.restartStripIndex = restartStripIndex;
    
	return result;
}






