#pragma once

#include <windows.h>
#include "Singleton.h"

// Window�N���X Singleton
class Window : public Singleton<Window>
{
protected: 
	Window(void);                           //�O���ł̃C���X�^���X�쐬�͋֎~
	virtual ~Window(void);
public:

	friend class Singleton<Window>;               //Singleton�ł̃C���X�^���X�쐬�͋���

	// �V���v���ȃE�B���h�E�̍쐬
	bool Create(const char* pName, int x, int y, int width, int height);
	// �E�B���h�E�����
	void Quit(void);
	// ���b�Z�[�W�̍X�V
	bool UpdateMessage(void);

public:
	// �I���ʒm�����Ă��邩�H
	bool IsQuitMessage(void) { return m_isQuitMessage; }
	// �E�B���h�E�n���h���̎擾
	HWND GetHandle(void) { return m_hWnd; }

	// �E�B���h�E�̉����擾
	int GetWidth(void) const { return m_width; }
	// �E�B���h�E�̏c���擾
	int GetHeight(void) const { return m_height; }

private:
	// �E�B���h�E�n���h��
	HWND	m_hWnd;
	// �I���ʒm�����Ă��邩�H
	bool	m_isQuitMessage;

	//�F
	HBRUSH m_hmyb;             //�w�i�F

	// �E�B���h�E�T�C�Y
	int		m_width;          //��
	int		m_height;		  //����
};
