cbuffer LightCBuf
{
    float3 lightPos;
    float4 lightColor;
    float4 ambient;
    float4 diffuseLight;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};
cbuffer ObjectCbuf
{
    float3 materialColor;
    float specularIntensity;
    float specularPower;
};
/*static const float3 materialColor = { 0.7f, 0.7f, 0.9f };
static const float3 ambient = { 0.05f, 0.05f, 0.05f };
static const float3 diffuseLight = { 1.0f, 1.0f, 1.0f };

static const float diffuseIntensity = 1.0f;
static const float attConst = 1.0f;
static const float attLin = 0.045f;
static const float attQuad = 0.0075f;*/

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_TARGET
{
    const float3 vTol = lightPos - worldPos;
    const float distTol = length(vTol);
    const float3 dirTol = vTol / distTol;
    const float att = 1.0f / (attConst + attLin * distTol + attQuad * (distTol * distTol));
    const float3 diffuse = lightColor.xyz * diffuseIntensity * att * max(0.0f, dot(dirTol, n));
    const float3 w = n * dot(vTol, n);
    const float3 r = w * 2.0f - vTol;
    const float3 specular = att * (diffuse * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(worldPos))), specularPower);
   
    return float4(saturate(diffuse + ambient.xyz * specular) * materialColor.xyz, 1.0f);
}