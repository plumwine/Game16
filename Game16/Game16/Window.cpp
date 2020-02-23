#include "Window.h"


// �E�C���h�E�v���V�[�W���[
static LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
	case WM_CLOSE:		// ����ۂ�Window��j������
		DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:	// �v���O�����̏I����ʒm����
		PostQuitMessage(0);
		return 0L;
	}
	// ����̃E�B���h�E�v���V�[�W�����Ăяo��
	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

// �R���X�g���N�^
Window::Window(void)
	: m_hWnd(NULL)
	, m_isQuitMessage(false)
	, m_width(0)
	, m_height(0)
	, m_hmyb(CreateSolidBrush(RGB(0, 0, 100)))
{
}

// �f�X�g���N�^
Window::~Window(void)
{
}

// �V���v���ȃE�B���h�E�̍쐬
bool Window::Create(const char* pName, int x, int y, int width, int height)
{
	WNDCLASSEX wcex;
	// �E�C���h�E�̐ݒ�
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
	// �E�B���h�E�̃X�^�C���ɍ��킹���K�؂ȃT�C�Y���擾����
	AdjustWindowRect(&Rect, dwStyle, false);

	width = Rect.right - Rect.left;
	height = Rect.bottom - Rect.top;

	// �E�C���h�E�̐���
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
	// �E�C���h�E�̕\��
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return true;
}

// �E�B���h�E�����
void Window::Quit(void)
{
	SendMessage(m_hWnd, WM_CLOSE, 0, 0);
}

// ���b�Z�[�W�̍X�V
bool Window::UpdateMessage(void)
{
	MSG msg;
	// ���b�Z�[�W�����݂��邩�m�F
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// �I���ʒm�����Ă���ꍇ�͔�����
		if (msg.message == WM_QUIT)
		{
			m_isQuitMessage = true;
		}
		else
		{
			// ���b�Z�[�W���E�C���h�E�v���V�[�W���ɓ]��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return true;
	}
	return false;
}
