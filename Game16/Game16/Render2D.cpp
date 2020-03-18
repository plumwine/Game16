#include "Render2D.h"
#include "Window.h"
//#include "vs_tex.h"
//#include "ps_tex.h"
#include "Math/Color4.h"
#include "Math/Vector2.h"
#include "Texture2D.h"
#include "vs_test.h"
#include "ps_test.h"

#include <d3dx10.h>
#pragma comment (lib,"d3dx10.lib")


//���_�̍\����
struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos; //�ʒu
	DirectX::XMFLOAT2 TexCoord;
};
//�V�F�[�_�[�p�̒萔�o�b�t�@��App���\����
struct SHADER_CONSTANT_BUFFER
{
	//WorldViewProjection
	XMMATRIX mWVP;
	Vector2 mUVOffset;
	Vector2 mUVSize;
};
 std::unordered_map<std::string, ID3D11VertexShader*>    m_VertexShaderMap;      //���_�V�F�[�_�[�Ǘ��p
 std::unordered_map<std::string, ID3D11PixelShader*>     m_PixelShaderMap;       //�s�N�Z���V�F�[�_�[�Ǘ��p	
 std::unordered_map<LPSTR, ID3D11VertexShader*>          l_VertexShaderMap;      //���_�V�F�[�_�[�Ǘ��p
 std::unordered_map<LPSTR, ID3D11PixelShader*>           l_PixelShaderMap;       //�s�N�Z���V�F�[�_�[�Ǘ��p	
 std::unordered_map<LPSTR, ID3DBlob*>                    m_BlobMap;       


Render2D::Render2D()
	:m_MaxLayer(10)
{
}

Render2D::~Render2D()
{
}

