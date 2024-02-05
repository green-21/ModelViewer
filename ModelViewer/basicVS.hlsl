#include "common.hlsli"

struct VSInput
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float2 uv : UV;
};

cbuffer ModelTransform : register(b1)
{
    matrix model;
};


VSOutput main(VSInput input)
{
    VSOutput output;
    float4 pos = float4(input.pos, 1.0);
    pos = mul(pos, model);
    pos = mul(pos, view);
    pos = mul(pos, projection);
    
    output.pos = pos;
    output.normal = input.normal;
    output.uv = input.uv;
    return output;
}