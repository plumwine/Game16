#pragma once
#include "DirectX11.h"
#include "Singleton.h"
#include "Math/Vector3.h"
#include <unordered_map>
#include <vector>


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
	void createRTV(ID3D11Texture2D *pBack);

	//デプスステンシルビュー(DSV)を作成    
	void createDSV();
	void createDSV(D3D11_TEXTURE2D_DESC* pDescDepth);

	//シェーダーに返す用のコンスタンとバッファー
	void createConstantBuffer();
	void createConstantBuffer(D3D11_BUFFER_DESC* constantBuffer, D3D11_BUFFER_DESC* vertexBuffer);

	//シェーダー情報の取得
	ID3D11VertexShader* GetVertexShader(std::string vsName);
	ID3D11PixelShader* GetPixelShader(std::string psName);
	ID3D11GeometryShader* GetGeometryShader(std::string gsName);
	ID3D11HullShader* GetHullShader(std::string hsName);
	ID3D11DomainShader* GetDomainShader(std::string dsName);
	ID3D11ComputeShader* GetComputeShader(std::string csName);

	HRESULT MakeShader(const std::string shaderName, const char* szProfileName,const void * pShaderByteCode,SIZE_T byteCodeLength);

private:

	ID3D11Device* g_pDevice;               // DirectX11のデバイス
	ID3D11DeviceContext* g_pDeviceContext; // 描画用のデバイスコンテキストこの1つを使いまわす。例）1つ目の情報を描画、2つ目を描画する際にまた代入しなおせば2つ目が描画される
	IDXGISwapChain* g_pSwapChain;		   // 画面出力用のスワップチェイン　こいつは1つだけでいい

	ID3D11RenderTargetView* g_pRTV;
	ID3D11Texture2D* g_pDS;
	ID3D11DepthStencilView* g_pDSV;
	ID3D11InputLayout* g_pVertexLayout;

	//シェーダー系
	ID3D11Buffer* g_pConstantBuffer;          //シェーダー受け渡し用の定数一時保存
	ID3D11Buffer* g_pVertexBuffer;            //頂点情報

	//シェーダー管理用
	std::unordered_map<std::string, ID3D11VertexShader*>    m_VertexShaderMap;      //頂点シェーダー管理用
	std::unordered_map<std::string, ID3D11PixelShader*>     m_PixelShaderMap;       //ピクセルシェーダー管理用		
	std::unordered_map<std::string, ID3D11GeometryShader*>  m_GeometryShaderMap;    //ジオメトリシェーダー管理用
	std::unordered_map<std::string, ID3D11HullShader*>      m_HullShaderMap;        //ハルシェーダー管理用
	std::unordered_map<std::string, ID3D11DomainShader*>    m_DomainShaderMap;      //ドメインシェーダー管理用
	std::unordered_map<std::string, ID3D11ComputeShader*>   m_ComputeShaderMap;     //コンピュートシェーダー管理用



	//レイヤー関係
	int m_MaxLayer;
	std::unordered_map<UINT, std::vector<DrawInfomation2D>> mLayerDrawMap;
};

