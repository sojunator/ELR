struct VOut
{
	float4 position : SV_POSITION;
	
};
  
struct GOut
{
	float4 position : SV_POSITION;
	
	uint viewportIndex : SV_ViewportArrayIndex;
};

cbuffer lightBuffer : register(b0)
{
	float4x4 viewMatrix;
	float4 viewport;
};

cbuffer ModelBuffer : register(b1)
{
	float4x4 worldMatrix;
};

cbuffer projection : register(b2)
{
	float4x4 projectionMatrix;
}

VOut VSmain(float4 position : POSITION, float4 color : COLOR, float4 normal : NORMAL, float4 tangent : TANGENT, float4 bitangent : BITANGENT, float2 texcoord : TEXCOORD)
{
	VOut output;
	
	output.position = mul(mul(mul(position, worldMatrix), viewMatrix), projectionMatrix);
 
	
	return output;
}


