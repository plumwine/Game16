
#include "Window.h"
#include "Camera.h"
#include "Render2D.h"
#include <crtdbg.h>

//関数プロトタイプ宣言
void init();
void app();

// 初期化
void init()
{
	//カメラのシングルトンインスタンス作成
	Camera::createInstance();
	//シングルトンのカメラの初期化
	Camera::getInstance().init();
	//レンダラーのシングルトン作成
	Render2D::createInstance();
	Render2D::getInstance().init();   //レンダーの初期化
}
//アプリケーション
void app()
{
	Render2D::getInstance().Draw();
}
// エントリーポイント
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//ウィンドウクラスのシングルトンのインスタンス生成
	Window::createInstance();
	//Windowの生成                               生成出来なかったら終了
	if (!Window::getInstance().Create("Test", 0, 0, 860, 640)) return false;
	//DirectX11のSingletonインスタンス作成
	DirectX11::createInstance();
	//DirectX11の初期化                          初期化できなかったら終了
	if (!DirectX11::getInstance().Initialize()) return false;
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
