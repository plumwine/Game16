#include "Camera.h"
#include "Window.h"


//�R���X�g���N�^�E����������
Camera::Camera():
	m_ViewEyePos(XMVectorSet(0.0,1.0,-2,0.0)),
	m_ViewLookAtPos(XMVectorSet(0.0f, 0.0f, 0.0f,0.0)),
	m_ViewUpVec(XMVectorSet(0.0, 1.0, 0.0,0.0))
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
	m_View = XMMatrixLookAtLH(m_ViewEyePos, m_ViewLookAtPos, m_ViewUpVec);
	
	//D3DXMatrixLookAtLH(&m_View,&m_ViewEyePos, &m_ViewLookAtPos, &m_ViewUpVec);
	// �v���W�F�N�V�����g�����X�t�H�[��
	//D3DXMatrixPerspectiveFovLH(&m_Proj, Math::PI / 4, (FLOAT)Window::getInstance().GetWidth() / (FLOAT)Window::getInstance().GetHeight(), 0.1f, 100.0f);
	
	m_Proj = XMMatrixPerspectiveFovLH(XM_PI / 4, (FLOAT)Window::getInstance().GetWidth() / (FLOAT)Window::getInstance().GetHeight(), 0.1f, 100.0f);

}

//�X�V
void Camera::update()
{
}
//���_�ʒu�̕ύX
void Camera::setViewEyePos(Vector3 eyePos)
{
	m_ViewEyePos = XMVectorSet(eyePos.x, eyePos.y, eyePos.z,0.0);
}
//�����_�̕ύX
void Camera::setViewLookAtPos(Vector3 lookAtPos)
{
	m_ViewLookAtPos = XMVectorSet( lookAtPos.x, lookAtPos.y, lookAtPos.z,0.0);
}
//����ʒu�̕ύX
void Camera::setViewUpVec(Vector3 upVecPos)
{
	m_ViewUpVec = XMVectorSet(upVecPos.x, upVecPos.y, upVecPos.z,0.0);
}
//�r���[�|�[�g�̎擾
D3D11_VIEWPORT const Camera::getViewPort()
{
	return m_ViewPort;
}
//View�s��
XMMATRIX Camera::getViewMat()
{
	return m_View;
}
//���e�s��
XMMATRIX Camera::getProjMat()
{
	return m_Proj;
}
//���_�ʒu
Vector3 Camera::getViewEyePos()
{
	XMStoreFloat3(&m_vViewEyePos, m_ViewEyePos);
	return Vector3(m_vViewEyePos.x, m_vViewEyePos.y, m_vViewEyePos.z);
}
//�����_
Vector3 Camera::getViewLookAtPos()
{
	XMStoreFloat3(&m_vViewLookAtPos, m_ViewLookAtPos);
	return Vector3(m_vViewLookAtPos.x, m_vViewLookAtPos.y, m_vViewLookAtPos.z);
}
//����ʒu
Vector3 Camera::getViewUpVec()
{
	XMStoreFloat3(&m_vViewUpVec, m_ViewUpVec);
	return Vector3(m_vViewUpVec.x, m_vViewUpVec.y, m_vViewUpVec.z);
}
//WVP(ViewPosition)�̎擾�B�����ɌX��World�s�������
XMMATRIX Camera::getWorldView_Pjojection(XMMATRIX world)
{
	return world * m_View * m_Proj;
}
