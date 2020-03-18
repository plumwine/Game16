#pragma once
#include "DirectX11.h"
#include "Camera.h"
#include "Singleton.h"
#include "Math/Vector3.h"
#include <unordered_map>
#include <vector>
#include <d3dcompiler.h>
#include <d3d11shader.h>

//2D�`����
struct DrawInfomation2D
{
	std::string textureName;    //�e�N�X�`����
	Vector3 position;			//�ʒu
	Vector3 angles;				//��]
	Vector3 scale;				//�g��k��
	float uvX;					//uv�֌W
	float uvY;					//
	float uvWidth;				//
	float uvHeight;				//

};



//�����_���[2D�̃V���O���g��
class Render2D :public Singleton<Render2D>
{
protected:
	Render2D();
	virtual ~Render2D();

public:
	friend class Singleton<Render2D>;               //Singleton�ł̃C���X�^���X�쐬�͋���

	//������
	void init();                               //�����ݒ肵�Ȃ����init�����ł���
	void Draw();                               //�`��

	//2D�iUI�֌W�́j�`�� �g�p�����
	void drawTexture2D(std::string textureName, Vector3 position, Vector3 angle, Vector3 scale, UINT layer = 0);
	
	void drawTexture2D(
		std::string textureName, Vector3 position, Vector3 angles, Vector3 scale,
		float uvX, float uvY, float uvWidth, float uvHeight, UINT layer = 0);

	void drawManager();
	//������
	void draw2D(std::string textureName,Vector3 position, Vector3 angle,Vector3 scale, float uvX, float uvY, float uvWidth, float uvHeight);


	//�o�b�N�o�b�t�@�[�̃����_�[�^�[�Q�b�g�r���[(RTV)���쐬
	void createRTV();
	//�f�v�X�X�e���V���r���[(DSV)���쐬    
	void createDSV();
	//�V�F�[�_�[�ɕԂ��p�̃R���X�^���ƃo�b�t�@�[
	void createConstantBuffer();

	//�V�F�[�_�[���̎擾
	ID3D11VertexShader* GetVertexShader(std::string vsName);
	ID3D11PixelShader* GetPixelShader(std::string psName);


	ID3DBlob** ppGetBlob(LPSTR blobName);
	ID3DBlob* GetBlob(LPSTR blobName);
	void AddBlob(LPSTR blobName);
	ID3D11VertexShader* GetVertexShader(LPSTR name);
	ID3D11PixelShader* GetPixelShader(LPSTR name);

	HRESULT MakeShader(const std::string shaderName, const char* szProfileName,const void * pShaderByteCode,SIZE_T byteCodeLength);


	HRESULT LoadVertexShader(
		ID3D11Device * pDevice,
		const std::string key,
		const void * pShaderByteCode,
		SIZE_T byteCodeLength);
	
	
	HRESULT LoadPixelShader(
		ID3D11Device * pDevice,
		const std::string key,
		const void * pShaderByteCode,
		SIZE_T byteCodeLength);

	HRESULT MakeVertex(LPSTR name, ID3DBlob** ppBlob);
	HRESULT MakePixel(LPSTR name, ID3DBlob** ppBlob);

private:

	ID3D11Device* g_pDevice;               // DirectX11�̃f�o�C�X
	ID3D11DeviceContext* g_pDeviceContext; // �`��p�̃f�o�C�X�R���e�L�X�g����1���g���܂킷�B��j1�ڂ̏���`��A2�ڂ�`�悷��ۂɂ܂�������Ȃ�����2�ڂ��`�悳���
	IDXGISwapChain* g_pSwapChain;		   // ��ʏo�͗p�̃X���b�v�`�F�C���@������1�����ł���

	ID3D11SamplerState* g_pSampleLinear;
	ID3D11RenderTargetView* g_pRTV;
	ID3D11Texture2D* g_pDS;
	ID3D11DepthStencilView* g_pDSV;
	ID3D11InputLayout* g_pVertexLayout;
	ID3D11BlendState* g_pBlendState;

	//�V�F�[�_�[�n
	ID3D11Buffer* g_pConstantBuffer;          //�V�F�[�_�[�󂯓n���p�̒萔�ꎞ�ۑ�
	ID3D11Buffer* g_pVertexBuffer;            //���_���

	//�V�F�[�_�[�Ǘ��p
	//std::unordered_map<std::string, ID3D11VertexShader*>    m_VertexShaderMap;      //���_�V�F�[�_�[�Ǘ��p
	//std::unordered_map<std::string, ID3D11PixelShader*>     m_PixelShaderMap;       //�s�N�Z���V�F�[�_�[�Ǘ��p		
	

	//���C���[�֌W
	int m_MaxLayer;
	std::unordered_map<UINT, std::vector<DrawInfomation2D>> mLayerDrawMap;
};

