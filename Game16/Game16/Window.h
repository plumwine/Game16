#pragma once

#include <windows.h>
#include "Singleton.h"

// Windowクラス Singleton
class Window : public Singleton<Window>
{
protected: 
	Window(void);                           //外部でのインスタンス作成は禁止
	virtual ~Window(void);
public:

	friend class Singleton<Window>;               //Singletonでのインスタンス作成は許可

	// シンプルなウィンドウの作成
	bool Create(const char* pName, int x, int y, int width, int height);
	// ウィンドウを閉じる
	void Quit(void);
	// メッセージの更新
	bool UpdateMessage(void);

public:
	// 終了通知が来ているか？
	bool IsQuitMessage(void) { return m_isQuitMessage; }
	// ウィンドウハンドルの取得
	HWND GetHandle(void) { return m_hWnd; }

	// ウィンドウの横幅取得
	int GetWidth(void) const { return m_width; }
	// ウィンドウの縦幅取得
	int GetHeight(void) const { return m_height; }

private:
	// ウィンドウハンドル
	HWND	m_hWnd;
	// 終了通知が来ているか？
	bool	m_isQuitMessage;

	//色
	HBRUSH m_hmyb;             //背景色

	// ウィンドウサイズ
	int		m_width;          //幅
	int		m_height;		  //高さ
};
