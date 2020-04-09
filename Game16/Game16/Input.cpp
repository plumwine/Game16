#include "Input.h"
// �R���X�g���N�^
Input::Input(void)
{
	//1�t���[���O�̃L�[���X�V
	for (int i = 0; i < 256; i++)
	{
		mPreviousKey[i] = NULL;
	}
}
// �f�X�g���N�^
Input::~Input(void)
{
}

//�L�[�������ꂽ�u��
bool Input::isKeyDown(enum KEYCORD key)
{
	return (mCurrentKey[key] & inputNum) && !(mPreviousKey[key] & inputNum);
}
//�L�[��������Ă����
bool Input::isKeyState(enum KEYCORD key)
{
	return (mCurrentKey[key] & inputNum) && (mPreviousKey[key] & inputNum);
}
//�L�[�������ꂽ�u��   �Ȃ����@�\���Ȃ�
bool Input::isKeyUp(enum KEYCORD key)
{
	return !(mCurrentKey[key] & inputNum) && (mPreviousKey[key] & inputNum);
}
//�X�V
void Input::update()
{
	//1�t���[���O�̃L�[���X�V
	for (int i = 0; i < 256; i++)
	{
		mPreviousKey[i] = mCurrentKey[i];
	}
	//���ׂẲ��z�L�[�̏�Ԃ�ێ����� 256 �o�C�g�̔z��ւ̃|�C���^���w�肷��B
	GetKeyboardState(mCurrentKey);
}
