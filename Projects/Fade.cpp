#include "Fade.h"
#include "DxLib.h"
#include "Game.h"

Fade::Fade():
	m_fadeState(FadeNone),
	m_alpha(0),
	m_fadeCount(0)
{
}

Fade::~Fade()
{
}

void Fade::Init()
{
	m_fadeState = FadeIn;	// 開始時はフェードイン
	m_alpha = 255;			// 初期アルファ値は不透明
	m_fadeCount = 0;
}

void Fade::End()
{

}

void Fade::Update()
{
	if (m_fadeState == FadeNone)
	{
		m_fadeCount = 0;
		m_alpha = 0;
	}
	else if (m_fadeState == FadeIn)
	{
		m_fadeCount++;
		m_alpha = 255 - (static_cast<int>(m_fadeCount) * 255 / 30);
		if (m_alpha <= 0)
		{
			m_alpha = 0;
			m_fadeState = FadeNone;
		}
	}
	else if (m_fadeState == FadeOut)
	{
		m_fadeCount++;
		m_alpha = static_cast<int>(m_fadeCount) * 255 / 30;
		if (m_alpha >= 255)
		{
			m_alpha = 255;
			m_fadeState = FadeNone;
		}
	}
}

void Fade::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}