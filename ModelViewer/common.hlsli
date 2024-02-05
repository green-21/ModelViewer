#ifndef __COMMON_HLSLI__
#define __COMMON_HLSLI__


cbuffer CameraTransform : register(b0)
{
    matrix view;
    matrix projection;
    matrix invProjection;
}

#endif // __COMMON_HLSLI__