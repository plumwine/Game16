#include "Input.h"
// コンストラクタ
Input::Input(void)
{
	//1フレーム前のキーを更新
	for (int i = 0; i < 256; i++)
	{
		mPreviousKey[i] = NULL;
	}
}
// デストラクタ
Input::~Input(void)
{
}

//キーが押された瞬間
bool Input::isKeyDown(enum KEYCORD key)
{
	return (mCurrentKey[key] & inputNum) && !(mPreviousKey[key] & inputNum);
}
//キーが押されている間
bool Input::isKeyState(enum KEYCORD key)
{
	return (mCurrentKey[key] & inputNum) && (mPreviousKey[key] & inputNum);
}
//キーが離された瞬間   なぜか機能しない
bool Input::isKeyUp(enum KEYCORD key)
{
	return !(mCurrentKey[key] & inputNum) && (mPreviousKey[key] & inputNum);
}
//更新
void Input::update()
{
	//1フレーム前のキーを更新
	for (int i = 0; i < 256; i++)
	{
		mPreviousKey[i] = mCurrentKey[i];
	}
	//すべての仮想キーの状態を保持する 256 バイトの配列へのポインタを指定する。
	GetKeyboardState(mCurrentKey);
}
