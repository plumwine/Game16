#pragma once
#include "Window.h"
#define KEY_MAX 256
//キー入力
class Input
{
public:
	Input(void);  //コンストラクタ
	~Input(void); //デスストラクタ

	//キーが押された瞬間
	bool isKeyDown(enum KEYCORD key);
	//キーが押されている間
	bool isKeyState(enum KEYCORD key);
	//キーが離された瞬間
	bool isKeyUp(enum KEYCORD key);

	void update();

private:
	// キー情報
	BYTE mCurrentKey[KEY_MAX];
	// 前のキー情報
	BYTE mPreviousKey[KEY_MAX];
	//この値との論理積で入力を取る
	const int inputNum = 0x80;
};


#pragma region DirectXInput_KEYCORD

////キーボード番号DXInput
//enum DX_KEYCORD
//{
//	ESCAPE                =  0x01,    //Escape
//	No_1                  =  0x02,    //1
//	No_2                  =  0x03,    //2
//	No_3                  =  0x04,    //3
//	No_4                  =  0x05,    //4
//	No_5                  =  0x06,    //5
//	No_6                  =  0x07,    //6
//	No_7                  =  0x08,    //7
//	No_8                  =  0x09,    //8
//	No_9                  =  0x0A,    //9
//	No_0                  =  0x0B,    //0
//	SYMBOL_MINUS          =  0x0C,    //-
//	SYMBOL_EQUALS         =  0x0D,    //=
//	BACK_SPACE            =  0x0E,    //Back Space
//	TAB                   =  0x0F,    //Tab
//	Q                     =  0x10,    //Q
//	W                     =  0x11,    //W
//	E                     =  0x12,    //E
//	R                     =  0x13,    //R
//	T                     =  0x14,    //T
//	Y                     =  0x15,    //Y
//	U                     =  0x16,    //U
//	I                     =  0x17,    //I
//	O                     =  0x18,    //O
//	P                     =  0x19,    //P
//	L_BRACKET             =  0x1A,    //[
//	R_BRACKET             =  0x1B,    //]
//	ENTER                 =  0x1C,    //Enter
//	L_CONTROL             =  0x1D,    //Ctrl
//	A                     =  0x1E,    //A
//	S                     =  0x1F,    //S
//	D                     =  0x20,    //D
//	F                     =  0x21,    //F
//	G                     =  0x22,    //G
//	H                     =  0x23,    //H
//	J                     =  0x24,    //J
//	K                     =  0x25,    //K
//	L                     =  0x26,    //L
//	SYMBOL_SEMICOKON      =  0x27,    //;
//	SYMBOL_APOSTROPHE     =  0x28,    //:
//	SYMBOL_GRAVE          =  0x29,    //わからない
//	L_SHIFT               =  0x2A,    //左Shift
//	SYMBOL_BACKSLASH      =  0x2B,    // [\]
//	Z                     =  0x2C,    //Z
//	X                     =  0x2D,    //X
//	C                     =  0x2E,    //C
//	V                     =  0x2F,    //V
//	B                     =  0x30,    //B
//	N                     =  0x31,    //N
//	M                     =  0x32,    //M
//	SYMBOL_COMMA          =  0x33,    //,
//	SYMBOL_PERIOD         =  0x34,    //.
//	SYMBOL_SLASH          =  0x35,    //[/]
//	R_SHIFT               =  0x36,    //右Shift
//	SYMBOL_MULTIPLY       =  0x37,    //*
//	L_ALT                 =  0x38,    //左Alt
//	SPACE                 =  0x39,    //Space
//	CAPTAL_LOCK           =  0x3A,    //Caps Lock
//	F1                    =  0x3B,    //F1
//	F2                    =  0x3C,    //F2
//	F3                    =  0x0D,    //F3
//	F4                    =  0x3E,    //F4
//	F5                    =  0x3F,    //F5
//	F6                    =  0x40,    //F6
//	F7                    =  0x41,    //F7
//	F8                    =  0x42,    //F8
//	F9                    =  0x43,    //F9
//	F10                   =  0x44,    //F10
//	NUM_LOCK              =  0x45,    //Num Lock
//	SCROLL_LOCK           =  0x46,    //Scroll Lock
//	NK_7                  =  0x47,    //テンキー 7
//	NK_8                  =  0x48,    //テンキー 8 
//	NK_9                  =  0x49,    //テンキー 9
//	NK_SUBTRACT           =  0x4A,    //テンキー -
//	NK_4                  =  0x4B,    //テンキー 4
//	NK_5                  =  0x4C,    //テンキー 5
//	NK_6                  =  0x4D,    //テンキー 6
//	NK_ADD                =  0x4E,    //テンキー +
//	NK_1                  =  0x4F,    //テンキー 1
//	NK_2                  =  0x50,    //テンキー 2
//	NK_3                  =  0x51,    //テンキー 3
//	NK_0                  =  0x52,    //テンキー 0
//	NK_DECIMAL            =  0x53,    //テンキー .
//	F_11                  =  0x57,    //F11
//	F_12                  =  0x58,    //F12
//	F_13                  =  0x64,    //F13        NEC PC-98
//	F_14                  =  0x65,    //F14        NEC PC-98
//	F_15                  =  0x66,    //F15        NEC PC-98
//	KANA                  =  0x70,    //カナ       日本語キーボード
//	CONVERT               =  0x79,    //変換	   日本語キーボード  
//	NOCONVERT             =  0x7B,    //無変換	   日本語キーボード 
//	SYMBOL_YEN            =  0x7D,    //[\]	       日本語キーボード
//	SYMBOL_NUMPADEQUALS   =  0x8D,    //=          NEC PC-98
//	SYMBOL_CIRCUMFLEX     =  0x90,    //^          日本語キーボード
//	SYMBOL_AT             =  0x91,    //@          NEC PC-98
//	SYMBOL_COLON          =  0x92,    //:          NEC PC-98
//	SYMBOL_UNDERLINE      =  0x93,    //-          NEC PC-98
//	KANJI                 =  0x94,    //漢字       日本語キーボード
//	STOP                  =  0x95,    //Stop       NEC PC-98
//	AX                    =  0x96,    //(JapanAX)
//	UNLABELED             =  0x97,    //(J3100)
//	NK_ENTER              =  0x9C,    //テンキー Enter
//	R_CONTROL             =  0x9D,    //右Ctrl
//	NK_COMMA              =  0xB3,    //テンキー , NEC PC-98
//	NK_DIVIDE             =  0xB5,    //テンキー /
//	SYSRQ                 =  0xB7,    //Sys Rq
//	R_ALT                 =  0xB8,    //右Alt
//	PAUSE                 =  0xC5,    //Pause
//	HOME                  =  0xC7,    //Home
//	UP                    =  0xC8,    //矢印↑
//	PAGE_UP               =  0xC9,    //Page Up
//	LEFT                  =  0xCB,    //矢印←
//	RIGHT                 =  0xCD,    //矢印→
//	END                   =  0xCF,    //End
//	DOWN                  =  0xD0,    //矢印↓
//	PAGE_NEXT             =  0xD1,    //Page Down	
//	INSERT                =  0xD2,    //Insert
//	DELETE_KEY            =  0xD3,    //Delete
//	L_WIN                 =  0xDB,    //Windows
//	R_WIN                 =  0xDC,    //Windows
//	APPS                  =  0xDD,    //Menu
//	POWER                 =  0xDE,    //Power
//	SLEEP                 =  0xDF,    //Windows
//};
#pragma endregion



