//--------------------------------------
// GLOBALS
//--------------------------------------
float4x4 gWorldViewProj : WorldViewProjection;
float4x4 gWorldMatrix : WorldMatrix;

Texture2D gDiffuseMap : DiffuseMap;

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap; //or Mirror, Clamp, Border
    AddressV = Wrap; //or Mirror, Clamp, Border
};


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
// Rasterizer-, Blend- and DepthStencilState
//--------------------------------------
RasterizerState gRasterizerState
{
    CullMode = none;
    FrontCounterClockwise = false; //default
};

BlendState gBlendState
{
    BlendEnable[0] = true;
    SrcBlend = src_alpha;
    DestBlend = inv_src_alpha;
    BlendOp = add;
    SrcBlendAlpha = zero;
    DestBlendAlpha = zero;
    BlendOpAlpha = add;
    RenderTargetWriteMask[0] = 0x0F;
};

DepthStencilState gDepthStencilState
{
    DepthEnable = true;
    DepthWriteMask = zero;
    DepthFunc = less;
    StencilEnable = false;

    // Others are redundant because StencilEnable is false (for demo only)
    StencilReadMask = 0x0F;
    StencilWriteMask = 0x0F;

    FrontFaceStencilFunc = always;
    BackFaceStencilFunc = always;

    FrontFaceStencilDepthFail = keep;
    BackFaceStencilDepthFail = keep;

    FrontFaceStencilPass = keep;
    BackFaceStencilPass = keep;

    FrontFaceStencilFail = keep;
    BackFaceStencilFail = keep;
};

//--------------------------------------
// Vertex Shader
//--------------------------------------
VS_OUTPUT VS_FireFx(VS_INPUT input)
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


//--------------------------------------
// Pixel Shader
//--------------------------------------
float4 PS_FireFx(VS_OUTPUT input) : SV_TARGET
{
    float4 color = gDiffuseMap.Sample(samPoint, input.Uv);
    return color;
}


//--------------------------------------
// Technique
//--------------------------------------
technique11 DefaultTechnique
{
    pass P0
    {
        SetRasterizerState(gRasterizerState);
        SetDepthStencilState(gDepthStencilState, 0);
        SetBlendState(gBlendState, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader(CompileShader( vs_5_0, VS_FireFx() ));
        SetGeometryShader( NULL );
        SetPixelShader(CompileShader( ps_5_0, PS_FireFx() ));
    }
}