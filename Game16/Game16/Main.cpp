
#include "Window.h"
#include "Camera.h"
#include "Render2D.h"
#include <crtdbg.h>

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
	//�����_���[�̃V���O���g���쐬
	Render2D::createInstance();
	Render2D::getInstance().init();   //�����_�[�̏�����
}
//�A�v���P�[�V����
void app()
{
	Render2D::getInstance().Draw();
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
		}
	}
	//Drectx11���I������
	DirectX11::getInstance().Finalize();

	return 0;
}
