// Vertex
cbuffer CameraBuffer : register(b0)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

cbuffer ModelBuffer : register(b1)
{
	float4x4 worldMatrix;
};

// Pixel


struct Light
{
    float4 position;
    float4 colour;
    float4 attrib; // x = diffuse y = constant z = linear w = quadratic
};


cbuffer dirBuffer : register(b0)
{
    Light dirLights[256];
    int nrOfDir;
};

cbuffer spotbuffer : register(b1)
{
    Light spotLights[256];
    int nrOfSpot;
};

cbuffer pointBuffer : register(b2)
{
    Light pointLights[256];
    int nrOfPoint;
};


cbuffer cameraBuffer : register(b3)
{
    float4 cameraPos;
}



struct VOut
{
	float4x4 viewMatrix : VIEW;
	float4 position : SV_POSITION;
};

VOut VSmain(float4 position : POSITION, float4 color : COLOR, float4 normal : NORMAL, float4 tangent : TANGENT, float4 bitangent : BITANGENT, float2 texcoord : TEXCOORD)
{

	VOut output;
	output.viewMatrix = viewMatrix;
	output.position = float4(position.xyz, 1.0f);
	return output;
}


Texture2D NormalTexture : register(t0);
Texture2D AlbedoTexture : register(t1);
Texture2D SpecularTexture : register(t2);
Texture2D PositionTexture : register(t3);

float4 calcPointLight(float4 normal, float4 position, Light light, float4x4 view, float4 albedo, float4 specularMtr)
{
	// ambient
    float4 ambient = light.attrib.x * albedo;

	// diffuse
    float4 lightDir = mul(light.position, view);
	lightDir = normalize(lightDir - position);


	float gamma = 2.2f;
	
	float4 colour = float4(pow(albedo.x, gamma), pow(albedo.y, gamma), pow(albedo.z, gamma), 1.0f);

    float diff = max(dot(normal, lightDir), 0.0f);
    float4 diffuse = light.colour * diff * albedo;

	// speuclar
	float4 viewDir = normalize(-float4(position.xyz, 0.0f));
	float4 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(halfwayDir, viewDir), 0.0f), 32.0f);
    float4 specular = spec * light.colour * specularMtr.x;

	float distance = length(mul(light.position, view) - position);

	float attenuation = 1.0 / (light.attrib.y + light.attrib.z * distance +
		light.attrib.w * (distance * distance));
 
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (diffuse + specular);
}

float4 PSmain(VOut input) : SV_TARGET
{
	float gamma = 2.22f;

	float4 albedo = AlbedoTexture.Load(float3(input.position.xy, 0), 0);
	float4 normal = NormalTexture.Load(float3(input.position.xy, 0), 0);
	float4 specular = SpecularTexture.Load(float3(input.position.xy, 0), 0);
	float4 position = PositionTexture.Load(float3(input.position.xy, 0), 0);
  
    float4 result = float4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < nrOfPoint; i++)
         result += calcPointLight(normal, position, pointLights[i], input.viewMatrix, albedo, specular);
	// gamma correction
	return float4(pow(result.x, gamma), pow(result.y, gamma), pow(result.z, gamma), 1.0f);
    
}

