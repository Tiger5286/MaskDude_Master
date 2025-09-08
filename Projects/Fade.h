#pragma once

namespace
{
	constexpr int kFadeFrame = 60; // 60フレームでフェード完了
}

enum FadeState
{
	FadeNone,	// フェードインしていない
	FadeIn,	// フェードイン中
	FadeOut	// フェードアウト中
};

class Fade
{
public:
	Fade();
	~Fade();

	void Init();
	void End();
	void Update();
	void Draw();

	/// <summary>
	/// フェードイン、アウトを開始する
	/// </summary>
	/// <param name="isFadeIn">true:フェードイン, false:フェードアウト</param>
	void StartFade(bool isFadeIn)	
	{
		if (isFadeIn)
		{
			m_fadeState = FadeIn;
			m_fadeCount = 0;
			m_alpha = 1.0f; // フェードインは最初真っ黒
		}
		else
		{
			m_fadeState = FadeOut;
			m_fadeCount = 0;
			m_alpha = 0.0f; // フェードアウトは最初透明
		}
	}

	bool GetIsFade() const { return m_fadeState != FadeNone; } // フェード中かどうか

private:
	FadeState m_fadeState;
	int m_fadeCount;
	int m_alpha;
};

