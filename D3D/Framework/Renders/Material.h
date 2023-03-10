#pragma once

class Material
{
public:
	Material();
	Material(Shader* shader);
	~Material();

private:
	void Initialize();

public:
	Shader* GetShader() { return shader; }
	void SetShader(Shader* shader);

	void Name(wstring val) { name = val; }
	wstring Name() { return name; }

	Color& Ambient() { return colorDesc.Ambient; }
	void Ambient(Color& color);
	void Ambient(float r, float g, float b, float a = 1.f);

	Color& Diffuse() { return colorDesc.Diffuse; }
	void Diffuse(Color& color);
	void Diffuse(float r, float g, float b, float a = 1.f);

	Color& Specular() { return colorDesc.Specular; }
	void Specular(Color& color);
	void Specular(float r, float g, float b, float a = 1.f);

	Color& Emissive() { return colorDesc.Emissive; }
	void Emissive(Color& color);
	void Emissive(float r, float g, float b, float a = 1.f);

	Texture* DiffuseMap() { return diffuseMap; }
	void DiffuseMap(string file);
	void DiffuseMap(wstring file);

	Texture* SpecularMap() { return specularMap; }
	void SpecularMap(string file);
	void SpecularMap(wstring file);

	Texture* NormalMap() { return normalMap; }
	void NormalMap(string file);
	void NormalMap(wstring file);

	void Render();

private:
	struct ColorDesc
	{
		Color Ambient;
		Color Diffuse;
		Color Specular;
		Color Emissive;
	} colorDesc;

private:
	Shader* shader;

	wstring name;

	Texture* diffuseMap;
	Texture* specularMap;
	Texture* normalMap;

	ID3DX11EffectShaderResourceVariable* sDiffuseMap;
	ID3DX11EffectShaderResourceVariable* sSpecularMap;
	ID3DX11EffectShaderResourceVariable* sNormalMap;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;
};