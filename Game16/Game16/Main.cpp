#include <windows.h>
#include <crtdbg.h>

//定数定義
#define NAME "Game16"
#define WINDOW_WIDTH 640 //ウィンドウ幅
#define WINDOW_HEIGHT 480 //ウィンドウ高さ

//グローバル変数
HWND g_hWnd = NULL;

//関数プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void init();   //初期化
void app();    //アプリケーション


//エントリーポイント
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	//初期化一回しか呼ばれない

	//ウィンドウの設定
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);  //ウィンドウサイズ（WNDCLASSEX型のサイズ）を入れる
	wc.lpfnWndProc = WndProc;//ウィンドウのメッセージを処理するコールバック関数へのポインタ
	wc.hInstance = hInst;    //このクラスのための振動プロジャーがあるインスタンスハンドル
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);//ウィンドウ背景色
	wc.lpszClassName = NAME;
	//ウィンドウの登録
	if (!RegisterClassEx(&wc)) return 1;

	//ウィンドウ作成
	g_hWnd = CreateWindow(NAME, NAME, WS_OVERLAPPEDWINDOW, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInst, 0);
	init();




	ShowWindow(g_hWnd, SW_SHOW); //ウィンドウ表示
	UpdateWindow(g_hWnd);        //ウィンドウの更新

	//メッセージループ
	MSG msg = { 0 };

	//ここで更新
	//終了するまで止まらない
	while (msg.message != WM_QUIT)
	{
		//メモリリーク検出
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
		}
		else
		{
			//アプリケーションを更新
			app();
		}
	}

	return 0;

}

//ウィンドウプロジャーの設定
LRESULT CALLBACK WndProc(HWND  hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

//初期化
void init()
{
}

//アプリケーション
void app()
{
}

