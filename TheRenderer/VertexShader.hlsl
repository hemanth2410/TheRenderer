struct VsOut
{
    float4 color : COLOR;
    float4 pos : SV_Position;
};

VsOut main(float4 pos : POSITION, float4 color : COLOR)
{
    VsOut vso;
    vso.pos = float4(pos.x, pos.y, 0, 1);
    vso.color = color;
    return vso;
}