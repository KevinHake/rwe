cbuffer VertexUniforms : register(b0, space1)
{
    float4x4 mvpMatrix;
};

struct VSInput
{
    float3 position : TEXCOORD0;
    float3 color    : TEXCOORD1;
};

struct VSOutput
{
    float4 position : SV_Position;
    float3 color    : TEXCOORD0;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.position = mul(mvpMatrix, float4(input.position, 1.0));
    output.color = input.color;
    return output;
}
