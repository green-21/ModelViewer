
Texture2D texture0 : register(t0);
SamplerState basicSampler : register(s0);

struct PSInput
{
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float2 uv : UV;
};

float4 main(PSInput input) : SV_Target
{
    return float4(input.normal, 1.0);
}