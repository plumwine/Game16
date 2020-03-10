#pragma once

#include "DirectX11.h"
#include "Singleton.h"
#include <string>
#include <unordered_map>

//�e�N�X�`���ǂݍ���
class Texture2D :public Singleton<Texture2D>
{
protected:
	Texture2D();
	virtual ~Texture2D();
public:
	friend class Singleton<Texture2D>;  //Singleton�ł̃C���X�^���X�쐬�͋���
	

	void losdTecture(LPSTR filePath, std::string textureName);   //�摜�̒ǉ�

	ID3D11Resource * getTexture(std::string textureName);                  //�摜���
	ID3D11ShaderResourceView * getTextureView(std::string textureName);//�摜�r���[���

	std::unordered_map<std::string, ID3D11Resource*> textureMap;                //�e�N�X�`�����
	std::unordered_map<std::string, ID3D11ShaderResourceView*> textureViewMap;
};

