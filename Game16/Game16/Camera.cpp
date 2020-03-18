#include "Camera.h"
#include "Window.h"


//コンストラクタ・初期化並び
Camera::Camera():
	m_ViewEyePos(XMVectorSet(0.0,1.0,-2,0.0)),
	m_ViewLookAtPos(XMVectorSet(0.0f, 0.0f, 0.0f,0.0)),
	m_ViewUpVec(XMVectorSet(0.0, 1.0, 0.0,0.0))
{
}

//デスストラクタ
Camera::~Camera()
{

}

//初期化
void Camera::init()
{	
	//ビューポートの設定
	m_ViewPort.Width  = Window::getInstance().GetWidth();
	m_ViewPort.Height = Window::getInstance().GetHeight();
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;

	//レンズ関係

	//カメラ視点系を初期化
	m_View = XMMatrixLookAtLH(m_ViewEyePos, m_ViewLookAtPos, m_ViewUpVec);
	
	//D3DXMatrixLookAtLH(&m_View,&m_ViewEyePos, &m_ViewLookAtPos, &m_ViewUpVec);
	// プロジェクショントランスフォーム
	//D3DXMatrixPerspectiveFovLH(&m_Proj, Math::PI / 4, (FLOAT)Window::getInstance().GetWidth() / (FLOAT)Window::getInstance().GetHeight(), 0.1f, 100.0f);
	
	m_Proj = XMMatrixPerspectiveFovLH(XM_PI / 4, (FLOAT)Window::getInstance().GetWidth() / (FLOAT)Window::getInstance().GetHeight(), 0.1f, 100.0f);

}

//更新
void Camera::update()
{
}
//視点位置の変更
void Camera::setViewEyePos(Vector3 eyePos)
{
	m_ViewEyePos = XMVectorSet(eyePos.x, eyePos.y, eyePos.z,0.0);
}
//注視点の変更
void Camera::setViewLookAtPos(Vector3 lookAtPos)
{
	m_ViewLookAtPos = XMVectorSet( lookAtPos.x, lookAtPos.y, lookAtPos.z,0.0);
}
//上方位置の変更
void Camera::setViewUpVec(Vector3 upVecPos)
{
	m_ViewUpVec = XMVectorSet(upVecPos.x, upVecPos.y, upVecPos.z,0.0);
}
//ビューポートの取得
D3D11_VIEWPORT const Camera::getViewPort()
{
	return m_ViewPort;
}
//View行列
XMMATRIX Camera::getViewMat()
{
	return m_View;
}
//投影行列
XMMATRIX Camera::getProjMat()
{
	return m_Proj;
}
//視点位置
Vector3 Camera::getViewEyePos()
{
	XMStoreFloat3(&m_vViewEyePos, m_ViewEyePos);
	return Vector3(m_vViewEyePos.x, m_vViewEyePos.y, m_vViewEyePos.z);
}
//注視点
Vector3 Camera::getViewLookAtPos()
{
	XMStoreFloat3(&m_vViewLookAtPos, m_ViewLookAtPos);
	return Vector3(m_vViewLookAtPos.x, m_vViewLookAtPos.y, m_vViewLookAtPos.z);
}
//上方位置
Vector3 Camera::getViewUpVec()
{
	XMStoreFloat3(&m_vViewUpVec, m_ViewUpVec);
	return Vector3(m_vViewUpVec.x, m_vViewUpVec.y, m_vViewUpVec.z);
}
//WVP(ViewPosition)の取得。引数に個々のWorld行列を入れる
XMMATRIX Camera::getWorldView_Pjojection(XMMATRIX world)
{
	return world * m_View * m_Proj;
}
