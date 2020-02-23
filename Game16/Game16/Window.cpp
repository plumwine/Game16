#include "Window.h"


// ウインドウプロシージャー
static LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
	case WM_CLOSE:		// 閉じる際にWindowを破棄する
		DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:	// プログラムの終了を通知する
		PostQuitMessage(0);
		return 0L;
	}
	// 既定のウィンドウプロシージャを呼び出す
	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

// コンストラクタ
Window::Window(void)
	: m_hWnd(NULL)
	, m_isQuitMessage(false)
	, m_width(0)
	, m_height(0)
	, m_hmyb(CreateSolidBrush(RGB(0, 0, 100)))
{
}

// デストラクタ
Window::~Window(void)
{
}

// シンプルなウィンドウの作成
bool Window::Create(const char* pName, int x, int y, int width, int height)
{
	WNDCLASSEX wcex;
	// ウインドウの設定
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = m_hmyb;// (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = pName;
	wcex.hIcon = LoadIcon(NULL, IDC_ARROW);
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassExA(&wcex);

	DWORD dwStyle = WS_OVERLAPPEDWINDOW;

	RECT Rect;
	Rect.left = 0;
	Rect.top = 0;
	Rect.right = width;
	Rect.bottom = height;
	// ウィンドウのスタイルに合わせた適切なサイズを取得する
	AdjustWindowRect(&Rect, dwStyle, false);

	width = Rect.right - Rect.left;
	height = Rect.bottom - Rect.top;

	// ウインドウの生成
	m_hWnd = CreateWindowA(wcex.lpszClassName,
		pName,
		dwStyle,
		x,
		y,
		width,
		height,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL);
	if (m_hWnd == NULL)
	{
		return false;
	}

	m_width = width;
	m_height = height;
	// ウインドウの表示
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return true;
}

// ウィンドウを閉じる
void Window::Quit(void)
{
	SendMessage(m_hWnd, WM_CLOSE, 0, 0);
}

// メッセージの更新
bool Window::UpdateMessage(void)
{
	MSG msg;
	// メッセージが存在するか確認
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// 終了通知が来ている場合は抜ける
		if (msg.message == WM_QUIT)
		{
			m_isQuitMessage = true;
		}
		else
		{
			// メッセージをウインドウプロシージャに転送
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return true;
	}
	return false;
}
