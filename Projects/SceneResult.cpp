#include "SceneResult.h"

SceneResult::SceneResult() :
	m_dispScore(0),
	m_resultScore(0),
	m_isPressButton(false),
	m_snailHandle(-1)
{
}

SceneResult::~SceneResult()
{
}

void SceneResult::Init()
{
	BaseInit();
	m_fade.Init();
	m_isPressButton = false;
	m_dispScore = 0;
	PlaySoundMem(m_resultBGMHandle, DX_PLAYTYPE_LOOP); // ���U���gBGM�����[�v�Đ�

	m_snailHandle = LoadGraph("data/Enemys/Snail.png");
	m_resultSnail0.SetHandle(m_snailHandle);
	m_resultSnail1.SetHandle(m_snailHandle);
	m_resultSnail2.SetHandle(m_snailHandle);
	m_resultSnail3.SetHandle(m_snailHandle);
}

void SceneResult::End()
{
	BaseEnd();
	StopSoundMem(m_resultBGMHandle); // ���U���gBGM���~
	DeleteGraph(m_snailHandle);
}

void SceneResult::Update()
{
	m_frameCount++;
	m_bg.Update();
	m_fade.Update();
	m_resultSnail0.UpdateResult();
	m_resultSnail1.UpdateResult();
	m_resultSnail2.UpdateResult();
	m_resultSnail3.UpdateResult();

	if (m_frameCount == 1)
	{
		m_resultSnail0.InitResult();
	}
	if (m_frameCount == 16)
	{
		m_resultSnail1.InitResult();
	}
	if (m_frameCount == 31)
	{
		m_resultSnail2.InitResult();
	}
	if (m_frameCount == 46)
	{
		m_resultSnail3.InitResult();
	}


	m_dispScore += ((m_resultScore - m_dispScore) / 50);
	if ((m_dispScore > m_dispScore - 50) && (m_dispScore < m_resultScore))
	{
		m_dispScore++;
	}

	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// ������͂�����\���X�R�A�������ɓ����X�R�A�ɂ���
	if (pad & PAD_INPUT_1)
	{
		m_dispScore = m_resultScore;
	}

	// 1�b�ォ�猈����͎�t
	if (m_frameCount > 60)
	{
		if (pad & PAD_INPUT_1)
		{
			if (m_isPressButton == false)
			{
				// ����{�^���������ꂽ��t�F�[�h�J�n
				PlaySoundMem(m_PressButtonSEHandle, DX_PLAYTYPE_BACK); // ����SE���Đ�
				m_fade.StartFade(false);
			}
			m_isPressButton = true;
		}

		if (!m_fade.GetIsFade() && m_isPressButton)
		{
			// �t�F�[�h����������V�[�����I��
			m_isSceneEnd = true;
		}
	}
}

void SceneResult::Draw()
{
	m_bg.Draw();

	m_resultSnail0.DrawResult();
	m_resultSnail1.DrawResult();
	m_resultSnail2.DrawResult();
	m_resultSnail3.DrawResult();

	// �X�R�A�S�̂̕����m��
	int textWidth = GetDrawFormatStringWidthToHandle(m_titleFontHandle, "Score:%d", m_resultScore);
	// �\���p�̃X�R�A��\��
	DrawFormatStringToHandle((Game::kScreenWidth / 2 - textWidth / 2), (Game::kScreenHeight / 2 - 32), GetColor(255, 255, 255), m_titleFontHandle, "Score:%d", m_dispScore);

	// ���̕����͓_�ł�����
	if ((m_frameCount % 60) > 30)
	{
		textWidth = GetDrawFormatStringWidthToHandle(m_scoreFontHandle, "Press A to back to title");
		DrawStringToHandle((Game::kScreenWidth / 2 - textWidth / 2), (Game::kScreenHeight / 2 + 200), "Press A to back to title", GetColor(255, 255, 255), m_scoreFontHandle);
	}
	
	m_fade.Draw();
#ifdef _DEBUG

	DrawFormatString(0,0,GetColor(255,255,255),"%d",m_dispScore);

#endif
}