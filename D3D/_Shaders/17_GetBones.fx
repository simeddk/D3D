#include "00_Global.fx"

struct BoneDesc
{
	matrix Transform;
};
StructuredBuffer<BoneDesc> InputBones;
RWStructuredBuffer<BoneDesc> OutputBones;

#define MAX_MODEL_TRANSFORMS 250

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

Texture2DArray TransformsMap;

void SetAnimationWorld(inout matrix world, uint3 id)
{
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
	
	
	c0 = TransformsMap.Load(int4(id.x * 4 + 0, currFrame[0], clip[0], 0));
	c1 = TransformsMap.Load(int4(id.x * 4 + 1, currFrame[0], clip[0], 0));
	c2 = TransformsMap.Load(int4(id.x * 4 + 2, currFrame[0], clip[0], 0));
	c3 = TransformsMap.Load(int4(id.x * 4 + 3, currFrame[0], clip[0], 0));
	curr = matrix(c0, c1, c2, c3);
		
	n0 = TransformsMap.Load(int4(id.x * 4 + 0, nextFrame[0], clip[0], 0));
	n1 = TransformsMap.Load(int4(id.x * 4 + 1, nextFrame[0], clip[0], 0));
	n2 = TransformsMap.Load(int4(id.x * 4 + 2, nextFrame[0], clip[0], 0));
	n3 = TransformsMap.Load(int4(id.x * 4 + 3, nextFrame[0], clip[0], 0));
	next = matrix(n0, n1, n2, n3);
		
	currAnim = lerp(curr, next, time[0]);
		
	[flatten]
	if (clip[1] > -1)
	{
		c0 = TransformsMap.Load(int4(id.x * 4 + 0, currFrame[1], clip[1], 0));
		c1 = TransformsMap.Load(int4(id.x * 4 + 1, currFrame[1], clip[1], 0));
		c2 = TransformsMap.Load(int4(id.x * 4 + 2, currFrame[1], clip[1], 0));
		c3 = TransformsMap.Load(int4(id.x * 4 + 3, currFrame[1], clip[1], 0));
		curr = matrix(c0, c1, c2, c3);
		
		n0 = TransformsMap.Load(int4(id.x * 4 + 0, nextFrame[1], clip[1], 0));
		n1 = TransformsMap.Load(int4(id.x * 4 + 1, nextFrame[1], clip[1], 0));
		n2 = TransformsMap.Load(int4(id.x * 4 + 2, nextFrame[1], clip[1], 0));
		n3 = TransformsMap.Load(int4(id.x * 4 + 3, nextFrame[1], clip[1], 0));
		next = matrix(n0, n1, n2, n3);
		
		nextAnim = lerp(curr, next, time[1]);
			
		currAnim = lerp(currAnim, nextAnim, TweenFrames.TweenTime);
	}
	
	world = mul(currAnim, world);
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

void SetBledingWorld(inout matrix world, uint3 id)
{
	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;
	
	matrix transform = 0;
	matrix curr = 0, next = 0;
	matrix currAnim[3];
	matrix anim = 0;
	
	[unroll(3)]
	for (int k = 0; k < 3; k++)
	{
		c0 = TransformsMap.Load(int4(id.x * 4 + 0, BlendFrames.Clip[k].CurrFrame, BlendFrames.Clip[k].Clip, 0));
		c1 = TransformsMap.Load(int4(id.x * 4 + 1, BlendFrames.Clip[k].CurrFrame, BlendFrames.Clip[k].Clip, 0));
		c2 = TransformsMap.Load(int4(id.x * 4 + 2, BlendFrames.Clip[k].CurrFrame, BlendFrames.Clip[k].Clip, 0));
		c3 = TransformsMap.Load(int4(id.x * 4 + 3, BlendFrames.Clip[k].CurrFrame, BlendFrames.Clip[k].Clip, 0));
		curr = matrix(c0, c1, c2, c3);
		
		n0 = TransformsMap.Load(int4(id.x * 4 + 0, BlendFrames.Clip[k].NextFrame, BlendFrames.Clip[k].Clip, 0));
		n1 = TransformsMap.Load(int4(id.x * 4 + 1, BlendFrames.Clip[k].NextFrame, BlendFrames.Clip[k].Clip, 0));
		n2 = TransformsMap.Load(int4(id.x * 4 + 2, BlendFrames.Clip[k].NextFrame, BlendFrames.Clip[k].Clip, 0));
		n3 = TransformsMap.Load(int4(id.x * 4 + 3, BlendFrames.Clip[k].NextFrame, BlendFrames.Clip[k].Clip, 0));
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
	
	world = mul(anim, world);
}

[numthreads(MAX_MODEL_TRANSFORMS, 1, 1)]
void CS(uint3 id : SV_GroupThreadID)
{
	Matrix world = World;
	
	if (BlendFrames.Mode == 0)
		SetAnimationWorld(world, id);
	else
		SetBledingWorld(world, id);

	world = mul(InputBones[id.x].Transform, world);
	
	OutputBones[id.x].Transform = world;
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);

		SetComputeShader(CompileShader(cs_5_0, CS()));
	}
}