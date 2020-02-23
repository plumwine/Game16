
#include "Window.h"
#include "DirectX11.h"
#include "Camera.h"

#include <crtdbg.h>

//DirecxtX11.h�ŏ��������Ă�̂ł�������������
ID3D11Device* g_pDevice;               // DirectX11�̃f�o�C�X
ID3D11DeviceContext* g_pDeviceContext; // �`��p�̃f�o�C�X�R���e�L�X�g����1���g���܂킷�B��j1�ڂ̏���`��A2�ڂ�`�悷��ۂɂ܂�������Ȃ�����2�ڂ��`�悳���
IDXGISwapChain* g_pSwapChain;		   // ��ʏo�͗p�̃X���b�v�`�F�C���@������1�����ł���

ID3D11RenderTargetView* g_pRTV;
ID3D11Texture2D* g_pDS;
ID3D11DepthStencilView* g_pDSV;
ID3D11InputLayout* g_pVertexLayout;
//�V�F�[�_�[�n
ID3D11VertexShader* g_pVertexShader;
ID3D11PixelShader* g_pPixelShader;
ID3D11Buffer* g_pConstantBuffer;          //�V�F�[�_�[�󂯓n���p�̒萔�ꎞ�ۑ�
//�o�͏��
ID3D11Buffer* g_pVertexBuffer;            //���_���





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

//�֐��v���g�^�C�v�錾
HRESULT MakeShader(LPSTR, LPSTR, LPSTR, void**, ID3DBlob**);
void init();
void app();

//������shader�ɑΉ�
//hlsl�t�@�C����ǂݍ��݃V�F�[�_�[���쐬����
HRESULT MakeShader(LPSTR szFileName, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob)
{
	ID3DBlob *pErrors = NULL;
	if (FAILED(D3DX11CompileFromFileA(szFileName, NULL, NULL, szFuncName, szProfileName, D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, ppBlob, &pErrors, NULL)))
	{
		char*p = (char*)pErrors->GetBufferPointer();
		MessageBoxA(0, p, 0, MB_OK);
		return E_FAIL;
	}
	char szProfile[3] = { 0 };
	memcpy(szProfile, szProfileName, 2);
	if (strcmp(szProfile, "vs") == 0)//Vertex Shader
	{
		g_pDevice->CreateVertexShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11VertexShader**)ppShader);
	}
	if (strcmp(szProfile, "ps") == 0)//Pixel Shader
	{
		g_pDevice->CreatePixelShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11PixelShader**)ppShader);
	}
	if (strcmp(szProfile, "gs") == 0)//Geometry Shader
	{
		g_pDevice->CreateGeometryShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11GeometryShader**)ppShader);
	}
	if (strcmp(szProfile, "hs") == 0)//Hull Shader
	{
		g_pDevice->CreateHullShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11HullShader**)ppShader);
	}
	if (strcmp(szProfile, "ds") == 0)//Domain Shader
	{
		g_pDevice->CreateDomainShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11DomainShader**)ppShader);
	}
	if (strcmp(szProfile, "cs") == 0)//Compute Shader
	{
		g_pDevice->CreateComputeShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11ComputeShader**)ppShader);
	}
	return S_OK;
}


// ������
void init() {

	////�e�X�g
	//�J�����̃V���O���g���C���X�^���X�쐬
	Camera::createInstance();
	//�V���O���g���̃J�����̏�����
	Camera::getInstance().init();
	//Buffer�N���X���g�����O�p�`�`��̃e�X�g�E������ɂ̂��Ƃ��Ă��

	//DirectX11.h�ŏ������������������Ă݂�
	g_pDevice = DirectX11::getInstance().GetDevice();        //render
	g_pDeviceContext = DirectX11::getInstance().GetContext();//render
	g_pSwapChain = DirectX11::getInstance().GetSwapChain();  //main


	//�o�b�N�o�b�t�@�[�̃����_�[�^�[�Q�b�g�r���[(RTV)���쐬
	ID3D11Texture2D *pBack;                                                  //render
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBack);	 //render
	g_pDevice->CreateRenderTargetView(pBack, NULL, &g_pRTV);				 //render
	pBack->Release();                                                        //render

	//�f�v�X�X�e���V���r���[(DSV)���쐬                                     //render
	D3D11_TEXTURE2D_DESC descDepth;											//render
	descDepth.Width = Window::getInstance().GetWidth();					//render
	descDepth.Height = Window::getInstance().GetHeight();					//render
	descDepth.MipLevels = 1;												//render
	descDepth.ArraySize = 1;												//render
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;								//render
	descDepth.SampleDesc.Count = 1;											//render
	descDepth.SampleDesc.Quality = 0;										//render
	descDepth.Usage = D3D11_USAGE_DEFAULT;									//render
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;							//render
	descDepth.CPUAccessFlags = 0;											//render
	descDepth.MiscFlags = 0;												//render
	g_pDevice->CreateTexture2D(&descDepth, NULL, &g_pDS);					//render
	g_pDevice->CreateDepthStencilView(g_pDS, NULL, &g_pDSV);				//render


	//�r���[�|�[�g�̐ݒ�                                                 //camera
	//D3D11_VIEWPORT vp;                                                 //camera
	//vp.Width  = Window::getInstance().GetWidth();                      //camera
	//vp.Height = Window::getInstance().GetHeight();                     //camera
	//vp.MinDepth = 0.0f;												 //camera
	//vp.MaxDepth = 1.0f;												 //camera
	//vp.TopLeftX = 0;													 //camera
	//vp.TopLeftY = 0;


	g_pDeviceContext->RSSetViewports(1, &Camera::getInstance().getViewPort());                            //render



	//shader
	ID3DBlob *pCompiledShader = NULL;
	//�o�[�e�b�N�X�V�F�[�_�[�쐬
	MakeShader((const LPSTR)"simpleShader.hlsl", (const LPSTR)"VS", (const LPSTR)"vs_5_0", (void**)&g_pVertexShader, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	g_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &g_pVertexLayout);

	//�s�N�Z���V�F�[�_�[�쐬
	MakeShader((const LPSTR)"simpleShader.hlsl", (const LPSTR)"PS", (const LPSTR)"ps_5_0", (void**)&g_pPixelShader, &pCompiledShader);


	//�R���X�^���g�o�b�t�@�[�쐬               
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	g_pDevice->CreateBuffer(&cb, NULL, &g_pConstantBuffer);

	//�g���C�A���O���쐬
	//�o�[�e�b�N�X�o�b�t�@�[�쐬
	SimpleVertex vertices[] =
	{
		D3DXVECTOR3(-0.5,-0.5,0),//���_1	
		D3DXVECTOR3(-0.5,0.7,0), //���_2
		D3DXVECTOR3(0.5,-0.5,0),  //���_3
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;


	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	g_pDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);

}



