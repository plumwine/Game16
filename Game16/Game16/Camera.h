#pragma once

#include "Singleton.h"
#include <d3dx10.h>
#include <d3dx11.h>
#include <DirectXMath.h>
#include "Math/Vector3.h"

#pragma comment (lib,"d3dx10.lib")
#pragma comment (lib,"d3dx11.lib")

using namespace DirectX;
//WVP(WorldViewPosition)�Ȃǂ��g�� 
//Camera�N���X Singleton
class Camera :public Singleton<Camera>
{

protected:
	Camera();                                    //�O���ł̃C���X�^���X�쐬�͋֎~
	virtual ~Camera();


public:

	friend class Singleton<Camera>;               //Singleton�ł̃C���X�^���X�쐬�͋���

	void init();              //������      

	void update();            //�X�V
	

	void setViewEyePos(Vector3 eyePos);       //���_�ʒu�̕ύX
	void setViewLookAtPos(Vector3 lookAtPos); //�����_�̕ύX
	void setViewUpVec(Vector3 upVecPos);      //����ʒu�̕ύX

	
	//�󂯎���p
	D3D11_VIEWPORT const getViewPort();     //�r���[�|�[�g�擾

	XMMATRIX getViewMat();		  //View�s��
	XMMATRIX getProjMat();		  //���e�s��
			   
	Vector3 getViewEyePos();      //���_�ʒu
	Vector3 getViewLookAtPos();	  //�����_
	Vector3 getViewUpVec();		  //����ʒu
								
	XMMATRIX getWorldView_Pjojection(XMMATRIX world);  //VP(ViewPosition) 

private:

	D3D11_VIEWPORT m_ViewPort;    //�r���[�|�[�g

	//�s��
	XMMATRIX m_View;			   //View�s��
	XMMATRIX m_Proj;			   //���e�s��
								   
	//���_
	XMVECTOR m_ViewEyePos;      //���_�ʒu  ���ꂪposition
	XMVECTOR m_ViewLookAtPos;   //�����_
	XMVECTOR m_ViewUpVec;	   //����ʒu
	//Vector3

	XMFLOAT3 m_vViewEyePos;      //���_�ʒu  ���ꂪposition
	XMFLOAT3 m_vViewLookAtPos;   //�����_
	XMFLOAT3 m_vViewUpVec;	   //����ʒu

	XMMATRIX m_MatrixLookAtLH;
	XMMATRIX m_MatrixPerspectiveFovLH;


};

