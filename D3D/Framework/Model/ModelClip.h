#pragma once

//1 Bone, 1 Frame - SRT
struct ModelKeyFrameData
{
	float Frame;

	Vector3 Scale;
	Quaternion Rotation;
	Vector3 Translation;
};

//1 Bone, All Frames
struct ModelKeyFrame
{
	wstring BoneName;
	vector<ModelKeyFrameData> Transforms;
};

//All Bones, All Frames
class ModelClip
{
public:
	friend class Model;

private:
	ModelClip();
	~ModelClip();

public:
	UINT FrameCount() { return frameCount; }
	float FrameRate() { return frameRate; }

	ModelKeyFrame* Keyframe(wstring name);

private:
	wstring name;

	UINT frameCount;
	float frameRate;

	unordered_map<wstring, ModelKeyFrame*> keyFrameMap;
};