#include "Texture2D.h"
#include "Camera.h"
//#include"WICTextureLoader.h"
//#include"WICTextureLoader.cpp"
Texture2D::Texture2D()
{
}


Texture2D::~Texture2D()
{
}

void Texture2D::losdTecture(LPSTR filePath, std::string textureName)
{
	ID3D11Resource* pTexture;
	ID3D11ShaderResourceView* pTextureView;

	//テクスチャー作成
	D3DX11CreateShaderResourceViewFromFile(DirectX11::getInstance().GetDevice(),filePath, NULL, NULL, &pTextureView, NULL);
	//HRESULT result = 
	//CreateWICTextureFromFile(DirectX11::getInstance().GetDevice(), filePath, &pTexture, &pTextureView);

	textureMap.emplace(textureName, pTexture);
	textureViewMap.emplace(textureName, pTextureView);

	//if (FAILED(result)) return E_FAIL;

	//return S_OK;
}

ID3D11Resource * Texture2D::getTexture(std::string textureName)
{
	return textureMap.at(textureName);
}

ID3D11ShaderResourceView * Texture2D::getTextureView(std::string textureName)
{
	return textureViewMap.at(textureName);
}