#pragma region WINDOWS_KEYCORD

//キーボード番号WindowsAPI
enum KEYCORD
{
	MOUSE_L_BUTTON    =  0x01,     //マウス左クリック
	MOUSE_R_BUTTON    =  0x02,     //マウス右クリック
	MOUSE_C_BUTTON    =  0x04,     //マウスセンターボタン
	MOUSE_EX01_BUTTON =  0x05,     //マウス拡張ボタン1
	MOUSE_EX02_BUTTON =  0x06,     //マウス拡張ボタン2
	BACKSPACE         =  0x08,     //BackSpace
	TAB               =  0x09,     //Tab
	RENTER            =  0x0D,     //Enter
	SHIFT             =  0x10,     //Shift
	CONTROL           =  0x11,     //
	ALT               =  0x12,     //
	PAUSE             =  0x13,     //
	CAPITALLOCK       =  0x14,     //
	KANA              =  0x15,     //
	ESCAPE            =  0x1B,     //
	CONVERT           =  0x1C,     //
	NOCONVERT         =  0x1D,     //
	SPACE             =  0x20,     //
	ARROW_LEFT        =  0x25,     //
	ARROW_UP          =  0x26,     //
	ARROW_RIGHT       =  0x27,     //
	ARROW_DOWN        =  0x28,     //
	PRINT             =  0x2A,     //
	EXECUTE           =  0x2B,     //
	PRINT_SCREEN      =  0x2C,     //
	INSERT            =  0x2D,     //
	DELETEKEY         =  0x2E,     //
	HELP              =  0x2F,     //
	No_0              =  0x30,     //
	No_1              =  0x31,     //
	No_2              =  0x32,     //
	No_3              =  0x33,     //
	No_4              =  0x34,     //
	No_5              =  0x35,     //
	No_6              =  0x36,     //
	No_7              =  0x37,     //
	No_8              =  0x38,     //
	No_9              =  0x39,     //
	A                 =  0x41,     //
	B                 =  0x42,     //
	C                 =  0x43,     //
	D                 =  0x44,     //
	E                 =  0x45,     //
	F                 =  0x46,     //
	G                 =  0x47,     //
	H                 =  0x48,     //
	I                 =  0x49,     //
	J                 =  0x4A,     //
	K                 =  0x4B,     //
	L                 =  0x4C,     //
	M                 =  0x4D,     //
	N                 =  0x4E,     //
	O                 =  0x5F,     //
	P                 =  0x50,     //
	Q                 =  0x51,     //
	R                 =  0x52,     //
	S                 =  0x53,     //
	T                 =  0x54,     //
	U                 =  0x55,     //
	V                 =  0x56,     //
	W                 =  0x57,     //
	X                 =  0x58,     //
	Y                 =  0x59,     //
	Z                 =  0x5A,     //
	L_WIN             =  0x5B,     //
	R_WIN             =  0x5C,     //
	APPS              =  0x5D,     //
	SLEEP             =  0x5F,     //
	NK_0              =  0x60,     //
	NK_1              =  0x61,     //
	NK_2              =  0x62,     //
	NK_3              =  0x63,     //
	NK_4              =  0x64,     //
	NK_5              =  0x65,     //
	NK_6              =  0x66,     //
	NK_7              =  0x67,     //
	NK_8              =  0x68,     //
	NK_9              =  0x69,     //
	F1                =  0x70,     //
	F2                =  0x71,     //
	F3                =  0x72,     //
	F4                =  0x73,     //
	F5                =  0x74,     //
	F6                =  0x75,     //
	F7                =  0x76,     //
	F8                =  0x77,     //
	F9                =  0x78,     //
	F10               =  0x79,     //
	F11               =  0x7A,     //
	F12               =  0x7B,     //
	L_SHIFT           =  0xA0,     //
	R_SHIFT           =  0xA1,     //
	L_CONTROL         =  0xA2,     //
	R_CONTROL         =  0xA3,     //
	L_ALT             =  0xA4,     //
	R_ALT             =  0xA5,     //
	KANA_HIRA         =  0xF2,     //
	HANKAKU_ZENKAKU   =  0xF3,     //
};
#pragma endregion