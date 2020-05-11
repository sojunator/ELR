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
	float4 color : COLOR;
	float4 normal : NORMAL;
	float2 texcoord : TEXCOORD;
};

VOut VSmain(float4 position : POSITION, float4 color : COLOR, float4 normal : NORMAL, float2 texcoord : TEXCOORD0)
{
	VOut output;

	output.position = mul(position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.color = color;
	output.texcoord = texcoord;
	output.normal = mul(mul(normal, worldMatrix), viewMatrix);
	return output;
}


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
    output.normal = input.normal;
    output.position = input.position;
    output.specular = float4(1.0f, 1.0f, 1.0f, 1.0f);
    output.albedo = input.color;
    return output;
}

