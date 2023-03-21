//----------------------------------------------------------------------
//Texture
//----------------------------------------------------------------------
Texture2D DiffuseMap;
Texture2D SpecularMap;
Texture2D NormalMap;

//----------------------------------------------------------------------
//Global Light
//----------------------------------------------------------------------
struct LightDesc
{
	float4 Ambient;
	float4 Specular;
	float3 Direction;
	float Padding;

	float3 Position;
};

cbuffer CB_Light
{
	LightDesc GlobalLight;
}

//----------------------------------------------------------------------
//Material
//----------------------------------------------------------------------
struct MaterialDesc
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emissive;
};

cbuffer CB_Material
{
	MaterialDesc Material;
}

//----------------------------------------------------------------------
//Functions
//----------------------------------------------------------------------
void Texture(inout float4 color, Texture2D t, float2 uv, SamplerState samp)
{
	float4 sampling = t.Sample(samp, uv);
	
	[flatten]
	if (any(sampling.rgb))
		color = color * sampling;
}

void Texture(inout float4 color, Texture2D t, float2 uv)
{
	Texture(color, t, uv, LinearSampler);
}

float3 MaterialToColor(MaterialDesc result)
{
	return float4(result.Ambient + result.Diffuse + result.Specular + result.Emissive).rgb;
}

//----------------------------------------------------------------------
//Lighting
//----------------------------------------------------------------------
void ComputeLight(out MaterialDesc output, float3 normal, float3 wPosition)
{
	output = (MaterialDesc) 0;
	
	output.Ambient = GlobalLight.Ambient * Material.Ambient;

	float3 direction = -GlobalLight.Direction;
	float lambert = dot(normalize(normal), direction);

	//Diffuse
	output.Diffuse = Material.Diffuse * lambert;

	float3 E = normalize(ViewPosition() - wPosition);
	
	//Specular
	if (Material.Specular.a > 0.f)
	{
		float3 R = normalize(reflect(GlobalLight.Direction, normal));
		float RdotE = saturate(dot(R, E));

		float specular = pow(RdotE, Material.Specular.a);
		output.Specular = GlobalLight.Specular * Material.Specular * specular;
	}
	
	//Emissive
	if (Material.Emissive.a > 0.f)
	{
		float NdotE = dot(normalize(normal), E);

		float emissive = smoothstep(1 - Material.Emissive.a, 1.f, 1 - NdotE);
		output.Emissive = Material.Emissive * emissive;
	}
	
}

void NormalMapping(float2 uv, float3 normal, float3 tangent, SamplerState samp)
{
	float3 map = NormalMap.Sample(samp, uv).rgb;

	[flatten]
	if (any(map.rgb) == false)
		return;
	
	float3 coord = map.rgb * 2.f - 1.f;

	float3 N = normalize(normal);
	float3 T = normalize(tangent - dot(tangent, N) * N);
	float3 B = cross(N, T);
	
	float3x3 TBN = float3x3(T, B, N);
	coord = mul(coord, TBN);

	Material.Diffuse *= saturate(dot(coord, -GlobalLight.Direction));
}

void NormalMapping(float2 uv, float3 normal, float3 tangent)
{
	NormalMapping(uv, normal, tangent, LinearSampler);
}