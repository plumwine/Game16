#pragma once
#include "DirectX11.h"
#include "Camera.h"
#include "Singleton.h"
#include "Math/Vector3.h"
#include <unordered_map>
#include <vector>
#include <d3dcompiler.h>
#include <d3d11shader.h>

//2D描画情報
struct DrawInfomation2D
{
	std::string textureName;    //テクスチャ名
	Vector3 position;			//位置
	Vector3 angles;				//回転
	Vector3 scale;				//拡大縮小
	float uvX;					//uv関係
	float uvY;					//
	float uvWidth;				//
	float uvHeight;				//

};



//レンダラー2Dのシングルトン
class Render2D :public Singleton<Render2D>
{
protected:
	Render2D();
	virtual ~Render2D();

public:
	friend class Singleton<Render2D>;               //Singletonでのインスタンス作成は許可

	//初期化
	void init();                               //何も設定しなければinitだけでいい
	void Draw();                               //描画

	//2D（UI関係の）描画 使用する方
	void drawTexture2D(std::string textureName, Vector3 position, Vector3 angle, Vector3 scale, UINT layer = 0);
	
	void drawTexture2D(
		std::string textureName, Vector3 position, Vector3 angles, Vector3 scale,
		float uvX, float uvY, float uvWidth, float uvHeight, UINT layer = 0);

	void drawManager();
	//いつもの
	void draw2D(std::string textureName,Vector3 position, Vector3 angle,Vector3 scale, float uvX, float uvY, float uvWidth, float uvHeight);


	//バックバッファーのレンダーターゲットビュー(RTV)を作成
	void createRTV();
	//デプスステンシルビュー(DSV)を作成    
	void createDSV();
	//シェーダーに返す用のコンスタンとバッファー
	void createConstantBuffer();

	//シェーダー情報の取得
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

	ID3D11Device* g_pDevice;               // DirectX11のデバイス
	ID3D11DeviceContext* g_pDeviceContext; // 描画用のデバイスコンテキストこの1つを使いまわす。例）1つ目の情報を描画、2つ目を描画する際にまた代入しなおせば2つ目が描画される
	IDXGISwapChain* g_pSwapChain;		   // 画面出力用のスワップチェイン　こいつは1つだけでいい

	ID3D11SamplerState* g_pSampleLinear;
	ID3D11RenderTargetView* g_pRTV;
	ID3D11Texture2D* g_pDS;
	ID3D11DepthStencilView* g_pDSV;
	ID3D11InputLayout* g_pVertexLayout;
	ID3D11BlendState* g_pBlendState;

	//シェーダー系
	ID3D11Buffer* g_pConstantBuffer;          //シェーダー受け渡し用の定数一時保存
	ID3D11Buffer* g_pVertexBuffer;            //頂点情報

	//シェーダー管理用
	//std::unordered_map<std::string, ID3D11VertexShader*>    m_VertexShaderMap;      //頂点シェーダー管理用
	//std::unordered_map<std::string, ID3D11PixelShader*>     m_PixelShaderMap;       //ピクセルシェーダー管理用		
	

	//レイヤー関係
	int m_MaxLayer;
	std::unordered_map<UINT, std::vector<DrawInfomation2D>> mLayerDrawMap;
};

