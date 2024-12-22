//--------------------------------------
// GLOBALS
//--------------------------------------
float4x4 gWorldViewProj : WorldViewProjection;
float4x4 gWorldMatrix : WorldMatrix;

float3 gLightDirection : LightDirection;
float3 gCameraPosition : CameraPosition;

Texture2D gDiffuseMap : DiffuseMap;
Texture2D gNormalMap : NormalMap;
Texture2D gSpecularMap : SpecularMap;
Texture2D gGlossinessMap : GlossinessMap;

#define PI 3.1415926535897932384626433832795
#define LIGHTDIR float3(0.577f, -0.577f, 0.577f)

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap; //or Mirror, Clamp, Border
    AddressV = Wrap; //or Mirror, Clamp, Border
};

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap; //or Mirror, Clamp, Border
    AddressV = Wrap; //or Mirror, Clamp, Border
};

SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    AddressU = Wrap; //or Mirror, Clamp, Border
    AddressV = Wrap; //or Mirror, Clamp, Border
};

// Functions
float4 CalcObservedArea(float3 normal)
{
    float4 color = (float4)0;
    
    color.rgb = saturate(dot(normal, -LIGHTDIR));
    color.a = 1.0f;

    return color;
}

float4 SampleNormal(float4 normalSample, float3 inputNormal, float3 inputTangent)
{
    float3 biNormal = cross(inputNormal, inputTangent);
    float4x4 tangentSpaceAxis = float4x4(float4(inputTangent, 1.0f), float4(biNormal, 1.0f), float4(inputNormal, 1.0f), float4(0.0f, 0.0f, 0.0f, 0.0f));

    float4 normal = float4(2.0f * normalSample.xyz - float3(1.0f, 1.0f, 1.0f), 1.0f);
    normal = mul(normal, tangentSpaceAxis);

    return normal;
}

float3 CalcSpecularReflec(float4 specularSample, float glossSample, float3 worldPos, float3 normal)
{
    float3 invViewDir = normalize(gCameraPosition - worldPos);
    float shininess = 25.0f;

    float3 reflectLight = reflect(-LIGHTDIR, normal);
    float cosAlpha = saturate(dot(reflectLight, -invViewDir));
    float3 phong = specularSample.rgb * pow(cosAlpha, shininess * glossSample);

    return phong;
}

//--------------------------------------
// Input/Output Structs
//--------------------------------------
struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Color : COLOR;
    float2 Uv: TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : WORLD;
    float2 Uv : TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

//--------------------------------------
// Vertex Shader
//--------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    float3 modelSpacePosition = input.Position;

    output.WorldPosition = mul(float4(input.Position, 1.0f), gWorldViewProj);
    output.Position = mul(float4(modelSpacePosition, 1.0f), gWorldViewProj);

    output.Uv = input.Uv;
    output.Normal = normalize(mul(input.Normal, (float3x3)gWorldMatrix));
    output.Tangent = normalize(mul(input.Tangent, (float3x3)gWorldMatrix));

    return output;
}

VS_OUTPUT VS_FireFx(VS_INPUT input)
{
    VS_PUTPUT output = (VS_OUTPUT)0;

    output.Position = mul(float4(modelSpacePosition, 1.0f), gWorldViewProj);
    output.Uv = input.Uv;

    return output;
}

//--------------------------------------
// Pixel Shader
//--------------------------------------
float4 PS_Point(VS_OUTPUT input) : SV_TARGET
{
    float kd = 7.0f;
    float4 lambert = (gDiffuseMap.Sample(samPoint, input.Uv) * kd / PI);

    float4 normal = SampleNormal(gNormalMap.Sample(samPoint, input.Uv), input.Normal, input.Tangent);
    float4 observedArea = CalcObservedArea(normal);

    float3 phong = CalcSpecularReflec(gSpecularMap.Sample(samPoint, input.Uv), gGlossinessMap.Sample(samPoint, input.Uv).r, input.WorldPosition.xyz, normal.xyz);

    lambert.rgb += phong;
    return (lambert * observedArea);
}

float4 PS_Linear(VS_OUTPUT input) : SV_TARGET
{
    float kd = 7.0f;
    float4 lambert = (gDiffuseMap.Sample(samLinear, input.Uv) * kd / PI);

    float4 normal = SampleNormal(gNormalMap.Sample(samLinear, input.Uv), input.Normal, input.Tangent);
    float4 observedArea = CalcObservedArea(normal);

    float3 phong = CalcSpecularReflec(gSpecularMap.Sample(samLinear, input.Uv), gGlossinessMap.Sample(samLinear, input.Uv).r, input.WorldPosition.xyz, normal.xyz);

    lambert.rgb += phong;
    return (lambert * observedArea);
}

float4 PS_Anisotropic(VS_OUTPUT input) : SV_TARGET
{
    float kd = 7.0f;
    float4 lambert = (gDiffuseMap.Sample(samAnisotropic, input.Uv) * kd / PI);

    float4 normal = SampleNormal(gNormalMap.Sample(samAnisotropic, input.Uv), input.Normal, input.Tangent);
    float4 observedArea = CalcObservedArea(normal);

    float3 phong = CalcSpecularReflec(gSpecularMap.Sample(samAnisotropic, input.Uv), gGlossinessMap.Sample(samAnisotropic, input.Uv).r, input.WorldPosition.xyz, normal.xyz);

    lambert.rgb += phong;
    return (lambert * observedArea);
}

//--------------------------------------
// Technique
//--------------------------------------
technique11 DefaultTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader( vs_5_0, VS() ));
        SetGeometryShader( NULL );
        SetPixelShader(CompileShader( ps_5_0, PS_Point() ));
    }

    pass P1
    {
        SetVertexShader(CompileShader( vs_5_0, VS() ));
        SetGeometryShader( NULL );
        SetPixelShader(CompileShader( ps_5_0, PS_Linear() ));
    }

    pass P2
    {
        SetVertexShader(CompileShader( vs_5_0, VS() ));
        SetGeometryShader( NULL );
        SetPixelShader(CompileShader( ps_5_0, PS_Anisotropic() ));
    }
}