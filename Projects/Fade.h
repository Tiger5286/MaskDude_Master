#pragma once

namespace
{
	constexpr int kFadeFrame = 60; // 60�t���[���Ńt�F�[�h����
}

enum FadeState
{
	FadeNone,	// �t�F�[�h�C�����Ă��Ȃ�
	FadeIn,	// �t�F�[�h�C����
	FadeOut	// �t�F�[�h�A�E�g��
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
	/// �t�F�[�h�C���A�A�E�g���J�n����
	/// </summary>
	/// <param name="isFadeIn">true:�t�F�[�h�C��, false:�t�F�[�h�A�E�g</param>
	void StartFade(bool isFadeIn)	
	{
		if (isFadeIn)
		{
			m_fadeState = FadeIn;
			m_fadeCount = 0;
			m_alpha = 1.0f; // �t�F�[�h�C���͍ŏ��^����
		}
		else
		{
			m_fadeState = FadeOut;
			m_fadeCount = 0;
			m_alpha = 0.0f; // �t�F�[�h�A�E�g�͍ŏ�����
		}
	}

	bool GetIsFade() const { return m_fadeState != FadeNone; } // �t�F�[�h�����ǂ���

private:
	FadeState m_fadeState;
	int m_fadeCount;
	int m_alpha;
};

