// Vertexshader
cbuffer CameraBuffer : register(b0)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

cbuffer ModelBuffer : register(b1)
{
    float4x4 worldMatrix;
};

// Pixelshader
cbuffer mtrBuffer : register(b0)
{
    float4 mtrspecular;
    float4 mtrambient;
    float4 mtrdiffuse;
    float4 shininess;
}





struct VOut
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 posView : VIEWPOS;
	float4x4 view : VIEW;
	float3x3 TBN : TBN;
};

VOut VSmain(float4 position : POSITION, float4 color : COLOR, float4 normal : NORMAL, float4 tangent : TANGENT, float4 bitangent : BITANGENT, float2 texcoord : TEXCOORD)
{
	VOut output;

	output.position = mul(position, worldMatrix);

    output.position = mul(output.position, viewMatrix);
	output.posView = output.position; // lights in view space

	output.position = mul(output.position, projectionMatrix);


	// normal mapping
	normal = mul(normal, worldMatrix);
	normal = mul(normal, viewMatrix);

	bitangent = mul(bitangent, worldMatrix);
	bitangent = mul(bitangent, viewMatrix);

	tangent = mul(tangent, worldMatrix);
	tangent = mul(tangent, viewMatrix);

	normal = normalize(normal);
    tangent = normalize(tangent);
    bitangent = normalize(bitangent); //normalize(bitangent);

	
	output.TBN = float3x3(tangent.xyz, bitangent.xyz, normal.xyz);
	output.view = viewMatrix;
	output.texcoord = texcoord;

	return output;
}




Texture2D tex : register(t0);
Texture2D norm : register(t1);

SamplerState ss : register(s0);


float3 getNormal(float3x3 TBN, float2 texcoord)
{
	float3 normal = float3(norm.Sample(ss, texcoord).rgb);
	normal = normalize(normal * 2.0 - 1.0);
	normal = mul(normal, TBN);
	normal = normalize(normal);

	return normal;
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
	output.normal = float4(getNormal(input.TBN, input.texcoord), 0.f);
	output.albedo = float4(tex.Sample(ss, input.texcoord));
    output.specular = mtrspecular;
	output.position = input.posView;
	return output;
}

