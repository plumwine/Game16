////基礎の機能を持つshader
//
//
////グローバル変数
//cbuffer global
//{
//	matrix g_WVP;  //ワールドから射影までの変換行列
//};
//
////バーテックスシェーダー（頂点情報）
////セマンティクスPOSITION
//float4 VS(float4 Pos:POSITION):SV_POSITION
//{
//	//位置をmul関数を使用してWorld座標を調整
//	Pos = mul(Pos,g_WVP);
//    return Pos;
//}
////ピクセルシェーダー（ピクセル単位で指定範囲内をどの色で描画するか）
//float4 PS(float4 Pos:SV_POSITION) :SV_Target
//{
//	//直接App側からピクセル情報をもらうことが出来ないので、一度バーテックスシェーダーを経由して
//	//そこからもらう
//	//取り敢えず白で描画
//	return float4(1,1,1,1);
//}


Texture2D g_texColor: register(t0);
SamplerState g_samLinear : register(s0);

cbuffer global
{
	matrix g_mWVP;//ワールドから射影までの変換行列
};

//構造体
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;    //UV座標が入ってくる
};

//
//バーテックスシェーダー
//
VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, g_mWVP);
	output.Tex = Tex;   //入ってきたUV座標をピクセルシェーダーに渡す

	return output;
}

//
//ピクセルシェーダー
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	//入ってきたUV座標
	return g_texColor.Sample(g_samLinear, input.Tex);
}