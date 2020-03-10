#pragma once

#include "Singleton.h"
#include <d3dx10.h>
#include <d3dx11.h>
#include <DirectXMath.h>
#include "Math/Vector3.h"

#pragma comment (lib,"d3dx10.lib")
#pragma comment (lib,"d3dx11.lib")

using namespace DirectX;
//WVP(WorldViewPosition)などを使う 
//Cameraクラス Singleton
class Camera :public Singleton<Camera>
{

protected:
	Camera();                                    //外部でのインスタンス作成は禁止
	virtual ~Camera();


public:

	friend class Singleton<Camera>;               //Singletonでのインスタンス作成は許可

	void init();              //初期化      

	void update();            //更新
	

	void setViewEyePos(Vector3 eyePos);       //視点位置の変更
	void setViewLookAtPos(Vector3 lookAtPos); //注視点の変更
	void setViewUpVec(Vector3 upVecPos);      //上方位置の変更

	
	//受け取り専用
	D3D11_VIEWPORT const getViewPort();     //ビューポート取得

	XMMATRIX getViewMat();		  //View行列
	XMMATRIX getProjMat();		  //投影行列
			   
	Vector3 getViewEyePos();      //視点位置
	Vector3 getViewLookAtPos();	  //注視点
	Vector3 getViewUpVec();		  //上方位置
								
	XMMATRIX getWorldView_Pjojection(XMMATRIX world);  //VP(ViewPosition) 

private:

	D3D11_VIEWPORT m_ViewPort;    //ビューポート

	//行列
	XMMATRIX m_View;			   //View行列
	XMMATRIX m_Proj;			   //投影行列
								   
	//視点
	XMVECTOR m_ViewEyePos;      //視点位置  これがposition
	XMVECTOR m_ViewLookAtPos;   //注視点
	XMVECTOR m_ViewUpVec;	   //上方位置
	//Vector3

	XMFLOAT3 m_vViewEyePos;      //視点位置  これがposition
	XMFLOAT3 m_vViewLookAtPos;   //注視点
	XMFLOAT3 m_vViewUpVec;	   //上方位置

	XMMATRIX m_MatrixLookAtLH;
	XMMATRIX m_MatrixPerspectiveFovLH;


};

