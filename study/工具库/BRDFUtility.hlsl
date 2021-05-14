#define kDieletricSpec half4(0.04, 0.04, 0.04, 1.0 - 0.04)
struct CustomBRDFData
{
    half3 diffuse;
    half3 specular;
    half perceptualRoughness;
    half roughness;
    half roughness2;
    half grazingTerm;

    // We save some light invariant BRDF terms so we don't have to recompute
    // them in the light loop. Take a look at DirectBRDF function for detailed explaination.
    half normalizationTerm;     // roughness * 4.0 + 2.0
    half roughness2MinusOne;    // roughness^2 - 1.0
};

half OrcasPerceptualRoughnessToMipmapLevel(half perceptualRoughness)
{
    perceptualRoughness = perceptualRoughness * (1.7 - 0.7 * perceptualRoughness);
    return perceptualRoughness * 6;
}

half CustomPositivePow(half base,half power)
{
    return pow(abs(base), power);
}

half3 CustomDecodeHDREnvironment(half4 encodedIrradiance, half4 decodeInstructions)
{
    // Take into account texture alpha if decodeInstructions.w is true(the alpha value affects the RGB channels)
    half alpha = max(decodeInstructions.w * (encodedIrradiance.a - 1.0) + 1.0, 0.0);
    // If Linear mode is not supported we can skip exponent part
    return (decodeInstructions.x * CustomPositivePow(alpha, decodeInstructions.y)) * encodedIrradiance.rgb;
}

half3 CustomGlossyEnvironmentReflection(half3 reflectVector, half perceptualRoughness, half occlusion)
{
    //environment reflection 
    half mip = OrcasPerceptualRoughnessToMipmapLevel(perceptualRoughness);
    half4 encodedIrradiance = SAMPLE_TEXTURECUBE_LOD(unity_SpecCube0,samplerunity_SpecCube0, reflectVector, mip);
    //默认decode hdr
    half3 irradiance = CustomDecodeHDREnvironment(encodedIrradiance, unity_SpecCube0_HDR);
    //half3 irradiance = encodedIrradiance.rbg;
    return irradiance * occlusion;
}

half CustomOneMinusReflectivityMetallic(half metallic)
{
    //kDieletricSpec half4(0.04, 0.04, 0.04, 1.0 - 0.04)
    half oneMinusDielectricSpec = 0.96;
    return oneMinusDielectricSpec - metallic * oneMinusDielectricSpec;
}

half CustomPerceptualSmoothnessToPerceptualRoughness(half perceptualSmoothness)
{
    return (1.0 - perceptualSmoothness);
}

half CustomPerceptualRoughnessToRoughness(half perceptualRoughness)
{
    return perceptualRoughness * perceptualRoughness;
}

//只做了金属流程的BRDF,Specular流程去掉
void CustomInitializeBRDFData(half3 albedo, half metallic, half smoothness, half alpha, out CustomBRDFData outBRDFData)
{
    half oneMinusReflectivity = CustomOneMinusReflectivityMetallic(metallic);
    half reflectivity = 1.0 - oneMinusReflectivity;

    outBRDFData.diffuse = albedo * oneMinusReflectivity;
    outBRDFData.specular = lerp(kDieletricSpec.rgb, albedo, metallic);
    outBRDFData.grazingTerm = saturate(smoothness + reflectivity);
    outBRDFData.perceptualRoughness = CustomPerceptualSmoothnessToPerceptualRoughness(smoothness);
    outBRDFData.roughness = max(CustomPerceptualRoughnessToRoughness(outBRDFData.perceptualRoughness), HALF_MIN);
    outBRDFData.roughness2 = outBRDFData.roughness * outBRDFData.roughness;

    outBRDFData.normalizationTerm = outBRDFData.roughness * 4.0 + 2.0;
    outBRDFData.roughness2MinusOne = outBRDFData.roughness2 - 1.0;

#ifdef _ALPHAPREMULTIPLY_ON
    outBRDFData.diffuse *= alpha;
    alpha = alpha * oneMinusReflectivity + reflectivity;
#endif
}

half3 CustomSubtractDirectMainLightFromLightmap(half shadowStrength,half3 direction,half3 lightColor,half distanceAttenuation,
                                          half shadowAttenuation, half3 normalWS, half3 bakedGI)
{
    half4 _SubtractiveShadowColor = half4(0,0,0,1);
    
    half contributionTerm = saturate(dot(direction, normalWS));
    half3 lambert = lightColor * contributionTerm;
    half3 estimatedLightContributionMaskedByInverseOfShadow = lambert * (1.0 - shadowAttenuation);
    half3 subtractedLightmap = bakedGI - estimatedLightContributionMaskedByInverseOfShadow;

    half3 realtimeShadow = max(subtractedLightmap, _SubtractiveShadowColor.xyz);
    realtimeShadow = lerp(bakedGI, realtimeShadow, shadowStrength);

    return min(bakedGI, realtimeShadow);
}

void CustomMixRealtimeAndBakedGI(half shadowStrength,half3 direction,half3 lightColor,half distanceAttenuation,half shadowAttenuation,half3 normalWS,inout half3 bakedGI)
{
    bakedGI = CustomSubtractDirectMainLightFromLightmap(shadowStrength,direction,lightColor,distanceAttenuation,shadowAttenuation,normalWS,bakedGI);
}

