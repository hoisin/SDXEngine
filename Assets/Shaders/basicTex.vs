/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};


//////////////
// TYPEDEFS //
//////////////
// Input has normal but we don't use it here
//
struct VertexInputType
{
    float3 position : POSITION;
	float3 normal : NORMAL;		
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType Main(VertexInputType input)
{
    PixelInputType output;
    
    // Change the position vector to be 4 units for proper matrix calculations.
	float4 pos = float4(input.position.xyz, 1);

    // Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(pos, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the input textureUV for the pixel shader to use.
    output.tex = input.tex;
    
    return output;
}