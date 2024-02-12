#ifndef __COMMON_HLSLI__
#define __COMMON_HLSLI__


cbuffer CameraTransform : register(b0)
{
    matrix view;
    matrix projection;
    matrix invProjection;
    float3 cameraPos;
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

#endif // __COMMON_HLSLI__