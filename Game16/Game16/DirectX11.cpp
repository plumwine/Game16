#include "DirectX11.h"
#include "Window.h"


//コンストラクタ
DirectX11::DirectX11(void)
	: m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_pSwapChain(nullptr)
{}

//デスストラクタ
DirectX11::~DirectX11(void)
{}

// 初期化
bool DirectX11::Initialize()
{
	DXGI_SWAP_CHAIN_DESC desc{};
	// バッファ情報の設定
	desc.BufferCount = 1;
	desc.BufferDesc.Width = Window::getInstance().GetWidth();
	desc.BufferDesc.Height = Window::getInstance().GetHeight();
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// MSAAの設定
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	// ウィンドウ設定
	desc.Windowed = true;
	desc.OutputWindow = Window::getInstance().GetHandle();

	//	機能レベル
	const D3D_FEATURE_LEVEL FeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,		// DirectX 11.0
		//D3D_FEATURE_LEVEL_10_1,	// DirectX 10.1
		//D3D_FEATURE_LEVEL_10_0,	// DirectX 10.0
	};
	UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);

	D3D_FEATURE_LEVEL level;
	HRESULT hr;
	//	デバイスを作成
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		FeatureLevels,
		NumFeatureLevels,
		D3D11_SDK_VERSION,
		&desc,
		&m_pSwapChain,
		&m_pDevice,
		&level,
		&m_pDeviceContext
	);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

// 終了処理
void DirectX11::Finalize(void)
{
	SafeRelease(m_pDevice);
	SafeRelease(m_pDeviceContext);
	SafeRelease(m_pSwapChain);
}