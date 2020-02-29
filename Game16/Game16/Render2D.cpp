#include "Render2D.h"
#include "Window.h"
#include "Camera.h"


//���_�̍\����
struct SimpleVertex
{
	D3DXVECTOR3 Pos; //�ʒu
};

//�V�F�[�_�[�p�̒萔�o�b�t�@��App���\����
struct SHADER_CONSTANT_BUFFER
{
	//WorldViewProjection
	D3DXMATRIX mWVP;
};
Render2D::Render2D()
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

	MakeShader("simpleShader.hlsl", "vs_5_0");
	MakeShader("simpleShader.hlsl", "ps_5_0");

	createRTV();
	createDSV();
	createConstantBuffer();
}
void Render2D::Draw()
{
	//�����_�[�^�[�Q�b�g�r���[�ƃf�v�X�X�e���V���r���[���Z�b�g
	g_pDeviceContext->OMSetRenderTargets(1, &g_pRTV, g_pDSV);
	//��ʃN���A
	float ClearColor[4] = { 0,0,0.5,1 };// �N���A�F�쐬�@RGBA�̏�

	g_pDeviceContext->ClearRenderTargetView(g_pRTV, ClearColor);//�J���[�o�b�t�@�N���A
	g_pDeviceContext->ClearDepthStencilView(g_pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//�f�v�X�X�e���V���o�b�t�@�N���A

	//�X�̃��[�J�����W�iWorld�j
	D3DXMATRIX World;
	//�P�ʍs��ɂ���
	D3DXMatrixIdentity(&World);
	//�ʒu										
	D3DXMATRIX Tran;
	D3DXMatrixTranslation(&Tran, 0.5, 0.5, 20);
	World = Tran;

	//��]�E�g��E�ʒu�̏��ɂ�����			    
	World = Tran;


	//�g�p����V�F�[�_�[�̃Z�b�g
	g_pDeviceContext->VSSetShader(GetVertexShader("simpleShader.hlsl"), NULL, 0);
	g_pDeviceContext->PSSetShader(GetPixelShader("simpleShader.hlsl"), NULL, 0);

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	SHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(g_pDeviceContext->Map(g_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�A�J�����A�ˉe�s���n��
		cb.mWVP = Camera::getInstance().getWorldView_Pjojection(World);
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);

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
//�f�t�H���g�œ����
void Render2D::createRTV()
{
	//�o�b�N�o�b�t�@�[�̃����_�[�^�[�Q�b�g�r���[(RTV)���쐬
	ID3D11Texture2D *pBack = NULL;
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBack);
	g_pDevice->CreateRenderTargetView(pBack, NULL, &g_pRTV);
	pBack->Release();
}

//�O���ō��ꂽ���̂�����
void Render2D::createRTV(ID3D11Texture2D * pBack)
{
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBack);
	g_pDevice->CreateRenderTargetView(pBack, NULL, &g_pRTV);
	pBack->Release();
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

//�O���ō��ꂽ���̂�����
void Render2D::createDSV(D3D11_TEXTURE2D_DESC * pDescDepth)
{
	g_pDevice->CreateTexture2D(pDescDepth, NULL, &g_pDS);
	g_pDevice->CreateDepthStencilView(g_pDS, NULL, &g_pDSV);
	//�r���[�|�[�g�ݒ�������ł���
	g_pDeviceContext->RSSetViewports(1, &Camera::getInstance().getViewPort());
}

//�f�t�H���g�œ����
void Render2D::createConstantBuffer()
{
	//�o�[�e�b�N�X�V�F�[�_�[�쐬
	//MakeShader("simpleShader.hlsl", "vs_5_0");
	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	g_pDevice->CreateInputLayout(layout, numElements, GetCompileData("simpleShader.hlsl"), sizeof(GetCompileData("simpleShader.hlsl")), &g_pVertexLayout);

	//�s�N�Z���V�F�[�_�[�쐬
	//MakeShader("simpleShader.hlsl","ps_5_0");

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
		D3DXVECTOR3(-0.5,-0.5,0), //���_1	
		D3DXVECTOR3(-0.5,0.5,0),  //���_2
		D3DXVECTOR3(0.5,-0.5,0),  //���_3
		D3DXVECTOR3(0.5,0.5,0),   //���_4
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
}

void Render2D::createConstantBuffer(D3D11_BUFFER_DESC *constantBuffer, D3D11_BUFFER_DESC* vertexBuffer)
{

	//shader
	//ID3DBlob *pCompiledShader = NULL;
	//�o�[�e�b�N�X�V�F�[�_�[�쐬
	MakeShader("simpleShader.hlsl", "vs_5_0");
	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_�C���v�b�g���C�A�E�g���쐬
	g_pDevice->CreateInputLayout(layout, numElements, GetCompileData("simpleShader.hlsl"), sizeof(GetCompileData("simpleShader.hlsl")), &g_pVertexLayout);

	//�s�N�Z���V�F�[�_�[�쐬
	MakeShader("simpleShader.hlsl", "ps_5_0");


	//�R���X�^���g�o�b�t�@�[�̍쐬
	g_pDevice->CreateBuffer(constantBuffer, NULL, &g_pConstantBuffer);
	D3D11_SUBRESOURCE_DATA InitData;

	//�o�[�e�b�N�X�o�b�t�@�[�쐬
	SimpleVertex vertices[] =
	{
		D3DXVECTOR3(-0.5,-0.5,0), //���_1	
		D3DXVECTOR3(-0.5,0.5,0),  //���_2
		D3DXVECTOR3(0.5,-0.5,0),  //���_3
		D3DXVECTOR3(0.5,0.5,0),  //���_4
	};

	//���_���̎擾
	InitData.pSysMem = vertices;
	g_pDevice->CreateBuffer(vertexBuffer, &InitData, &g_pVertexBuffer);


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

ID3D11GeometryShader * Render2D::GetGeometryShader(std::string gsName)
{
	return m_GeometryShaderMap.at(gsName);
}

ID3D11HullShader * Render2D::GetHullShader(std::string hsName)
{
	return m_HullShaderMap.at(hsName);
}

ID3D11DomainShader * Render2D::GetDomainShader(std::string dsName)
{
	return m_DomainShaderMap.at(dsName);
}

ID3D11ComputeShader * Render2D::GetComputeShader(std::string csName)
{
	return m_ComputeShaderMap.at(csName);
}

unsigned char* Render2D::GetCompileData(std::string cpName)
{
	return m_CompileData.at(cpName);
}

#pragma endregion


//������shader�ɑΉ�
//hlsl�t�@�C����ǂݍ��݃V�F�[�_�[���쐬����
HRESULT Render2D::MakeShader(const char* shaderName, const char* szProfileName)
{
	std::string  s_SahderName = shaderName;

	//�o�C�i���t�@�C����ǂݍ���

	FILE* fp;
	errno_t error;
	error = fopen_s(&fp, shaderName, "rb");
	if (error != 0)
	{
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	long cso_sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	unsigned char* cso_data = new unsigned char[cso_sz];
	fread(cso_data, cso_sz, 1, fp);

	//���Ȃ��悤�ɂ���
	auto itr = m_CompileData.find(s_SahderName);
	if (!(itr != m_CompileData.end()))
	{
		m_CompileData.emplace(s_SahderName, cso_data);
	}
	fclose(fp);
	//���̎��_�ŃR���p�C����̃o�C�i����cso_data�Ɋi�[����Ă���//

	char szProfile[3] = { 0 };
	memcpy(szProfile, szProfileName, 2);  //�擪2�o�C�g���R�s�[
	if (strcmp(szProfile, "vs") == 0)//Vertex Shader
	{
		ID3D11VertexShader* pVertexShader;
		g_pDevice->CreateVertexShader(cso_data, cso_sz, NULL, &pVertexShader);
		m_VertexShaderMap.emplace(s_SahderName, pVertexShader);//map�ɒǉ�
	}
	if (strcmp(szProfile, "ps") == 0)//Pixel Shader
	{
		ID3D11PixelShader* pPixelShader;
		g_pDevice->CreatePixelShader(cso_data, cso_sz, NULL, &pPixelShader);
		m_PixelShaderMap.emplace(s_SahderName, pPixelShader);//map�ɒǉ�
	}
	if (strcmp(szProfile, "gs") == 0)//Geometry Shader
	{
		ID3D11GeometryShader* pGeometryShader;
		g_pDevice->CreateGeometryShader(cso_data, cso_sz, NULL, &pGeometryShader);
		m_GeometryShaderMap.emplace(s_SahderName, pGeometryShader);//map�ɒǉ�
	}
	if (strcmp(szProfile, "hs") == 0)//Hull Shader
	{
		ID3D11HullShader* pHullShader;
		g_pDevice->CreateHullShader(cso_data, cso_sz, NULL, &pHullShader);
		m_HullShaderMap.emplace(s_SahderName, pHullShader);//map�ɒǉ�
	}
	if (strcmp(szProfile, "ds") == 0)//Domain Shader
	{
		ID3D11DomainShader* pDomainShader;
		g_pDevice->CreateDomainShader(cso_data, cso_sz, NULL, &pDomainShader);
		m_DomainShaderMap.emplace(s_SahderName, pDomainShader);//map�ɒǉ�
	}
	if (strcmp(szProfile, "cs") == 0)//Compute Shader
	{
		ID3D11ComputeShader* pComputeShader;
		g_pDevice->CreateComputeShader(cso_data, cso_sz, NULL, &pComputeShader);
		m_ComputeShaderMap.emplace(s_SahderName, pComputeShader);//map�ɒǉ�
	}

	//new�����̂ŏ���
	delete[] cso_data;

	return S_OK;
}
