#pragma once
struct Color4
{
	float  r;    //r成分
	float  g;    //g成分
	float  b;    //b成分
	float  a;    //a成分


public:
	Color4();
	Color4(float R, float G, float B, float A);

	//メンバ変数
	Color4& saturate();    //0.0～1.0の範囲にクランプ

	//単項演算子オーバーロード
	Color4 operator +() const;
	Color4 operator -() const;   //補色を求める

	//代入演算子オーバーロード
	Color4& operator +=(const Color4& c2);
	Color4& operator -=(const Color4& c2);
	Color4& operator *=(const Color4& c2);
	Color4& operator *=(float s);
	Color4& operator /=(float s);

	//2項演算子オーバーロード
	friend const Color4 operator+(const Color4& c1, const Color4& c2);
	friend const Color4 operator-(const Color4& c1, const Color4& c2);
	friend const Color4 operator*(const Color4& c1, const Color4& c2);
	friend const Color4 operator*(float s,const Color4& c);
	friend const Color4 operator*(const Color4& c, float s);
	friend const Color4 operator/(const Color4& c, float s);
};

