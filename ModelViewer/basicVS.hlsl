struct VSInput
{
    float3 pos : POSITION;
    float3 color : COLOR;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float3 color : COLOR;
};

VSOutput main(VSInput input)
{
    float4 pos = float4(input.pos, 1.0);

    VSOutput output;
    output.pos = pos;
    output.color = input.color;

    return output;
}
