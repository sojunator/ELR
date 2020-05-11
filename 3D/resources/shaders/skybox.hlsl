
cbuffer CameraBuffer : register(b0)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

cbuffer ModelBuffer : register(b1)
{
	float4x4 worldMatrix;
};


struct VOut
{
	float4 position : SV_POSITION;
	float3 texcoord : TEXCOORD;
    float4 test : TEST;
};

VOut VSmain(float4 position : POSITION)
{
	VOut output;
	// Output to projection space

	output.texcoord = position.xyz;
    output.position = mul(position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
   


	return output;
}


TextureCube skyboxTexture : register(t2);

SamplerState ss : register(s0);

struct POut
{
    float4 normal : SV_Target0;
    float4 albedo : SV_Target1;
    float4 specular : SV_Target2;
    float4 position : SV_Target3;
};

POut PSmain(VOut input) : SV_TARGET
{

    POut output;
    output.albedo = skyboxTexture.Sample(ss, input.texcoord);

    return output;
}