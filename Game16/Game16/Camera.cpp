#include "Camera.h"
#include "Window.h"

//コンストラクタ・初期化並び
Camera::Camera():
	m_ViewEyePos(0.0,0.0,0.2),
	m_ViewLookAtPos(m_ViewEyePos + D3DXVECTOR3(0.0f, 0.0f, 1.0f)),
	m_ViewUpVec(0.0, 1.0, 0.0)
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
	D3DXMatrixLookAtLH(&m_View,&m_ViewEyePos, &m_ViewLookAtPos, &m_ViewUpVec);
	// プロジェクショントランスフォーム
	D3DXMatrixPerspectiveFovLH(&m_Proj, D3DX_PI / 4, (FLOAT)Window::getInstance().GetWidth() / (FLOAT)Window::getInstance().GetHeight(), 0.1f, 100.0f);


}

//更新
void Camera::update()
{
}
//視点位置の変更
void Camera::setViewEyePos(D3DXVECTOR3 eyePos)
{
	m_ViewEyePos = eyePos;
}
//注視点の変更
void Camera::setViewLookAtPos(D3DXVECTOR3 lookAtPos)
{
	m_ViewLookAtPos = lookAtPos;
}
//上方位置の変更
void Camera::setViewUpVec(D3DXVECTOR3 upVecPos)
{
	m_ViewUpVec = upVecPos;
}
//ビューポートの取得
D3D11_VIEWPORT const Camera::getViewPort()
{
	return m_ViewPort;
}
//View行列
D3DXMATRIX Camera::getViewMat()
{
	return m_View;
}
//投影行列
D3DXMATRIX Camera::getProjMat()
{
	return m_Proj;
}
//視点位置
D3DXVECTOR3 Camera::getViewEyePos()
{
	return m_ViewEyePos;
}
//注視点
D3DXVECTOR3 Camera::getViewLookAtPos()
{
	return m_ViewLookAtPos;
}
//上方位置
D3DXVECTOR3 Camera::getViewUpVec()
{
	return m_ViewUpVec;
}
//WVP(ViewPosition)の取得。引数に個々のWorld行列を入れる
D3DXMATRIX Camera::getWorldView_Pjojection(D3DXMATRIX world)
{
	return world * m_View * m_Proj;
}
