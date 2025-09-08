#include "Bg.h"
#include "Dxlib.h"
#include "Game.h"

Bg::Bg():
	m_Handle(-1)
{
}

Bg::~Bg()
{
}

void Bg::Init()
{
	SetRandomBg();
	if (m_Handle == -1)
	{
		printfDx("”wŒi‰æ‘œ‚Ì“Ç‚Ýž‚Ý‚ÉŽ¸”s‚µ‚Ü‚µ‚½B\n");
	}
}

void Bg::End()
{
	DeleteGraph(m_Handle);
}

void Bg::Update()
{
	m_offsetY += 0.5f;;
	if (m_offsetY >= 64)
	{
		m_offsetY = 0;
	}
}

void Bg::Draw()
{
	int x = Game::kScreenWidth / 64;
	int y = (Game::kScreenHeight / 64) + 1;
	for (int i = -1; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			DrawGraph(j * 64, i * 64 + m_offsetY, m_Handle, true);
		}
	}
}

void Bg::SetRandomBg()
{
	int rand = GetRand(6);
	switch (rand)
	{
	case 0:
		m_Handle = LoadGraph("data/Background/Blue.png");
		break;
	case 1:
		m_Handle = LoadGraph("data/Background/Brown.png");
		break;
	case 2:
		m_Handle = LoadGraph("data/Background/Gray.png");
		break;
	case 3:
		m_Handle = LoadGraph("data/Background/Green.png");
		break;
	case 4:
		m_Handle = LoadGraph("data/Background/Pink.png");
		break;
	case 5:
		m_Handle = LoadGraph("data/Background/Purple.png");
		break;
	case 6:
		m_Handle = LoadGraph("data/Background/Yellow.png");
		break;
	}
}
