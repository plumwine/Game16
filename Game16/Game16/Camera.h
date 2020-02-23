#pragma once

#include "Singleton.h"

#include <d3dx10.h>
#include <d3dx11.h>

#pragma comment (lib,"d3dx10.lib")
#pragma comment (lib,"d3dx11.lib")


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


	void setViewEyePos(D3DXVECTOR3 eyePos);       //���_�ʒu�̕ύX
	void setViewLookAtPos(D3DXVECTOR3 lookAtPos); //�����_�̕ύX
	void setViewUpVec(D3DXVECTOR3 upVecPos);      //����ʒu�̕ύX

	
	//�󂯎���p
	D3D11_VIEWPORT const getViewPort();     //�r���[�|�[�g�擾

	D3DXMATRIX getViewMat();		  //View�s��
	D3DXMATRIX getProjMat();		  //���e�s��
			   
	D3DXVECTOR3 getViewEyePos();      //���_�ʒu
	D3DXVECTOR3 getViewLookAtPos();	  //�����_
	D3DXVECTOR3 getViewUpVec();		  //����ʒu
								
	D3DXMATRIX getWorldView_Pjojection(D3DXMATRIX world);  //VP(ViewPosition) 

private:

	D3D11_VIEWPORT m_ViewPort;    //�r���[�|�[�g

	//�s��
	D3DXMATRIX m_View;			   //View�s��
	D3DXMATRIX m_Proj;			   //���e�s��
								   
	//���_
	D3DXVECTOR3 m_ViewEyePos;      //���_�ʒu  ���ꂪposition
	D3DXVECTOR3 m_ViewLookAtPos;   //�����_
	D3DXVECTOR3 m_ViewUpVec;	   //����ʒu




};

