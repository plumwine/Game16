
#include "Window.h"
#include "DirectX11.h"
#include "Camera.h"

#include <crtdbg.h>

//DirecxtX11.hで初期化してるのでそこから代入する
ID3D11Device* g_pDevice;               // DirectX11のデバイス
ID3D11DeviceContext* g_pDeviceContext; // 描画用のデバイスコンテキストこの1つを使いまわす。例）1つ目の情報を描画、2つ目を描画する際にまた代入しなおせば2つ目が描画される
IDXGISwapChain* g_pSwapChain;		   // 画面出力用のスワップチェイン　こいつは1つだけでいい

ID3D11RenderTargetView* g_pRTV;
ID3D11Texture2D* g_pDS;
ID3D11DepthStencilView* g_pDSV;
ID3D11InputLayout* g_pVertexLayout;
//シェーダー系
ID3D11VertexShader* g_pVertexShader;
ID3D11PixelShader* g_pPixelShader;
ID3D11Buffer* g_pConstantBuffer;          //シェーダー受け渡し用の定数一時保存
//出力情報
ID3D11Buffer* g_pVertexBuffer;            //頂点情報





//頂点の構造体
struct SimpleVertex
{
	D3DXVECTOR3 Pos; //位置
};

//シェーダー用の定数バッファのApp側構造体
struct SHADER_CONSTANT_BUFFER
{
	//WorldViewProjection
	D3DXMATRIX mWVP;
};

//関数プロトタイプ宣言
HRESULT MakeShader(LPSTR, LPSTR, LPSTR, void**, ID3DBlob**);
void init();
void app();

//多くのshaderに対応
//hlslファイルを読み込みシェーダーを作成する
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


// 初期化
void init() {

	////テスト
	//カメラのシングルトンインスタンス作成
	Camera::createInstance();
	//シングルトンのカメラの初期化
	Camera::getInstance().init();
	//Bufferクラスを使った三角形描画のテスト・超入門にのっとってやる

	//DirectX11.hで初期化した情報を代入してみる
	g_pDevice = DirectX11::getInstance().GetDevice();        //render
	g_pDeviceContext = DirectX11::getInstance().GetContext();//render
	g_pSwapChain = DirectX11::getInstance().GetSwapChain();  //main


	//バックバッファーのレンダーターゲットビュー(RTV)を作成
	ID3D11Texture2D *pBack;                                                  //render
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBack);	 //render
	g_pDevice->CreateRenderTargetView(pBack, NULL, &g_pRTV);				 //render
	pBack->Release();                                                        //render

	//デプスステンシルビュー(DSV)を作成                                     //render
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


	//ビューポートの設定                                                 //camera
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
	//バーテックスシェーダー作成
	MakeShader((const LPSTR)"simpleShader.hlsl", (const LPSTR)"VS", (const LPSTR)"vs_5_0", (void**)&g_pVertexShader, &pCompiledShader);
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	g_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &g_pVertexLayout);

	//ピクセルシェーダー作成
	MakeShader((const LPSTR)"simpleShader.hlsl", (const LPSTR)"PS", (const LPSTR)"ps_5_0", (void**)&g_pPixelShader, &pCompiledShader);


	//コンスタントバッファー作成               
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	g_pDevice->CreateBuffer(&cb, NULL, &g_pConstantBuffer);

	//トライアングル作成
	//バーテックスバッファー作成
	SimpleVertex vertices[] =
	{
		D3DXVECTOR3(-0.5,-0.5,0),//頂点1	
		D3DXVECTOR3(-0.5,0.7,0), //頂点2
		D3DXVECTOR3(0.5,-0.5,0),  //頂点3
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



//アプリケーション
void app()
{
	//render
	//レンダーターゲットビューとデプスステンシルビューをセット
	g_pDeviceContext->OMSetRenderTargets(1, &g_pRTV, g_pDSV);
	//画面クリア
	float ClearColor[4] = { 0,0,0.5,1 };// クリア色作成　RGBAの順

	g_pDeviceContext->ClearRenderTargetView(g_pRTV, ClearColor);//カラーバッファクリア
	g_pDeviceContext->ClearDepthStencilView(g_pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//デプスステンシルバッファクリア
	//render


	/////物体の座標など  WVP                    //camer
	//個々のローカル座標（World）
	D3DXMATRIX World;				            //camer
	//D3DXMATRIX View;				            //camer
	//D3DXMATRIX Proj;				            //camer
	//ワールドトランスフォーム		            //camer
	//単位行列にする
	D3DXMatrixIdentity(&World);		            //camer
												//camer
	//位置										
	D3DXMATRIX Tran;                            //camer
	D3DXMatrixTranslation(&Tran, 0.5, 0.5, 20);	//camer
	World = Tran;								//camer
												//camer
	//回転・拡大・位置の順にかける			    //camer
	World = Tran;								//camer

	////カメラ・視点

	// ビュートランスフォーム
	//static D3DXVECTOR3 vEyePt(0.0f, 0.0f, -2.0f); //視点位置
	//vEyePt.x += 0.0001;
	//D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	//D3DXVECTOR3 vLookatPt = vEyePt + D3DXVECTOR3(0.0f, 0.0f, 1.0f);//注視位置
	//視点と注視点の両方を同じように動かさないとカメラが平行移動しないので両方同じように増幅させる
	//D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
	//D3DXMatrixLookAtLH(&View, &vEyePt, &vLookatPt, &vUpVec);
	//カメラのレンズ関係
	// プロジェクショントランスフォーム
	//D3DXMatrixPerspectiveFovLH(&Proj, D3DX_PI / 4, (FLOAT)Window::getInstance().GetWidth()/ (FLOAT)Window::getInstance().GetHeight(), 0.1f, 100.0f);

	//使用するシェーダーのセット
	g_pDeviceContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pDeviceContext->PSSetShader(g_pPixelShader, NULL, 0);


	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	SHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(g_pDeviceContext->Map(g_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		cb.mWVP = Camera::getInstance().getWorldView_Pjojection(World);
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		g_pDeviceContext->Unmap(g_pConstantBuffer, 0);
	}
	//このコンスタントバッファーをどのシェーダーで使うか
	g_pDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pDeviceContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	//バーテックスバッファーをセット
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	g_pDeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	//頂点インプットレイアウトをセット
	g_pDeviceContext->IASetInputLayout(g_pVertexLayout);
	//プリミティブ・トポロジーをセット
	g_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//プリミティブをレンダリング
	g_pDeviceContext->Draw(3, 0);


	g_pSwapChain->Present(0, 0);//画面更新
}

// エントリーポイント
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{


	//ウィンドウクラスのシングルトンのインスタンス生成
	Window::createInstance();
	//Windowの生成
	//生成出来なかったら終了
	if (!Window::getInstance().Create("Test", 0, 0, 860, 640))
	{
		return false;
	}

	//DirectX11のSingletonインスタンス作成
	DirectX11::createInstance();
	//DirectX11の初期化
	//初期化できなかったら終了
	if (!DirectX11::getInstance().Initialize())
	{
		return false;
	}

	init();  //初期化

	//メッセージに終了通知がくるまで回す
	while (!Window::getInstance().IsQuitMessage())
	{


		//メモリリーク検出
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		if (!Window::getInstance().UpdateMessage())
		{

			app();
		}
	}

	//Drectx11を終了する
	DirectX11::getInstance().Finalize();

	return 0;
}
