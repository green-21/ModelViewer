#include "common.hlsli"

Texture2D randeredTexture : register(t10);
Texture2D depthMap : register(t11);
SamplerState basicSampler : register(s0);

struct PSInput
{
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float2 uv : UV;
};

float4 main(PSInput input) : SV_Target
{
    float r = depthMap.Sample(basicSampler, input.uv).r;
    
    // TODO: Magic number;
    float4 color = randeredTexture.Sample(basicSampler, input.uv);
    float z = clamp(1 - length(TexcoordToView(input.uv, r)) * 0.008, 0.0, 1.0);
    
    return lerp(float4(0.3, 0.3, 0.3, 1.0), color, z);
}