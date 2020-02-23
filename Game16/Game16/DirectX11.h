#pragma once


#include "Singleton.h"
// DirectX11のヘッダーを含める
#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")

// 解放用テンプレート関数
template<class T>
void SafeRelease(T*& p)
{
	if (p)
	{
		p->Release();
		p = nullptr;
	}
}


// DirectX11クラス Singleton
class DirectX11 : public Singleton<DirectX11>
{
protected:
	DirectX11(void);                  //外部でのインスタンス作成は禁止
	virtual ~DirectX11(void);
public:

	friend class Singleton<DirectX11>;               //Singletonでのインスタンス作成は許可


	// 初期化
	bool Initialize();
	// 終了処理
	void Finalize(void);

public:
	// デバイスの取得
	ID3D11Device* GetDevice(void) { return m_pDevice; }
	// デバイスコンテキストの取得
	ID3D11DeviceContext* GetContext(void) { return m_pDeviceContext; }
	// スワップチェインの取得
	IDXGISwapChain* GetSwapChain(void) { return m_pSwapChain; }

private:
	ID3D11Device* m_pDevice;		        // DirectX11のデバイス
	ID3D11DeviceContext* m_pDeviceContext;	// 描画用のデバイスコンテキスト
	IDXGISwapChain* m_pSwapChain;		    // 画面出力用のスワップチェイン
};