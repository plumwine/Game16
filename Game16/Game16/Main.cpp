#include <windows.h>
#include <crtdbg.h>

//�萔��`
#define NAME "Game16"
#define WINDOW_WIDTH 640 //�E�B���h�E��
#define WINDOW_HEIGHT 480 //�E�B���h�E����

//�O���[�o���ϐ�
HWND g_hWnd = NULL;

//�֐��v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void init();   //������
void app();    //�A�v���P�[�V����


//�G���g���[�|�C���g
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	//��������񂵂��Ă΂�Ȃ�

	//�E�B���h�E�̐ݒ�
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);  //�E�B���h�E�T�C�Y�iWNDCLASSEX�^�̃T�C�Y�j������
	wc.lpfnWndProc = WndProc;//�E�B���h�E�̃��b�Z�[�W����������R�[���o�b�N�֐��ւ̃|�C���^
	wc.hInstance = hInst;    //���̃N���X�̂��߂̐U���v���W���[������C���X�^���X�n���h��
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);//�E�B���h�E�w�i�F
	wc.lpszClassName = NAME;
	//�E�B���h�E�̓o�^
	if (!RegisterClassEx(&wc)) return 1;

	//�E�B���h�E�쐬
	g_hWnd = CreateWindow(NAME, NAME, WS_OVERLAPPEDWINDOW, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInst, 0);
	init();




	ShowWindow(g_hWnd, SW_SHOW); //�E�B���h�E�\��
	UpdateWindow(g_hWnd);        //�E�B���h�E�̍X�V

	//���b�Z�[�W���[�v
	MSG msg = { 0 };

	//�����ōX�V
	//�I������܂Ŏ~�܂�Ȃ�
	while (msg.message != WM_QUIT)
	{
		//���������[�N���o
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
		}
		else
		{
			//�A�v���P�[�V�������X�V
			app();
		}
	}

	return 0;

}

//�E�B���h�E�v���W���[�̐ݒ�
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

//������
void init()
{
}

//�A�v���P�[�V����
void app()
{
}

