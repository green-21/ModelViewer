
Texture2D texture0 : register(t0);
SamplerState basicSampler : register(s0);

struct PSInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
};

float4 main(PSInput input) : SV_Target
{
    return texture0.Sample(basicSampler, input.uv);
}