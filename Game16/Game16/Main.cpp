#include "Window.h"
#include "Camera.h"
#include "Render2D.h"
#include <crtdbg.h>
#include "Texture2D.h"

//�֐��v���g�^�C�v�錾
void init();
void app();

// ������
void init()
{
	//�J�����̃V���O���g���C���X�^���X�쐬
	Camera::createInstance();
	//�V���O���g���̃J�����̏�����
	Camera::getInstance().init();

	//�e�N�X�`���̃V���O���g���쐬
	Texture2D::createInstance();
	Texture2D::getInstance().losdTecture((LPSTR)"Resources/Texture/sikaku_2.png", "a");
	Texture2D::getInstance().losdTecture((LPSTR)"grid.bmp", "grid");

	//�����_���[�̃V���O���g���쐬
	Render2D::createInstance();
	Render2D::getInstance().init();   //�����_�[�̏�����

}
//�A�v���P�[�V����
void app()
{
	static float a;
	if (GetKeyState('A')) { a += 0.001; }
	Render2D::getInstance().drawTexture2D("a", Vector3(a, 0, 0), Vector3(0, 0, 0), Vector3(1,1, 1),1);
	Render2D::getInstance().drawTexture2D("grid", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1),2);
	Render2D::getInstance().drawManager();
	
}
// �G���g���[�|�C���g
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//�E�B���h�E�N���X�̃V���O���g���̃C���X�^���X����
	Window::createInstance();
	//Window�̐���                               �����o���Ȃ�������I��
	if (!Window::getInstance().Create("Test", 0, 0, 860, 640)) return false;
	//DirectX11��Singleton�C���X�^���X�쐬
	DirectX11::createInstance();
	//DirectX11�̏�����                          �������ł��Ȃ�������I��
	if (!DirectX11::getInstance().Initialize()) return false;

	init();  //������
	//���b�Z�[�W�ɏI���ʒm������܂ŉ�
	while (!Window::getInstance().IsQuitMessage())
	{
		//���������[�N���o
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		if (!Window::getInstance().UpdateMessage())
		{
			app();
			DirectX11::getInstance().GetSwapChain()->Present(0, 0);//��ʍX�V  ��ʍX�V�͂����ōs��
		}
	}
	//Drectx11���I������
	DirectX11::getInstance().Finalize();

	return 0;
}
