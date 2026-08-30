cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

cbuffer ObjectCBuf
{
    float specularIntensity; // fallback when !useSpecularMap
    float specularPower; // fallback when !useSpecularMap
    int useSpecularMap;
    int useNormalMap;
    float metallicFactor; // fallback when !useMetalnessMap
    float roughnessFactor; // fallback when !useRoughnessMap
    float ambientFactor;
    int ormMap;
};

//cbuffer TransformCbuf
//{
//    matrix modelView;
//    matrix modelviewProj;
//    matrix normalMatrix;
//};

Texture2D tex;
Texture2D spec : register(t1);
Texture2D normalMap : register(t2);
SamplerState splr;


float4 main(float3 worldPos : Position, float3 n : Normal, float3 tan : Tangent, float3 bitan : Bitangent, float2 tc : Texcoord) : SV_Target
{ 
    n = normalize(n);
    if(useNormalMap)
    {
        float3x3 tanToView = float3x3(
            normalize(tan),
            normalize(bitan),
            normalize(n)
            );
        float3 normalSample = normalMap.Sample(splr, tc).xyz;
        n.x = normalSample.x * 2.0f - 1.0f;
        n.y = -normalSample.y * 2.0f + 1.0f;
        n.z = normalSample.z; 
        n = mul(n, tanToView);
    }

	// fragment to light vector data
    const float3 vToL = lightPos - worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
	// attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));
	// reflected light vector
    const float3 w = n * dot(vToL, n);
    const float3 r = w * 2.0f - vToL;
	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    //const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(worldPos))), specularPower);
    float3 specularColor;
    float specularPowerVal;
    if(useSpecularMap)
    {
        const float4 specularSample = spec.Sample(splr, tc);
        specularColor = specularSample.rgb;
        specularPowerVal = pow(2.0f, specularSample.a * 20);
    }
    else
    {
        specularColor = float3(1.0, 1.0, 1.0);
        specularPowerVal = specularPower;
    }
    //const float specularPower = specularSample.a * 100;
    const float3 specular = att * (diffuseColor * diffuseIntensity) * pow(max(0.0f, dot(normalize(-r), normalize(-worldPos))), specularPowerVal);
	// final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular * specularColor), 1.0f);
}