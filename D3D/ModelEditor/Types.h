#pragma once

#include "stdafx.h"

//-----------------------------------------------------------------
//Bone & Mesh
//-----------------------------------------------------------------
struct asBone
{
	int Index;
	string Name;

	int Parent;
	Matrix Transform;
};

struct asMeshPart
{
	string MaterialName;

	UINT StartVertex;
	UINT VertexCount;
	UINT StartIndex;
	UINT IndexCount;
};

struct asMesh
{
	int BoneIndex;

	vector<Model::VertexModel> Vertices;
	vector<UINT>Indices;

	vector<asMeshPart*> MeshParts;
};

//-----------------------------------------------------------------
//Material
//-----------------------------------------------------------------
struct asMaterial
{
	string Name;

	Color Ambient;
	Color Diffuse;
	Color Specular;
	Color Emissive;

	string DiffuseFile;
	string SpecularFile;
	string NormalFile;
};

//-----------------------------------------------------------------
//Skinning
//-----------------------------------------------------------------
struct asBlendWeigt
{
	Vector4 Indices = Vector4(0, 0, 0, 0);
	Vector4 Weights = Vector4(0, 0, 0, 0);

	void Set(UINT index, UINT boneIndex, float boneWeight)
	{
		float i = (float)boneIndex;
		float w = boneWeight;

		switch (index)
		{
			case 0:	Indices.x = i; Weights.x = w; break;
			case 1:	Indices.y = i; Weights.y = w; break;
			case 2:	Indices.z = i; Weights.z = w; break;
			case 3:	Indices.w = i; Weights.w = w; break;
		}
	}
};

struct asBoneWeights
{
private:
	typedef pair<int, float> Pair;
	vector<Pair> BoneWeights;

public:
	void AddWeights(UINT boneIndex, float boneWeights)
	{
		if (boneWeights <= 0.f) return;
		
		bool bAdd = false;

		vector<Pair>::iterator it = BoneWeights.begin();
		while (it != BoneWeights.end())
		{
			if (boneWeights > it->second)
			{
				BoneWeights.insert(it, Pair(boneIndex, boneWeights));
				bAdd = true;

				break;
			}

			it++;
		}

		if (bAdd == false)
			BoneWeights.push_back(Pair(boneIndex, boneWeights));
	}

	void GetBlendWeigts(asBlendWeigt& blendWeights)
	{
		for (UINT i = 0; i < BoneWeights.size(); i++)
		{
			if (i >= 4) return;

			blendWeights.Set(i, BoneWeights[i].first, BoneWeights[i].second);
		}
	}

	void Normalize()
	{
		float totalWeight = 0.f;

		int i = 0;

		vector<Pair>::iterator it = BoneWeights.begin();
		while (it != BoneWeights.end())
		{
			if (i < 4)
			{
				totalWeight += it->second;
				i++; it++;
			}
			else
				it = BoneWeights.erase(it);
		}

		float scale = 1.f / totalWeight;

		it = BoneWeights.begin();
		while (it != BoneWeights.end())
		{
			it->second *= scale;
			it++;
		}
	}
};


//-----------------------------------------------------------------
//KeyFrame
//-----------------------------------------------------------------

//1 Bone, 1 Frame - SRT
struct asKeyFrameData
{
	float Frame;

	Vector3 Scale;
	Quaternion Rotation;
	Vector3 Translation;
};

//1 Bone, All Frames
struct asKeyFrame
{
	string BoneName;
	vector<asKeyFrameData> Trnasforms;
};

//All Bones, All Frames -> (*.clip Result)
struct asClip
{
	string Name;

	UINT FrameCount;
	float FrameRate;

	vector<asKeyFrame*> KeyFrames;
};

//Temp Data to asClip
struct asClipNode
{
	aiString Name;
	vector<asKeyFrameData> Keyframe;
};