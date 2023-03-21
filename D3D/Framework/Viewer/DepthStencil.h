#pragma once

class DepthStencil
{
public:
	DepthStencil(float width = 0, float height = 0, bool bUseStencil = false);
	~DepthStencil();

	ID3D11Texture2D* Texture() { return texture; }
	ID3D11DepthStencilView* DSV() { return dsv; }
	ID3D11ShaderResourceView* SRV() { return srv; }

private:
	float width, height;

	ID3D11Texture2D* texture;
	ID3D11DepthStencilView* dsv;
	ID3D11ShaderResourceView* srv;
};
