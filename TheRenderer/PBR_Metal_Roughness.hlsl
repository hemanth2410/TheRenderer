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

cbuffer TransformCbuf
{
    matrix modelView;
    matrix modelviewProj;
    matrix normalMatrix;
};

Texture2D Albedo;
Texture2D spec : register(t1);
Texture2D NormalMap : register(t2);
Texture2D ORM : register(t3);
SamplerState splr;

// Hevy math stuff
static const float PI = 3.14159265f;

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0f);
    float NdotH2 = NdotH * NdotH;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    return a2 / (PI * denom * denom);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0f;
    float k = (r * r) / 8.0f; // direct-lighting remapping
    return NdotV / (NdotV * (1.0f - k) + k);
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0f);
    float NdotL = max(dot(N, L), 0.0f);
    return GeometrySchlickGGX(NdotV, roughness) * GeometrySchlickGGX(NdotL, roughness);
}

float3 FresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0f - F0) * pow(saturate(1.0f - cosTheta), 5.0f);
}

float4 main(float3 worldPos : Position, float3 n : Normal, float3 tan : Tangent, float3 bitan : Bitangent, float2 tc : Texcoord) : SV_Target
{
    float4 albedo = Albedo.Sample(splr, tc);
#ifdef ALPHA_MASK_ENABLED
    // Calculations for Normal based on availability of normal map
    if (dot(n, worldPos) >= 0.0f)
    {
        n = -n;
    }
    clip(albedo.a < 0.1f ? -1 : 1);
#endif
    float3 N = normalize(n);
    
    if (useNormalMap)
    {
        float3x3 tanToView = float3x3(
            normalize(tan),
            normalize(bitan),
            normalize(n)
            );
        float3 normalSample = NormalMap.Sample(splr, tc).xyz;
        N.x = normalSample.x * 2.0f - 1.0f;
        N.y = -normalSample.y * 2.0f + 1.0f;
        N.z = normalSample.z;
        N = mul(N, tanToView);
        N = normalize(N);
    }
    float3 V = normalize(-worldPos);
    float3 L = normalize(lightPos - worldPos);
    float3 H = normalize(V + L);
    float metallic;
    float roughness;
    float ao;
    if (ormMap)
    {
        metallic = ORM.Sample(splr, tc).b;
        roughness = ORM.Sample(splr, tc).g;
        //roughness = 0;
        ao = ORM.Sample(splr, tc).r;
    }
    else
    {
        metallic = metallicFactor;
        roughness = roughnessFactor;
        ao = ambientFactor;
    }
    const float distToL = length(lightPos - worldPos); // you already compute L = normalize(lightPos - worldPos) above; distToL needs the un-normalized version too
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
    
    float3 F0 = lerp(float3(0.04f, 0.04f, 0.04f), albedo.rgb, metallic); // dielectrics ~4% reflectance; metals tint by albedo
    float D = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    float3 F = FresnelSchlick(max(dot(H, V), 0.0f), F0);
    float3 numerator = D * G * F;
    float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.0001f; // epsilon avoids div-by-zero
    float3 specular = numerator / denominator;

    float3 kD = (1.0f - F) * (1.0f - metallic);
    float3 diffuse = kD * albedo.rgb / PI;

    float NdotL = max(dot(N, L), 0.0f);
    float3 outColor = (diffuse + specular + (ambient * ao)) * diffuseColor * diffuseIntensity * att * NdotL;
    //float3 outColor = float3(ao, roughness, metallic);
    return float4(saturate(outColor), 1.0f);
}