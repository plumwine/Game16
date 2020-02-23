#pragma once

#include "Singleton.h"

#include <d3dx10.h>
#include <d3dx11.h>

#pragma comment (lib,"d3dx10.lib")
#pragma comment (lib,"d3dx11.lib")


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


	void setViewEyePos(D3DXVECTOR3 eyePos);       //視点位置の変更
	void setViewLookAtPos(D3DXVECTOR3 lookAtPos); //注視点の変更
	void setViewUpVec(D3DXVECTOR3 upVecPos);      //上方位置の変更

	
	//受け取り専用
	D3D11_VIEWPORT const getViewPort();     //ビューポート取得

	D3DXMATRIX getViewMat();		  //View行列
	D3DXMATRIX getProjMat();		  //投影行列
			   
	D3DXVECTOR3 getViewEyePos();      //視点位置
	D3DXVECTOR3 getViewLookAtPos();	  //注視点
	D3DXVECTOR3 getViewUpVec();		  //上方位置
								
	D3DXMATRIX getWorldView_Pjojection(D3DXMATRIX world);  //VP(ViewPosition) 

private:

	D3D11_VIEWPORT m_ViewPort;    //ビューポート

	//行列
	D3DXMATRIX m_View;			   //View行列
	D3DXMATRIX m_Proj;			   //投影行列
								   
	//視点
	D3DXVECTOR3 m_ViewEyePos;      //視点位置  これがposition
	D3DXVECTOR3 m_ViewLookAtPos;   //注視点
	D3DXVECTOR3 m_ViewUpVec;	   //上方位置




};

