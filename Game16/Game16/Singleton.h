#pragma once


//TemplateのT!!
//シングルトンにするテンプレート機能
//これを継承してください
template <class T>
class Singleton
{
public:

	//インスタンス情報を返す
	static inline T& getInstance()
	{
		static T instance;
		return instance;
	}


	//シングルトンのインスタンスを生成(自信のgetInstance()をラップしてるだけだが、シングルトンのインスタンス生成として明示的なものを用意するため
	inline static void createInstance()
	{
		T::getInstance();
	}


protected:

	//シングルトンのインスタンス
	static Singleton *s_pInstance;

	//隠しコンストラクタ
	Singleton() {};                            //外部でのインスタンス作成を禁止する
	virtual ~Singleton(){}

private:
	void operator=(const Singleton& obj){}     //代入禁止
	Singleton(const Singleton &obj){}          //コピーコンストラクタを禁止
};

