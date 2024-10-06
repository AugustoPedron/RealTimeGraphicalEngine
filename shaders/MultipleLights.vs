#define NUM_LIGHTS 4

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer LightPositionBuffer
{
    float4 lightPosition[NUM_LIGHTS];
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 lightPos[NUM_LIGHTS] : TEXCOORD1;
};

PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;
    int i;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(mul(worldPosition, viewMatrix), projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

     // Calculate the normal vector against the world matrix only and normalize it.
    output.normal = normalize(mul(input.normal, (float3x3)worldMatrix));

    for(i=0; i<NUM_LIGHTS; i++)
    {
        // Determine the light positions based on the position of the lights and the position of the vertex in the world and normalize it.
        output.lightPos[i] = normalize(lightPosition[i].xyz - worldPosition.xyz);
    }

    return output;
}