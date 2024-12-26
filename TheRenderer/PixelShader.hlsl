float4 main(float3 color: COLOR) : SV_TARGET
{
    return float4(color.r, color.g, color.b, 1.0f);
}