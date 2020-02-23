#pragma once


#include "Singleton.h"
// DirectX11�̃w�b�_�[���܂߂�
#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")

// ����p�e���v���[�g�֐�
template<class T>
void SafeRelease(T*& p)
{
	if (p)
	{
		p->Release();
		p = nullptr;
	}
}


// DirectX11�N���X Singleton
class DirectX11 : public Singleton<DirectX11>
{
protected:
	DirectX11(void);                  //�O���ł̃C���X�^���X�쐬�͋֎~
	virtual ~DirectX11(void);
public:

	friend class Singleton<DirectX11>;               //Singleton�ł̃C���X�^���X�쐬�͋���


	// ������
	bool Initialize();
	// �I������
	void Finalize(void);

public:
	// �f�o�C�X�̎擾
	ID3D11Device* GetDevice(void) { return m_pDevice; }
	// �f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext* GetContext(void) { return m_pDeviceContext; }
	// �X���b�v�`�F�C���̎擾
	IDXGISwapChain* GetSwapChain(void) { return m_pSwapChain; }

private:
	ID3D11Device* m_pDevice;		        // DirectX11�̃f�o�C�X
	ID3D11DeviceContext* m_pDeviceContext;	// �`��p�̃f�o�C�X�R���e�L�X�g
	IDXGISwapChain* m_pSwapChain;		    // ��ʏo�͗p�̃X���b�v�`�F�C��
};