//�A�v���P�[�V����
void app()
{
	//render
	//�����_�[�^�[�Q�b�g�r���[�ƃf�v�X�X�e���V���r���[���Z�b�g
	g_pDeviceContext->OMSetRenderTargets(1, &g_pRTV, g_pDSV);
	//��ʃN���A
	float ClearColor[4] = { 0,0,0.5,1 };// �N���A�F�쐬�@RGBA�̏�

	g_pDeviceContext->ClearRenderTargetView(g_pRTV, ClearColor);//�J���[�o�b�t�@�N���A
	g_pDeviceContext->ClearDepthStencilView(g_pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//�f�v�X�X�e���V���o�b�t�@�N���A
	//render


	/////���̂̍��W�Ȃ�  WVP                    //camer
	//�X�̃��[�J�����W�iWorld�j
	D3DXMATRIX World;				            //camer
	//D3DXMATRIX View;				            //camer
	//D3DXMATRIX Proj;				            //camer
	//���[���h�g�����X�t�H�[��		            //camer
	//�P�ʍs��ɂ���
	D3DXMatrixIdentity(&World);		            //camer
												//camer
	//�ʒu										
	D3DXMATRIX Tran;                            //camer
	D3DXMatrixTranslation(&Tran, 0.5, 0.5, 20);	//camer
	World = Tran;								//camer
												//camer
	//��]�E�g��E�ʒu�̏��ɂ�����			    //camer
	World = Tran;								//camer

	////�J�����E���_

	// �r���[�g�����X�t�H�[��
	//static D3DXVECTOR3 vEyePt(0.0f, 0.0f, -2.0f); //���_�ʒu
	//vEyePt.x += 0.0001;
	//D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	//D3DXVECTOR3 vLookatPt = vEyePt + D3DXVECTOR3(0.0f, 0.0f, 1.0f);//�����ʒu
	//���_�ƒ����_�̗����𓯂��悤�ɓ������Ȃ��ƃJ���������s�ړ����Ȃ��̂ŗ��������悤�ɑ���������
	//D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
	//D3DXMatrixLookAtLH(&View, &vEyePt, &vLookatPt, &vUpVec);
	//�J�����̃����Y�֌W
	// �v���W�F�N�V�����g�����X�t�H�[��
	//D3DXMatrixPerspectiveFovLH(&Proj, D3DX_PI / 4, (FLOAT)Window::getInstance().GetWidth()/ (FLOAT)Window::getInstance().GetHeight(), 0.1f, 100.0f);

	//�g�p����V�F�[�_�[�̃Z�b�g
	g_pDeviceContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pDeviceContext->PSSetShader(g_pPixelShader, NULL, 0);


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
	g_pDeviceContext->Draw(3, 0);


	g_pSwapChain->Present(0, 0);//��ʍX�V
}

// �G���g���[�|�C���g
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{


	//�E�B���h�E�N���X�̃V���O���g���̃C���X�^���X����
	Window::createInstance();
	//Window�̐���
	//�����o���Ȃ�������I��
	if (!Window::getInstance().Create("Test", 0, 0, 860, 640))
	{
		return false;
	}

	//DirectX11��Singleton�C���X�^���X�쐬
	DirectX11::createInstance();
	//DirectX11�̏�����
	//�������ł��Ȃ�������I��
	if (!DirectX11::getInstance().Initialize())
	{
		return false;
	}

	init();  //������

	//���b�Z�[�W�ɏI���ʒm������܂ŉ�
	while (!Window::getInstance().IsQuitMessage())
	{


		//���������[�N���o
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		if (!Window::getInstance().UpdateMessage())
		{

			app();
		}
	}

	//Drectx11���I������
	DirectX11::getInstance().Finalize();

	return 0;
}
