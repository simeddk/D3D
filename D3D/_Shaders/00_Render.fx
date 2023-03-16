//-----------------------------------------------------------------------------
//VS_GENERATE Macro
//-----------------------------------------------------------------------------
#define VS_GENERATE \
output.oPosition = input.Position.xyz; \
\
output.Position = WorldPosition(input.Position); \
output.wPosition = output.Position.xyz; \
\
output.Position = ViewProjection(output.Position); \
\
output.Normal = WorldNormal(input.Normal); \
output.Uv = input.Uv; \
output.Color = input.Color;

//-----------------------------------------------------------------------------
//Mesh
//-----------------------------------------------------------------------------
struct VertexMesh
{
	float4 Position : Position;
	float2 Uv : Uv;
	float3 Normal : Normal;
	
	matrix Transform : Inst1_Transform;
	float4 Color : Inst2_Color;
};

void SetMeshWorld(inout matrix world, VertexMesh input)
{
	world = input.Transform;
}

MeshOutput VS_Mesh(VertexMesh input)
{
	MeshOutput output;
	
	SetMeshWorld(World, input);
	VS_GENERATE
	
	return output;
}

//-----------------------------------------------------------------------------
//Model
//-----------------------------------------------------------------------------
Texture2DArray TransformsMap;

struct VertexModel
{
	float4 Position : Position;
	float2 Uv : Uv;
	float3 Normal : Normal;
	float3 Tangent : Tangent;
	float4 BlendIndices : BlendIndices;
	float4 BlendWeights : BlendWeights;
	
	matrix Transform : Inst1_Transform;
	float4 Color : Inst2_Color;
	
	uint InstanceID : SV_InstanceID;
};

#define MAX_MODEL_TRANSFORMS 250
cbuffer CB_Bones
{
	Matrix Transforms[MAX_MODEL_TRANSFORMS];
	uint BoneIndex;
}

MeshOutput VS_Model(VertexModel input)
{
	MeshOutput output;
	
	World = mul(Transforms[BoneIndex], World);
	VS_GENERATE
	
	return output;
}

//-----------------------------------------------------------------------------
//Animation
//-----------------------------------------------------------------------------
#define MAX_MODEL_KEYFRAMES 500

struct AnimationFrame
{
	int Clip;

	uint CurrFrame;
	uint NextFrame;

	float Time;
	float RunningTime;

	float Speed;
};

struct TweenFrame
{
	float TakeTime;
	float TweenTime;
	float ChangeTime;
	float Padding;

	AnimationFrame Curr;
	AnimationFrame Next;
};

cbuffer CB_AnimationFrame
{
	TweenFrame TweenFrames;
}

void SetAnimationWorld(inout matrix world, VertexModel input)
{
	float indices[4] = { input.BlendIndices.x, input.BlendIndices.y, input.BlendIndices.z, input.BlendIndices.w };
	float weights[4] = { input.BlendWeights.x, input.BlendWeights.y, input.BlendWeights.z, input.BlendWeights.w };
	
	int clip[2];
	int currFrame[2], nextFrame[2];
	float time[2];
	
	clip[0] = TweenFrames.Curr.Clip;
	currFrame[0] = TweenFrames.Curr.CurrFrame;
	nextFrame[0] = TweenFrames.Curr.NextFrame;
	time[0] = TweenFrames.Curr.Time;
	
	clip[1] = TweenFrames.Next.Clip;
	currFrame[1] = TweenFrames.Next.CurrFrame;
	nextFrame[1] = TweenFrames.Next.NextFrame;
	time[1] = TweenFrames.Next.Time;
	
	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;
	
	matrix transform = 0;
	matrix curr = 0, next = 0;
	matrix currAnim = 0, nextAnim = 0;
	
	[unroll(4)]
	for (int i = 0; i < 4; i++)
	{
		c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, currFrame[0], clip[0], 0));
		c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, currFrame[0], clip[0], 0));
		c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, currFrame[0], clip[0], 0));
		c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, currFrame[0], clip[0], 0));
		curr = matrix(c0, c1, c2, c3);
		
		n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], clip[0], 0));
		n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], clip[0], 0));
		n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], clip[0], 0));
		n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], clip[0], 0));
		next = matrix(n0, n1, n2, n3);
		
		currAnim = lerp(curr, next, time[0]);
		
		[flatten]
		if (clip[1] > -1)
		{
			c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, currFrame[1], clip[1], 0));
			c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, currFrame[1], clip[1], 0));
			c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, currFrame[1], clip[1], 0));
			c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, currFrame[1], clip[1], 0));
			curr = matrix(c0, c1, c2, c3);
		
			n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], clip[1], 0));
			n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], clip[1], 0));
			n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], clip[1], 0));
			n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], clip[1], 0));
			next = matrix(n0, n1, n2, n3);
		
			nextAnim = lerp(curr, next, time[1]);
			
			currAnim = lerp(currAnim, nextAnim, TweenFrames.TweenTime);
		}

		transform += mul(weights[i], currAnim);
	}
	
	world = mul(transform, world);
}

struct BlendFrame
{
	uint Mode;
	float Alpha;
	float2 Padding;

	AnimationFrame Clip[3];
};

cbuffer CB_BlendingFrame
{
	BlendFrame BlendFrames;
}

void SetBledingWorld(inout matrix world, VertexModel input)
{
	float indices[4] = { input.BlendIndices.x, input.BlendIndices.y, input.BlendIndices.z, input.BlendIndices.w };
	float weights[4] = { input.BlendWeights.x, input.BlendWeights.y, input.BlendWeights.z, input.BlendWeights.w };
	
	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;
	
	matrix transform = 0;
	matrix curr = 0, next = 0;
	matrix currAnim[3];
	matrix anim = 0;
	
	[unroll(4)]
	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, BlendFrames.Clip[k].CurrFrame, BlendFrames.Clip[k].Clip, 0));
			c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, BlendFrames.Clip[k].CurrFrame, BlendFrames.Clip[k].Clip, 0));
			c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, BlendFrames.Clip[k].CurrFrame, BlendFrames.Clip[k].Clip, 0));
			c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, BlendFrames.Clip[k].CurrFrame, BlendFrames.Clip[k].Clip, 0));
			curr = matrix(c0, c1, c2, c3);
		
			n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, BlendFrames.Clip[k].NextFrame, BlendFrames.Clip[k].Clip, 0));
			n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, BlendFrames.Clip[k].NextFrame, BlendFrames.Clip[k].Clip, 0));
			n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, BlendFrames.Clip[k].NextFrame, BlendFrames.Clip[k].Clip, 0));
			n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, BlendFrames.Clip[k].NextFrame, BlendFrames.Clip[k].Clip, 0));
			next = matrix(n0, n1, n2, n3);
		
			currAnim[k] = lerp(curr, next, BlendFrames.Clip[k].Time);
		}
		
		float alpha = BlendFrames.Alpha;
		int clipIndex[2] = { 0, 1 };
		
		if (alpha > 1)
		{
			clipIndex[0] = 1;
			clipIndex[1] = 2;
			
			alpha -= 1.f;
		}
			
		anim = lerp(currAnim[clipIndex[0]], currAnim[clipIndex[1]], alpha);
		
		transform += mul(weights[i], anim);
	}
	
	world = mul(transform, world);
}

MeshOutput VS_Animation(VertexModel input)
{
	MeshOutput output;

	if (BlendFrames.Mode == 0)
		SetAnimationWorld(World, input);
	else
		SetBledingWorld(World, input);

	VS_GENERATE

	return output;
}