cbuffer Cbuf
{
    matrix transform;
};

float4 main(float4 pos : POSITION) : SV_Position
{
    return (mul(float4(pos.x, pos.y, pos.z, 1), transform));
}