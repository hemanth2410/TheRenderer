#include "_PShadow_Static.hlsli"

//Texture2D smap : register(t4);
//SamplerComparisonState ssamHw : register(s1);
//SamplerState ssamSw : register(s2);

//cbuffer ShadowControl : register(b5)
//{
//    int pcfLevel;
//    float depthBias;
//    bool hwPcf;
//}

//#define PCF_RANGE 2

//float ShadowLoop_(const in float3 spos, uniform int range)
//{
//    float shadowLevel = 0.0f;
//    [unroll]
//    for (int x = -range; x <= range; x++)
//    {
//        [unroll]
//        for (int y = -range; y <= range; y++)
//        {
//            if (hwPcf)
//            {
//                shadowLevel += smap.SampleCmpLevelZero(ssamHw, spos.xy, spos.b - depthBias, int2(x, y));
//            }
//            else
//            {
//                shadowLevel += smap.Sample(ssamSw, spos.xy, int2(x, y)).r >= spos.b - depthBias ? 1.0f : 0.0f;
//            }
//        }
//    }
//    return shadowLevel / ((range * 2 + 1) * (range * 2 + 1));
//}

//float Shadow(const in float4 shadowHomoPos)
//{
//    float shadowLevel = 0.0f;
//    const float3 spos = shadowHomoPos.xyz / shadowHomoPos.w;
    
//    if (spos.z > 1.0f || spos.z < 0.0f)
//    {
//        shadowLevel = 1.0f;
//    }
//    else
//    {
//        [unroll]
//        for (int level = 0; level <= 4; level++)
//        {
//            if (level == pcfLevel)
//            {
//                shadowLevel = ShadowLoop_(spos, level);
//            }
//        }
//    }
//    return shadowLevel;
//}


////float Shadow(const in float4 shadowHomoPos)
////{
////    const float3 spos = shadowHomoPos.xyz / shadowHomoPos.w;
////    float shadowLevel = 0.0;
////    if (spos.z > 1.0f || spos.z < 0.0f)
////        shadowLevel = 1.0f;
////    else
////    {
////        //uint width, height;
////        //smap.GetDimensions(width, height);
////        //const float dx = 0.5f / width;
////        //const float dy = 0.5f / height;
////        //const float zBiased = spos.z - 0.0005f;
////        //shadowLevel += smap.Sample(ssam, spos.xy + float2(dx, dy)).r >= zBiased ? 0.25f : 0.0f;
////        //shadowLevel += smap.Sample(ssam, spos.xy + float2(-dx, dy)).r >= zBiased ? 0.25f : 0.0f;
////        //shadowLevel += smap.Sample(ssam, spos.xy + float2(dx, -dy)).r >= zBiased ? 0.25f : 0.0f;
////        //shadowLevel += smap.Sample(ssam, spos.xy + float2(-dx, -dy)).r >= zBiased ? 0.25f : 0.0f;
        
        
////        //Multi sample Hardware PCF
////        //uint width, height;
////        //smap.GetDimensions(width, height);
////        //const float2 texelSize = 1.0f / float2(width, height);
////        //const float zBiased = spos.z - 0.0005f;
////        //[unroll]
////        //for (int x = -PCF_RANGE; x <= PCF_RANGE; x ++)
////        //{
////        //    [unroll]
////        //    for (int y = -PCF_RANGE; y <= PCF_RANGE; y ++)
////        //    {
////        //        float2 uvOffset = float2(x, y) * texelSize;
////        //        shadowLevel += smap.SampleCmpLevelZero(ssam, spos.xy + uvOffset, zBiased);
////        //    }

////        //}
////        //shadowLevel /= ((PCF_RANGE * 2 + 1) * (PCF_RANGE * 2 + 1));
////        shadowLevel = smap.SampleCmpLevelZero(ssam, spos.xy, spos.b - 0.0005f);

////    }
////    return shadowLevel;
////}


////bool ShadowUnoccluded(const in float4 shadowHomoPos)
////{
////    const float3 spos = shadowHomoPos.xyz / shadowHomoPos.w;
////    return spos.z > 1.0f ? true : Shadow(shadowHomoPos) > 0.25f;
////}

