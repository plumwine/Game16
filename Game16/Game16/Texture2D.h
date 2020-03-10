#pragma once

#include "DirectX11.h"
#include "Singleton.h"
#include <string>
#include <unordered_map>

//テクスチャ読み込み
class Texture2D :public Singleton<Texture2D>
{
protected:
	Texture2D();
	virtual ~Texture2D();
public:
	friend class Singleton<Texture2D>;  //Singletonでのインスタンス作成は許可
	

	void losdTecture(LPSTR filePath, std::string textureName);   //画像の追加

	ID3D11Resource * getTexture(std::string textureName);                  //画像情報
	ID3D11ShaderResourceView * getTextureView(std::string textureName);//画像ビュー情報

	std::unordered_map<std::string, ID3D11Resource*> textureMap;                //テクスチャ情報
	std::unordered_map<std::string, ID3D11ShaderResourceView*> textureViewMap;
};

