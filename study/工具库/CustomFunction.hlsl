void BlinnPhong_half(Texture2D remapTex,SamplerState ss,half3 albedo,half3 lightColor ,half3 specularColor, half3 worldNormal, half3 worldLightDir, half3 worldViewDir,half noise,out half4 finalColor)
{
	worldLightDir = normalize(worldLightDir);
	worldNormal = normalize(worldNormal);
	worldViewDir = normalize(worldViewDir);

	//漫反射
	half value = (dot(worldNormal, worldLightDir) + 1.01) / 2.02;
	half remapShadow = SAMPLE_TEXTURE2D(remapTex,ss, half2(clamp(value,0,1), 0.5));
	float3 diff = lightColor * albedo * remapShadow;

	//高光
	//half3 halfDir = normalize(worldViewDir + worldLightDir);
	//half3 spec = (specularColor.rgb * lightColor) * pow(saturate(dot(halfDir, worldNormal)), gloss);
	//finalColor = float4((diff + spec * noise ), 1);
	finalColor = float4((diff * noise * specularColor * 2.5 ), 1);
}

void GetMainLight_half(half3 WorldPos, out half3 LightDirection, out half3 LightColor, out half DistanceAtten)
{
    #if SHADERGRAPH_PREVIEW
	    LightDirection = half3(0.5, 0.5, 0);
		LightColor = 1;
    	DistanceAtten = 1;
    #else
        half4 shadowCoord = TransformWorldToShadowCoord(WorldPos);
    	Light mainLight = GetMainLight(shadowCoord);
		LightDirection = mainLight.direction;
		LightColor = mainLight.color;
    	DistanceAtten = mainLight.distanceAttenuation;
    #endif
}

//unity remap
void Remap_float(float inputValue, float2 InMinMax, float2 OutMinMax, out float outValue)
{
    outValue = OutMinMax.x + (inputValue - InMinMax.x) * (OutMinMax.y - OutMinMax.x) / (InMinMax.y - InMinMax.x);
}

//正交相机深度
void GetDepthOrthoCamera_float(float sceneRawDepth,float4 screenPositionRaw,float3 viewSpacePosition,out float depthLerpValue,out float3 worldPosition)
{
     float zRevert = _ProjectionParams.x == -1 ? 1.0 : 0.0;
     //正交相机 zRevert = -1 反转深度图 
     float sampleDepth = lerp(sceneRawDepth,1-sceneRawDepth,zRevert);
     //cameraNearClip,cameraFarClip,depth
     float sceneDepthValueViewSpace = lerp(_ProjectionParams.y,_ProjectionParams.z,sampleDepth);

     float remapValue;
     Remap_float(screenPositionRaw.z,
                 float2(UNITY_NEAR_CLIP_VALUE,UNITY_RAW_FAR_CLIP_VALUE),
                 float2(0,1),remapValue);

     float objDepthViewSpace = lerp(_ProjectionParams.y,_ProjectionParams.z,remapValue);
     depthLerpValue = saturate(1-(sceneDepthValueViewSpace - objDepthViewSpace));

     //recontrust world position
     float3 viewSpacePos = float3(viewSpacePosition.x,viewSpacePosition.y,-sceneDepthValueViewSpace);
     worldPosition = mul(UNITY_MATRIX_I_V, float4(viewSpacePos, 1)).xyz;
}