void Render2D::init()
{
	//DirectX11.h�ŏ������������������Ă݂�
	g_pDevice = DirectX11::getInstance().GetDevice();
	g_pDeviceContext = DirectX11::getInstance().GetContext();
	g_pSwapChain = DirectX11::getInstance().GetSwapChain();
	//MakeShader("SimpleVertexShader","vs", &g_vs_main, sizeof(g_vs_main));
	//MakeShader("SimpleVertexShader","ps", &g_ps_main, sizeof(g_ps_main));
	//LoadVertexShader(g_pDevice,"simpleShader", &g_vs_main, sizeof(g_vs_main));
	//LoadPixelShader(g_pDevice,"simpleShader", &g_ps_main, sizeof(g_ps_main));
	createRTV();
	createDSV();
	createConstantBuffer();
}
void Render2D::Draw()
{
	//�����_�[�^�[�Q�b�g�r���[�ƃf�v�X�X�e���V���r���[���Z�b�g
	g_pDeviceContext->OMSetRenderTargets(1, &g_pRTV, g_pDSV);
	//��ʃN���A
	float ClearColor[4] = { 0,0,0.5,1 };// �N���A�F�쐬�@��ʔw�i�@RGBA�̏�

	g_pDeviceContext->ClearRenderTargetView(g_pRTV, ClearColor);//�J���[�o�b�t�@�N���A
	g_pDeviceContext->ClearDepthStencilView(g_pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//�f�v�X�X�e���V���o�b�t�@�N���A

	//�X�̃��[�J�����W�iWorld�j
	D3DXMATRIX World;
	//�P�ʍs��ɂ���
	D3DXMatrixIdentity(&World);
	//�ʒu										
	D3DXMATRIX Tran;
	D3DXMatrixTranslation(&Tran, 0.0, 0.0, 0);
	//��]�E�g��E�ʒu�̏��ɂ�����			    
	World = Tran;

	
	//�g�p����V�F�[�_�[�̃Z�b�g
	g_pDeviceContext->VSSetShader(GetVertexShader("simpleShader"), NULL, 0);
	g_pDeviceContext->PSSetShader(GetPixelShader("simpleShader"), NULL, 0);

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	SHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(g_pDeviceContext->Map(g_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�A�J�����A�ˉe�s���n��
		//cb.mWVP.tramspose(cb.mWVP);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		g_pDeviceContext->Unmap(g_pConstantBuffer, 0);
	}

	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	g_pDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pDeviceContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	g_pDeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	g_pDeviceContext->IASetInputLayout(g_pVertexLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	g_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�v���~�e�B�u�������_�����O
	g_pDeviceContext->Draw(4, 0);

	g_pSwapChain->Present(0, 0);//��ʍX�V  ��ʍX�V�͂����ōs��
}

void Render2D::drawTexture2D(std::string textureName, Vector3 position, Vector3 angle, Vector3 scale, UINT layer)
{
	drawTexture2D(textureName, position, angle, scale , 0.0f, 0.0f, 1.0f, 1.0f, layer);
}

void Render2D::drawTexture2D(std::string textureName, Vector3 position, Vector3 angle, Vector3 scale, float uvX, float uvY, float uvWidth, float uvHeight, UINT layer)
{
	//�`���������
	DrawInfomation2D drawInfo;
	drawInfo.textureName = textureName;
	drawInfo.position = position;
	drawInfo.angles = angle;
	drawInfo.scale = scale;
	drawInfo.uvX = uvX;
	drawInfo.uvY = uvY;
	drawInfo.uvWidth = uvWidth;
	drawInfo.uvHeight = uvHeight;

	auto result = mLayerDrawMap.find(layer);
	if (result != mLayerDrawMap.end())
	{
		mLayerDrawMap.at(layer).emplace_back(drawInfo);
	}
	else
	{
		std::vector<DrawInfomation2D> drawList;
		drawList.emplace_back(drawInfo);
		mLayerDrawMap.emplace(layer, drawList);
	}

	if (layer > m_MaxLayer)
		m_MaxLayer = layer;

}

void Render2D::drawManager()
{
	//�����_�[�^�[�Q�b�g�r���[�ƃf�v�X�X�e���V���r���[���Z�b�g
	g_pDeviceContext->OMSetRenderTargets(1, &g_pRTV, g_pDSV);
	//��ʃN���A
	float ClearColor[4] = { 0,0,0.5,1 };// �N���A�F�쐬�@RGBA�̏�
	g_pDeviceContext->ClearRenderTargetView(g_pRTV, ClearColor);//�J���[�o�b�t�@�N���A
	g_pDeviceContext->ClearDepthStencilView(g_pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//�f�v�X�X�e���V���o�b�t�@�N���A

	g_pDeviceContext->VSSetShader(GetVertexShader((LPSTR)"simpleShader.hlsl"), NULL, 0);
	g_pDeviceContext->PSSetShader(GetPixelShader((LPSTR)"simpleShader.hlsl"), NULL, 0);

	for (int i = 0; i <= m_MaxLayer; i++)
	{
		if (mLayerDrawMap.find(i) == mLayerDrawMap.end())
			continue;

		for (auto drawInfo : mLayerDrawMap.at(i))
		{
			Vector3 pos  = Camera::getInstance().getViewEyePos();
			Vector3 drawPos = (drawInfo.position - Camera::getInstance().getViewEyePos());

			draw2D(drawInfo.textureName,
				drawPos,
				drawInfo.angles,
				drawInfo.scale,
				drawInfo.uvX,
				drawInfo.uvY,
				drawInfo.uvWidth,
				drawInfo.uvHeight
			);
		}
		mLayerDrawMap.at(i).clear();
	}

	mLayerDrawMap.clear();
	m_MaxLayer = 0;
}
//���� 2D
void Render2D::draw2D(std::string textureName, Vector3 position, Vector3 angle, Vector3 scale,
	float uvX, float uvY, float uvWidth, float uvHeight)
{
       //���W�ϊ�
	XMMATRIX translate_m = XMMatrixTranslationFromVector(position.toXMVector());
	
	//��]
	angle.x = XMConvertToRadians(angle.x);
	angle.y = XMConvertToRadians(angle.y);
	angle.z = XMConvertToRadians(angle.z);
	XMMATRIX rotate_m = XMMatrixRotationRollPitchYawFromVector(angle.toXMVector());
	//�g��E�k��
	Vector3 absSize(std::fabsf(scale.x), std::fabsf(scale.y), std::fabsf(scale.z));
	XMMATRIX scale_m = XMMatrixScalingFromVector(absSize.toXMVector());
	//World
	XMMATRIX world;
	//��]�E�g��E�ʒu�̏��ɂ�����	
	world = rotate_m * scale_m * translate_m;


	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	SHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(g_pDeviceContext->Map(g_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�A�J�����A�ˉe�s���n��
		cb.mWVP = Camera::getInstance().getWorldView_Pjojection(world);
		//D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		cb.mWVP  = XMMatrixTranspose(cb.mWVP);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		g_pDeviceContext->Unmap(g_pConstantBuffer, 0);
	}
	cb.mUVOffset.x = uvX;
	cb.mUVOffset.y = uvY;
	cb.mUVSize.x = uvWidth;
	cb.mUVSize.y = uvHeight;

	g_pDeviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	g_pDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pDeviceContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	//�e�N�X�`���̑I��
	ID3D11ShaderResourceView* texture = Texture2D::getInstance().getTextureView(textureName);
	g_pDeviceContext->PSSetShaderResources(0, 1, &texture);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	g_pDeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	g_pDeviceContext->IASetInputLayout(g_pVertexLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	g_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�v���~�e�B�u�������_�����O
	g_pDeviceContext->Draw(4, 0);
}
//�f�t�H���g�œ����
void Render2D::createRTV()
{
	//�o�b�N�o�b�t�@�[�̃����_�[�^�[�Q�b�g�r���[(RTV)���쐬
	ID3D11Texture2D *pBack = NULL;
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBack);
	g_pDevice->CreateRenderTargetView(pBack, NULL, &g_pRTV);
	pBack->Release();
	//�T���v���[�̍쐬
	D3D11_SAMPLER_DESC samDesc;
	ID3D11SamplerState* pSamplerState;
	ZeroMemory(&samDesc, sizeof(D3D11_SAMPLER_DESC));
	samDesc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	g_pDevice->CreateSamplerState(&samDesc, &pSamplerState);
	g_pDeviceContext->PSSetSamplers(0, 1, &pSamplerState);
}

//�f�t�H���g�œ����
void Render2D::createDSV()
{
	//�f�v�X�X�e���V���r���[(DSV)���쐬                                     
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = Window::getInstance().GetWidth();
	descDepth.Height = Window::getInstance().GetHeight();
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	g_pDevice->CreateTexture2D(&descDepth, NULL, &g_pDS);
	g_pDevice->CreateDepthStencilView(g_pDS, NULL, &g_pDSV);
	//�r���[�|�[�g�ݒ�������ł���
	g_pDeviceContext->RSSetViewports(1, &Camera::getInstance().getViewPort());
}

//�f�t�H���g�œ����
void Render2D::createConstantBuffer()
{
	AddBlob((LPSTR)"simpleShader.hlsl");
	//�o�[�e�b�N�X�V�F�[�_�[�쐬
	MakeVertex((LPSTR)"simpleShader.hlsl", ppGetBlob((LPSTR)"simpleShader.hlsl"));
	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	//g_pDevice->CreateInputLayout(layout, numElements, g_vs_main,sizeof(g_vs_main), &g_pVertexLayout);
	g_pDevice->CreateInputLayout(layout, numElements, GetBlob((LPSTR)"simpleShader.hlsl")->GetBufferPointer(), GetBlob((LPSTR)"simpleShader.hlsl")->GetBufferSize(), &g_pVertexLayout);
	
	MakePixel((LPSTR)"simpleShader.hlsl", ppGetBlob((LPSTR)"simpleShader.hlsl"));

	//�R���X�^���g�o�b�t�@�[�쐬               
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	g_pDevice->CreateBuffer(&cb, NULL, &g_pConstantBuffer);

	//�o�[�e�b�N�X�o�b�t�@�[�쐬   �l�p�`
	SimpleVertex vertices[] =
	{
		{ {-0.5,-0.5, 0}, {0.0f, 1.0f}},   //���_1 �F	
		{ {-0.5, 0.5, 0}, {0.0f, 0.0f}},   //���_2 �F
		{ {0.5,-0.5, 0 }, {1.0f, 1.0f}},   //���_3 �F
		{ {0.5, 0.5, 0 }, {1.0f, 0.0f}}    //���_4 �F
	};
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	g_pDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);

	// �A���t�@�u�����h�p�u�����h�X�e�[�g�쐬
	//png�t�@�C�����ɃA���t�@��񂪂���B�A���t�@�ɂ�蓧�߂���悤�w�肵�Ă���
	D3D11_BLEND_DESC bld;
	ZeroMemory(&bld, sizeof(D3D11_BLEND_DESC));
	bld.IndependentBlendEnable = false;
	bld.AlphaToCoverageEnable = false;
	bld.RenderTarget[0].BlendEnable = true;
	bld.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bld.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bld.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bld.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bld.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bld.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bld.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	g_pDevice->CreateBlendState(&bld, &g_pBlendState);

	UINT mask = 0xffffffff;
	g_pDeviceContext->OMSetBlendState(g_pBlendState, NULL, mask);
}

#pragma region �V�F�[�_�[����Ԃ�


ID3D11VertexShader * Render2D::GetVertexShader(std::string vsName)
{
	return m_VertexShaderMap.at(vsName);
}

ID3D11PixelShader * Render2D::GetPixelShader(std::string psName)
{
	return m_PixelShaderMap.at(psName);
}

ID3DBlob ** Render2D::ppGetBlob(LPSTR blobName)
{
	return &m_BlobMap.at(blobName);
}

ID3DBlob * Render2D::GetBlob(LPSTR blobName)
{
	return m_BlobMap.at(blobName);
}

void Render2D::AddBlob(LPSTR blobName)
{
	ID3DBlob* blob;
	m_BlobMap.emplace(blobName,blob);
}

ID3D11VertexShader * Render2D::GetVertexShader(LPSTR name)
{
	return l_VertexShaderMap.at(name);
}

ID3D11PixelShader * Render2D::GetPixelShader(LPSTR name)
{
	return l_PixelShaderMap.at(name);
}

#pragma endregion

//������shader�ɑΉ�
//hlsl�t�@�C����ǂݍ��݃V�F�[�_�[���쐬����
HRESULT Render2D::MakeShader(const std::string shaderName, const char* szProfileName, const void * pShaderByteCode,SIZE_T byteCodeLength)
{
	char szProfile[3] = { 0 };
	memcpy(szProfile, szProfileName, 2);  //�擪2�o�C�g���R�s�[
	if (strcmp(szProfile, "vs") == 0)//Vertex Shader
	{
		ID3D11VertexShader* pVertexShader;
		if (FAILED(g_pDevice->CreateVertexShader(pShaderByteCode, byteCodeLength, NULL, &pVertexShader))) return E_FAIL;
		
		m_VertexShaderMap.emplace(shaderName, pVertexShader);//map�ɒǉ�
	}
	if (strcmp(szProfile, "ps") == 0)//Pixel Shader
	{
		ID3D11PixelShader* pPixelShader;
		if (FAILED(g_pDevice->CreatePixelShader(pShaderByteCode, byteCodeLength, NULL, &pPixelShader))) return E_FAIL;
		
		m_PixelShaderMap.emplace(shaderName, pPixelShader);//map�ɒǉ�
	}

	return S_OK;
}

HRESULT Render2D::LoadVertexShader(ID3D11Device * pDevice, const std::string key, const void * pShaderByteCode, SIZE_T byteCodeLength)
{
	ID3D11VertexShader* pVertexShader;
	if (FAILED(pDevice->CreateVertexShader(pShaderByteCode, byteCodeLength, NULL, &pVertexShader))) return E_FAIL;
	m_VertexShaderMap.emplace(key, pVertexShader);
}

HRESULT Render2D::LoadPixelShader(ID3D11Device * pDevice, const std::string key, const void * pShaderByteCode, SIZE_T byteCodeLength)
{
	ID3D11PixelShader* pPixelShader;
	if (FAILED(pDevice->CreatePixelShader(pShaderByteCode, byteCodeLength, NULL, &pPixelShader))) return E_FAIL;
	m_PixelShaderMap.emplace(key, pPixelShader);
}

HRESULT Render2D::MakeVertex(LPSTR name, ID3DBlob ** ppBlob)
{
	ID3D11VertexShader* pVertexShader;
	ID3DBlob *pErrors = NULL;
	if (FAILED(D3DX11CompileFromFileA(name, NULL, NULL, "VS", "vs_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, ppBlob, &pErrors, NULL)))
	{
		char*p = (char*)pErrors->GetBufferPointer();
		MessageBoxA(0, p, 0, MB_OK);
		return E_FAIL;
	}
	g_pDevice->CreateVertexShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, &pVertexShader);
	l_VertexShaderMap.emplace(name, pVertexShader);


	return S_OK;
}

HRESULT Render2D::MakePixel(LPSTR name, ID3DBlob ** ppBlob)
{
	ID3D11PixelShader* pPixelShader;
	ID3DBlob *pErrors = NULL;
	if (FAILED(D3DX11CompileFromFileA(name, NULL, NULL, "PS", "ps_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, ppBlob, &pErrors, NULL)))
	{
		char*p = (char*)pErrors->GetBufferPointer();
		MessageBoxA(0, p, 0, MB_OK);
		return E_FAIL;
	}
	g_pDevice->CreatePixelShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, &pPixelShader);
	l_PixelShaderMap.emplace(name,pPixelShader);
	
	return S_OK;
}