half3 CustomEnvironmentBRDF(CustomBRDFData brdfData, half3 indirectDiffuse, half3 indirectSpecular, half fresnelTerm)
{
    half3 c = indirectDiffuse * brdfData.diffuse;
    float surfaceReduction = 1.0 / (brdfData.roughness2 + 1.0);
    c += surfaceReduction * indirectSpecular * lerp(brdfData.specular, brdfData.grazingTerm, fresnelTerm);
    return c;
}

half3 CustomGlobalIllumination(CustomBRDFData brdfData, half3 bakedGI, half occlusion, half3 normalWS, half3 viewDirectionWS)
{
    half3 reflectVector = reflect(-viewDirectionWS, normalWS);
    half fresnelTerm = Pow4(1.0 - saturate(dot(normalWS, viewDirectionWS)));

    half3 indirectDiffuse = bakedGI * occlusion;
    half3 indirectSpecular = CustomGlossyEnvironmentReflection(reflectVector, brdfData.perceptualRoughness, occlusion);

    return CustomEnvironmentBRDF(brdfData, indirectDiffuse, indirectSpecular, fresnelTerm);
}

half3 CustomSafeNormalize(half3 inVec)
{
    float dp3 = max(HALF_MIN, dot(inVec, inVec));
    return inVec * rsqrt(dp3);
}

//_SPECULARHIGHLIGHTS_OFF这个宏默认打开，不在做分支控制
half3 CustomDirectBDRF(CustomBRDFData brdfData, half3 normalWS, half3 lightDirectionWS, half3 viewDirectionWS)
{
    float3 halfDir = CustomSafeNormalize(float3(lightDirectionWS) + float3(viewDirectionWS));

    float NoH = saturate(dot(normalWS, halfDir));
    half LoH = saturate(dot(lightDirectionWS, halfDir));

    // GGX Distribution multiplied by combined approximation of Visibility and Fresnel
    // BRDFspec = (D * V * F) / 4.0
    // D = roughness^2 / ( NoH^2 * (roughness^2 - 1) + 1 )^2
    // V * F = 1.0 / ( LoH^2 * (roughness + 0.5) )
    // See "Optimizing PBR for Mobile" from Siggraph 2015 moving mobile graphics course
    // https://community.arm.com/events/1155

    // Final BRDFspec = roughness^2 / ( NoH^2 * (roughness^2 - 1) + 1 )^2 * (LoH^2 * (roughness + 0.5) * 4.0)
    // We further optimize a few light invariant terms
    // brdfData.normalizationTerm = (roughness + 0.5) * 4.0 rewritten as roughness * 4.0 + 2.0 to a fit a MAD.
    float d = NoH * NoH * brdfData.roughness2MinusOne + 1.00001f;

    half LoH2 = LoH * LoH;
    half specularTerm = brdfData.roughness2 / ((d * d) * max(0.1h, LoH2) * brdfData.normalizationTerm);

    // On platforms where half actually means something, the denominator has a risk of overflow
    // clamp below was added specifically to "fix" that, but dx compiler (we convert bytecode to metal/gles)
    // sees that specularTerm have only non-negative terms, so it skips max(0,..) in clamp (leaving only min(100,...))
#if defined (SHADER_API_MOBILE) || defined (SHADER_API_SWITCH)
    specularTerm = specularTerm - HALF_MIN;
    specularTerm = clamp(specularTerm, 0.0, 100.0); // Prevent FP16 overflow on mobiles
#endif

    half3 color = specularTerm * brdfData.specular + brdfData.diffuse;
    return color;
}

//lightAttenuation = shadowAttenuation * distanceAttenuation
half3 CustomLightingPhysicallyBased(CustomBRDFData brdfData, half3 lightColor, half3 lightDirectionWS, half lightAttenuation, half3 normalWS, half3 viewDirectionWS)
{
    half NdotL = saturate(dot(normalWS, lightDirectionWS));
    half3 radiance = lightColor * (lightAttenuation * NdotL);
    return CustomDirectBDRF(brdfData, normalWS, lightDirectionWS, viewDirectionWS) * radiance;
}

//every thing start here
//不做多光源
void UniversalPBR_half(half shadowStrength,half3 lightDirection,half3 lightColor,half distanceAttenuation,half shadowAttenuation,half3 bakedGI,
                       half3 normalWS,half3 viewDirectionWS,
                       half3 albedo, half metallic,half smoothness, half occlusion, half3 emission, half alpha,out half4 finalColor)
{
    CustomBRDFData brdfData;
    CustomInitializeBRDFData(albedo, metallic,smoothness, alpha, brdfData);
    CustomMixRealtimeAndBakedGI(shadowStrength,lightDirection,lightColor,distanceAttenuation,shadowAttenuation,normalWS,bakedGI);
    half3 color = CustomGlobalIllumination(brdfData,bakedGI,occlusion,normalWS,viewDirectionWS);
    color += CustomLightingPhysicallyBased(brdfData,lightColor,lightDirection,distanceAttenuation * shadowAttenuation,normalWS,viewDirectionWS);
    color += emission;
    finalColor = half4(color, alpha);
}