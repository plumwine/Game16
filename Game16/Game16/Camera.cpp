#include "Camera.h"
#include "Window.h"

//�R���X�g���N�^�E����������
Camera::Camera():
	m_ViewEyePos(0.0,0.0,0.2),
	m_ViewLookAtPos(m_ViewEyePos + D3DXVECTOR3(0.0f, 0.0f, 1.0f)),
	m_ViewUpVec(0.0, 1.0, 0.0)
{
}

//�f�X�X�g���N�^
Camera::~Camera()
{

}

//������
void Camera::init()
{	
	//�r���[�|�[�g�̐ݒ�
	m_ViewPort.Width  = Window::getInstance().GetWidth();
	m_ViewPort.Height = Window::getInstance().GetHeight();
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;

	//�����Y�֌W

	//�J�������_�n��������
	D3DXMatrixLookAtLH(&m_View,&m_ViewEyePos, &m_ViewLookAtPos, &m_ViewUpVec);
	// �v���W�F�N�V�����g�����X�t�H�[��
	D3DXMatrixPerspectiveFovLH(&m_Proj, D3DX_PI / 4, (FLOAT)Window::getInstance().GetWidth() / (FLOAT)Window::getInstance().GetHeight(), 0.1f, 100.0f);


}

//�X�V
void Camera::update()
{
}
//���_�ʒu�̕ύX
void Camera::setViewEyePos(D3DXVECTOR3 eyePos)
{
	m_ViewEyePos = eyePos;
}
//�����_�̕ύX
void Camera::setViewLookAtPos(D3DXVECTOR3 lookAtPos)
{
	m_ViewLookAtPos = lookAtPos;
}
//����ʒu�̕ύX
void Camera::setViewUpVec(D3DXVECTOR3 upVecPos)
{
	m_ViewUpVec = upVecPos;
}
//�r���[�|�[�g�̎擾
D3D11_VIEWPORT const Camera::getViewPort()
{
	return m_ViewPort;
}
//View�s��
D3DXMATRIX Camera::getViewMat()
{
	return m_View;
}
//���e�s��
D3DXMATRIX Camera::getProjMat()
{
	return m_Proj;
}
//���_�ʒu
D3DXVECTOR3 Camera::getViewEyePos()
{
	return m_ViewEyePos;
}
//�����_
D3DXVECTOR3 Camera::getViewLookAtPos()
{
	return m_ViewLookAtPos;
}
//����ʒu
D3DXVECTOR3 Camera::getViewUpVec()
{
	return m_ViewUpVec;
}
//WVP(ViewPosition)�̎擾�B�����ɌX��World�s�������
D3DXMATRIX Camera::getWorldView_Pjojection(D3DXMATRIX world)
{
	return world * m_View * m_Proj;
}
