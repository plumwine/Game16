#include "Render2D.h"
#include "Window.h"
#include "Camera.h"
#include "vs_tex.h"
#include "ps_tex.h"
#include "Math/Color4.h"
#include "Math/Vector2.h"
#include "Texture2D.h"

//#include "vs_test.h"
//#include "ps_test.h"

//頂点の構造体
struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos; //位置
	DirectX::XMFLOAT2 TexCoord;
};

//シェーダー用の定数バッファのApp側構造体
struct SHADER_CONSTANT_BUFFER
{
	//WorldViewProjection
	XMMATRIX mWVP;
	Vector2 mUVOffset;
	Vector2 mUVSize;
};


Render2D::Render2D()
	:m_MaxLayer(10)
{
}

Render2D::~Render2D()
{
}

void Render2D::init()
{
	//DirectX11.hで初期化した情報を代入してみる
	g_pDevice = DirectX11::getInstance().GetDevice();
	g_pDeviceContext = DirectX11::getInstance().GetContext();
	g_pSwapChain = DirectX11::getInstance().GetSwapChain();


	MakeShader("simpleShader", "vs", &g_vs_main, sizeof(g_vs_main));
	MakeShader("simpleShader", "ps", &g_ps_main, sizeof(g_ps_main));

	createRTV();
	createDSV();
	createConstantBuffer();
}
void Render2D::Draw()
{
	//レンダーターゲットビューとデプスステンシルビューをセット
	g_pDeviceContext->OMSetRenderTargets(1, &g_pRTV, g_pDSV);
	//画面クリア
	float ClearColor[4] = { 0,0,0.5,1 };// クリア色作成　画面背景　RGBAの順

	g_pDeviceContext->ClearRenderTargetView(g_pRTV, ClearColor);//カラーバッファクリア
	g_pDeviceContext->ClearDepthStencilView(g_pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//デプスステンシルバッファクリア

	//個々のローカル座標（World）
	D3DXMATRIX World;
	//単位行列にする
	D3DXMatrixIdentity(&World);
	//位置										
	D3DXMATRIX Tran;
	D3DXMatrixTranslation(&Tran, 0.0, 0.0, 0);
	//回転・拡大・位置の順にかける			    
	World = Tran;

	
	//使用するシェーダーのセット
	g_pDeviceContext->VSSetShader(GetVertexShader("simpleShader"), NULL, 0);
	g_pDeviceContext->PSSetShader(GetPixelShader("simpleShader"), NULL, 0);

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	SHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(g_pDeviceContext->Map(g_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		//cb.mWVP.tramspose(cb.mWVP);

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
	g_pDeviceContext->Draw(4, 0);

	g_pSwapChain->Present(0, 0);//画面更新  画面更新はここで行う
}

void Render2D::drawTexture2D(std::string textureName, Vector3 position, Vector3 angle, Vector3 scale, UINT layer)
{
	drawTexture2D(textureName, position, angle, scale , 0.0f, 0.0f, 1.0f, 1.0f, layer);
}

void Render2D::drawTexture2D(std::string textureName, Vector3 position, Vector3 angle, Vector3 scale, float uvX, float uvY, float uvWidth, float uvHeight, UINT layer)
{
	
	//描画情報を入れる
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

	//レンダーターゲットビューとデプスステンシルビューをセット
	g_pDeviceContext->OMSetRenderTargets(1, &g_pRTV, g_pDSV);
	//画面クリア
	float ClearColor[4] = { 0,0,0.5,1 };// クリア色作成　RGBAの順

	g_pDeviceContext->ClearRenderTargetView(g_pRTV, ClearColor);//カラーバッファクリア
	g_pDeviceContext->ClearDepthStencilView(g_pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//デプスステンシルバッファクリア


	g_pDeviceContext->VSSetShader(GetVertexShader("simpleShader"), NULL, 0);
	g_pDeviceContext->PSSetShader(GetPixelShader("simpleShader"), NULL, 0);



	for (int i = 0; i <= m_MaxLayer; i++)
	{
		if (mLayerDrawMap.find(i) == mLayerDrawMap.end())
			continue;

		for (auto drawInfo : mLayerDrawMap.at(i))
		{
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
//原則 2D
void Render2D::draw2D(std::string textureName, Vector3 position, Vector3 angle, Vector3 scale,
	float uvX, float uvY, float uvWidth, float uvHeight)
{
    //座標変換
	XMMATRIX translate_m = XMMatrixTranslationFromVector(position.toXMVector());
	
	//回転
	angle.x = XMConvertToRadians(angle.x);
	angle.y = XMConvertToRadians(angle.y);
	angle.z = XMConvertToRadians(angle.z);
	XMMATRIX rotate_m = XMMatrixRotationRollPitchYawFromVector(angle.toXMVector());
	//拡大・縮小
	Vector3 absSize(std::fabsf(scale.x), std::fabsf(scale.y), std::fabsf(scale.z));
	XMMATRIX scale_m = XMMatrixScalingFromVector(absSize.toXMVector());
	//World
	XMMATRIX world;
	//回転・拡大・位置の順にかける	
	world = scale_m * rotate_m * translate_m;


	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	SHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(g_pDeviceContext->Map(g_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		cb.mWVP = Camera::getInstance().getWorldView_Pjojection(world);
		//D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		cb.mWVP  = XMMatrixTranspose(cb.mWVP);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		g_pDeviceContext->Unmap(g_pConstantBuffer, 0);
	}

	//バーテックスバッファーをセット
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	g_pDeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	g_pDeviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);


	//テクスチャの選択
	ID3D11ShaderResourceView* texture = Texture2D::getInstance().getTextureView(textureName);
	g_pDeviceContext->PSSetShaderResources(0, 1, &texture);

	//このコンスタントバッファーをどのシェーダーで使うか
	g_pDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pDeviceContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);


	//プリミティブ・トポロジーをセット
	g_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//プリミティブをレンダリング
	g_pDeviceContext->Draw(4, 0);


}
//デフォルトで入れる
void Render2D::createRTV()
{
	//バックバッファーのレンダーターゲットビュー(RTV)を作成
	ID3D11Texture2D *pBack = NULL;
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBack);
	g_pDevice->CreateRenderTargetView(pBack, NULL, &g_pRTV);
	pBack->Release();
}

//外部で作られたものを入れる
void Render2D::createRTV(ID3D11Texture2D * pBack)
{
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBack);
	g_pDevice->CreateRenderTargetView(pBack, NULL, &g_pRTV);
	pBack->Release();
}

//デフォルトで入れる
void Render2D::createDSV()
{
	//デプスステンシルビュー(DSV)を作成                                     
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
	//ビューポート設定をここでする
	g_pDeviceContext->RSSetViewports(1, &Camera::getInstance().getViewPort());

}

//外部で作られたものを入れる
void Render2D::createDSV(D3D11_TEXTURE2D_DESC * pDescDepth)
{
	g_pDevice->CreateTexture2D(pDescDepth, NULL, &g_pDS);
	g_pDevice->CreateDepthStencilView(g_pDS, NULL, &g_pDSV);
	//ビューポート設定をここでする
	g_pDeviceContext->RSSetViewports(1, &Camera::getInstance().getViewPort());
}

//デフォルトで入れる
void Render2D::createConstantBuffer()
{
	//バーテックスシェーダー作成
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	g_pDevice->CreateInputLayout(layout, numElements, g_vs_main,sizeof(g_vs_main), &g_pVertexLayout);
	
	//コンスタントバッファー作成               
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	g_pDevice->CreateBuffer(&cb, NULL, &g_pConstantBuffer);

	//バーテックスバッファー作成   四角形
	SimpleVertex vertices[] =
	{
		{ {-0.5,-0.5, 0}, {0.0f, 1.0f}},   //頂点1 色	
		{ {-0.5, 0.5, 0}, {0.0f, 0.0f}},   //頂点2 色
		{ {0.5,-0.5, 0 }, {1.0f, 1.0f}},   //頂点3 色
		{ {0.5, 0.5, 0 }, {1.0f, 0.0f}}    //頂点4 色
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
	//バーテックスシェーダー作成
	//MakeShader("simpleShader.hlsl", "vs_5_0");
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点インプットレイアウトを作成
	//g_pDevice->CreateInputLayout(layout, numElements, GetCompileData("simpleShader.hlsl"), sizeof(GetCompileData("simpleShader.hlsl")), &g_pVertexLayout);

	//ピクセルシェーダー作成
	//MakeShader("simpleShader.hlsl", "ps_5_0");


	//コンスタントバッファーの作成
	g_pDevice->CreateBuffer(constantBuffer, NULL, &g_pConstantBuffer);
	D3D11_SUBRESOURCE_DATA InitData;

	//バーテックスバッファー作成   四角形
	SimpleVertex vertices[] =
	{
		{ {-0.5,-0.5, 0},{0.0f, 1.0f}},   //頂点1 色	
		{ {-0.5, 0.5, 0},{0.0f, 0.0f}},   //頂点2 色
		{ {0.5,-0.5, 0 }, {1.0f, 1.0f}},   //頂点3 色
		{ {0.5, 0.5, 0 }, {1.0f, 0.0f}}    //頂点4 色
	};

	//頂点情報の取得
	InitData.pSysMem = vertices;
	g_pDevice->CreateBuffer(vertexBuffer, &InitData, &g_pVertexBuffer);


}

#pragma region シェーダー情報を返す


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

#pragma endregion


//多くのshaderに対応
//hlslファイルを読み込みシェーダーを作成する
HRESULT Render2D::MakeShader(const std::string shaderName, const char* szProfileName, const void * pShaderByteCode,SIZE_T byteCodeLength)
{
	char szProfile[3] = { 0 };
	memcpy(szProfile, szProfileName, 2);  //先頭2バイトをコピー
	if (strcmp(szProfile, "vs") == 0)//Vertex Shader
	{
		ID3D11VertexShader* pVertexShader;
		//if (FAILED(g_pDevice->CreateVertexShader(pShaderByteCode, byteCodeLength, NULL, &pVertexShader))) return E_FAIL;
		g_pDevice->CreateVertexShader(pShaderByteCode, byteCodeLength, NULL, &pVertexShader);
		m_VertexShaderMap.emplace(shaderName, pVertexShader);//mapに追加
	}
	if (strcmp(szProfile, "ps") == 0)//Pixel Shader
	{
		ID3D11PixelShader* pPixelShader;
		g_pDevice->CreatePixelShader(pShaderByteCode, byteCodeLength, NULL, &pPixelShader);
		m_PixelShaderMap.emplace(shaderName, pPixelShader);//mapに追加
	}
	if (strcmp(szProfile, "gs") == 0)//Geometry Shader
	{
		ID3D11GeometryShader* pGeometryShader;
		g_pDevice->CreateGeometryShader(pShaderByteCode, byteCodeLength, NULL, &pGeometryShader);
		m_GeometryShaderMap.emplace(shaderName, pGeometryShader);//mapに追加
	}
	if (strcmp(szProfile, "hs") == 0)//Hull Shader
	{
		ID3D11HullShader* pHullShader;
		g_pDevice->CreateHullShader(pShaderByteCode, byteCodeLength, NULL, &pHullShader);
		m_HullShaderMap.emplace(shaderName, pHullShader);//mapに追加
	}
	if (strcmp(szProfile, "ds") == 0)//Domain Shader
	{
		ID3D11DomainShader* pDomainShader;
		g_pDevice->CreateDomainShader(pShaderByteCode, byteCodeLength, NULL, &pDomainShader);
		m_DomainShaderMap.emplace(shaderName, pDomainShader);//mapに追加
	}
	if (strcmp(szProfile, "cs") == 0)//Compute Shader
	{
		ID3D11ComputeShader* pComputeShader;
		g_pDevice->CreateComputeShader(pShaderByteCode, byteCodeLength, NULL, &pComputeShader);
		m_ComputeShaderMap.emplace(shaderName, pComputeShader);//mapに追加
	}

	return S_OK;
}
