Texture2D randeredTexture : register(t10);
Texture2D depthMap : register(t11);
SamplerState basicSampler : register(s0);

struct PSInput
{
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float2 uv : UV;
};

cbuffer CameraTransform : register(b0)
{
    matrix view;
    matrix projection;
    matrix invProjection;
}

float4 TexcoordToView(float2 texcoord, float z)
{
    float4 ndcPos;
    
    // uv -> ndc
    ndcPos.xy = texcoord * 2.0 - 1.0;
    ndcPos.y *= -1;
    ndcPos.z = z;
    ndcPos.w = 1.0;

    // ndc -> world
    float4 viewPos = mul(ndcPos, invProjection);
    viewPos.xyz /= viewPos.w;
    
    return viewPos;
}

float4 main(PSInput input) : SV_Target
{
    float r = depthMap.Sample(basicSampler, input.uv).r;\
    
    // TODO: Magic number;
    float z = clamp(1 - length(TexcoordToView(input.uv, r)) * 0.008, 0.2, 1.0);
    float4 color = randeredTexture.Sample(basicSampler, input.uv);
    color.xyz *= z;
    return color;
}