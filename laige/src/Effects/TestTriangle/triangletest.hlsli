
struct Interpolants
{
    float4 position : SV_Position;
    float4 color : COLOR0;
};


struct Vertex
{
    float4 position : SV_Position;
    float4 color : COLOR0;
};

struct Pixel
{
    float4 color : SV_Target;
};



Interpolants VS(Vertex In)
{
    return In;
}


Pixel PS(Interpolants In)
{
    Pixel Out;
    Out.color = In.color;
    return Out;
}