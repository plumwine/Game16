#pragma once
struct Color4
{
	float  r;    //r����
	float  g;    //g����
	float  b;    //b����
	float  a;    //a����


public:
	Color4();
	Color4(float R, float G, float B, float A);

	//�����o�ϐ�
	Color4& saturate();    //0.0�`1.0�͈̔͂ɃN�����v

	//�P�����Z�q�I�[�o�[���[�h
	Color4 operator +() const;
	Color4 operator -() const;   //��F�����߂�

	//������Z�q�I�[�o�[���[�h
	Color4& operator +=(const Color4& c2);
	Color4& operator -=(const Color4& c2);
	Color4& operator *=(const Color4& c2);
	Color4& operator *=(float s);
	Color4& operator /=(float s);

	//2�����Z�q�I�[�o�[���[�h
	friend const Color4 operator+(const Color4& c1, const Color4& c2);
	friend const Color4 operator-(const Color4& c1, const Color4& c2);
	friend const Color4 operator*(const Color4& c1, const Color4& c2);
	friend const Color4 operator*(float s,const Color4& c);
	friend const Color4 operator*(const Color4& c, float s);
	friend const Color4 operator/(const Color4& c, float s);
};

