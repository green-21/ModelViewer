#include "common.hlsli"

Texture2D texture0 : register(t0);

SamplerState basicSampler : register(s0);
struct PSInput
{
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float2 uv : UV;
};

// reference
// https://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/
// https://ruby0x1.github.io/machinery_blog_archive/post/borderland-between-rendering-and-editor-part-1/index.html
// https://www.shadertoy.com/view/WtyGDy
float4 Grid(PSInput input)
{
    const float scale = 1000.0; // 0~1 -> 0~scale
    const float unitCell = 1.0; // 보여질 범위 관련 (잘 모르겠음)
    const float cs = 0.5; // 최소 셀
    
    float2 uv = input.uv * scale;
    float2 der = fwidth(uv);
    
    float lod_level = max(0, log10((length(der) * unitCell) / cs) + 1);
    float lod_fade = frac(lod_level); // lod_level의 소수부

    
    float lod0_cs = cs * pow(10, floor(lod_level));
    float lod1_cs = lod0_cs * 10.f;
    float lod2_cs = lod1_cs * 10.f;
    
    
    der *= 2; // 선의 굵기
    
    // Calculate unsigned distance to cell line center for each lod [3]
    float2 lod0_cross_a = 1.f - abs(saturate(fmod(uv, lod0_cs) / der) * 2 - 1.f);
    // Pick max of x,y to get a coverage alpha value for lod
    float lod0_a = max(lod0_cross_a.x, lod0_cross_a.y);
    float2 lod1_cross_a = 1.f - abs(saturate(fmod(uv, lod1_cs) / der) * 2 - 1.f);
    float lod1_a = max(lod1_cross_a.x, lod1_cross_a.y);
    float2 lod2_cross_a = 1.f - abs(saturate(fmod(uv, lod2_cs) / der) * 2 - 1.f);
    float lod2_a = max(lod2_cross_a.x, lod2_cross_a.y);
    
    float4 thin_color = float4(0.3, 0.3, 0.3, 1.0); // 배경색
    float4 thick_color = float4(0.7, 0.7, 0.7, 1.0); // 선색
    
    // Blend between falloff colors to handle LOD transition [4]
    float4 c = lod2_a > 0 ? thick_color : lod1_a > 0 ? lerp(thick_color, thin_color, lod_fade) : thin_color;

    // Calculate opacity falloff based on distance to grid extents and gracing angle. [5]
    float3 view_dir = normalize(-cameraPos);
    // TODO: 이 코드는 도저히 이해하지 못하겠음.
    //float op_gracing = max(0.1, 1.f - pow(1.f - abs(dot(view_dir, float3(view[0][0], view[0][1], view[0][2]))), 16));
    float op_distance = (1.f - saturate(length(2 * uv - scale) / scale));
    float op = op_distance;

    // Blend between LOD level alphas and scale with opacity falloff. [6]
    c.a *= (lod2_a > 0 ? lod2_a : lod1_a > 0 ? lod1_a : (lod0_a * (1 - lod_fade))) * op;
    return c;
}

float4 main(PSInput input) : SV_Target
{
    return Grid(input);
}