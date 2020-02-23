//基礎の機能を持つshader


//グローバル変数
cbuffer global
{
	matrix g_WVP;  //ワールドから射影までの変換行列
};

//バーテックスシェーダー（頂点情報）
//セマンティクスPOSITION
float4 VS(float4 Pos:POSITION):SV_POSITION
{
	//位置をmul関数を使用してWorld座標を調整
	Pos = mul(Pos,g_WVP);
    return Pos;
}
//ピクセルシェーダー（ピクセル単位で指定範囲内をどの色で描画するか）
float4 PS(float4 Pos:SV_POSITION) :SV_Target
{
	//直接App側からピクセル情報をもらうことが出来ないので、一度バーテックスシェーダーを経由して
	//そこからもらう
	//取り敢えず白で描画
	return float4(1,1,1,1);
}