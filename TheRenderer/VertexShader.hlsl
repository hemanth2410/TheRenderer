struct VsOut
{
    float4 color : COLOR;
    float4 pos : SV_Position;
};

cbuffer Cbuf
{
    matrix transform;
};

VsOut main(float4 pos : POSITION, float4 color : COLOR)
{
    VsOut vso;
    vso.pos = mul(float4(pos.x, pos.y, 0, 1), transform);
    vso.color = color;
    return vso;
}