//ReflectDiection : reflect(-lightDirection,normalDirection) 
//RdotV:dot(ReflectDiection,normalDirection) 
void PhongNormalDistribution_float(float RdotV, float specularpower, float speculargloss,out float finalSpecular)
{
    float Distribution = pow(RdotV,speculargloss) * specularpower;
    Distribution *= (2+specularpower) / (2*3.1415926535);
    finalSpecular =  Distribution;
}

//halfDirection : normalize(viewDirection + lightDirection)
//NdotH : max(0.0,dot( normalDirection, halfDirection));
void BeckmannNormalDistribution_float(float roughness, float NdotH,out float finalSpecualr)
{
    float roughnessSqr = roughness * roughness;
    float NdotHSqr = NdotH * NdotH;
    finalSpecualr = max(0.000001,(1.0 / (3.1415926535 * roughnessSqr * NdotHSqr * NdotHSqr)) * exp((NdotHSqr-1)/(roughnessSqr * NdotHSqr)));
}

void GGXNormalDistribution_float(float roughness, float NdotH,out float finalSpecular)
{
    float roughnessSqr = roughness * roughness;
    float NdotHSqr = NdotH * NdotH;
    float TanNdotHSqr = (1 - NdotHSqr) / NdotHSqr;
    finalSpecular = (1.0 / 3.1415926535) * sqrt(roughness /(NdotHSqr * (roughnessSqr + TanNdotHSqr)));
}

void GaussianNormalDistribution_float(float roughness, float NdotH,out float finalSpecular)
{
    float roughnessSqr = roughness * roughness;
	float thetaH = acos(NdotH);
    finalSpecular = exp(-thetaH * thetaH/roughnessSqr);
}

void TrowbridgeReitzNormalDistribution_float(float NdotH, float roughness,out float finalSpecular)
{
    float roughnessSqr = roughness * roughness;
    float Distribution = NdotH * NdotH * (roughnessSqr-1.0) + 1.0;
    finalSpecular = roughnessSqr / (3.1415926535 * Distribution*Distribution);
}