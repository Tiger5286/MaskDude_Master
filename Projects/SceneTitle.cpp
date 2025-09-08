#include "SceneTitle.h"
#include "Dxlib.h"

namespace
{
	// �v���C���[�̃A�j���[�V�����Ɏg���萔
	constexpr int kPlayerGraphSize = 32;
	constexpr int kPlayerAnimFrame = 11;
	constexpr int kPlayerOneAnimFrame = 3;
	constexpr int kPlayerScale = 10;
}

SceneTitle::SceneTitle():
	SceneBase(),
	m_isStart(false),
	m_isInput(false),
	m_titleHandle(-1),
	m_playerHandle(-1)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	BaseInit();

	m_isStart = false;
	m_map.SetHandle(m_terrainHandle, m_smallPipeHandle, m_bigPipeHandle);
	m_map.Init();
	m_fade.Init();
	PlaySoundMem(m_titleBGMHandle, DX_PLAYTYPE_LOOP); // �^�C�g��BGM�����[�v�Đ�
	m_titleHandle = LoadGraph("data/Title.png");
	m_playerHandle = LoadGraph("data/Player/Idle.png");
	// �v���C���[�̃A�j���[�V������������(�^�C�g���\���Ɏg�p)
	m_playerAnim.Init(m_playerHandle, Vec2(kPlayerGraphSize, kPlayerGraphSize), kPlayerAnimFrame, kPlayerOneAnimFrame, kPlayerScale);
}

void SceneTitle::End()
{
	StopSoundMem(m_titleBGMHandle); // �^�C�g��BGM���~
	BaseEnd();
	// �����������
	DeleteGraph(m_titleHandle);
	DeleteGraph(m_playerHandle);
}

void SceneTitle::Update()
{
	m_frameCount++;

	m_bg.Update();
	m_fade.Update();
	m_playerAnim.Update();

	// �R���g���[���[�̓��͂�ۑ�
	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// 1�b�ォ����͎�t
	if (m_frameCount > 60)
	{
		// A�{�^������������J�n
		if (pad & PAD_INPUT_1)
		{
			if (!m_isStart)
			{
				PlaySoundMem(m_PressButtonSEHandle, DX_PLAYTYPE_BACK); // ����SE���Đ�
				m_fade.StartFade(false);	// �t�F�[�h�A�E�g�J�n
			}
			m_isStart = true;
		}
		
		// �t�F�[�h������������V�[���ڍs
		if (!m_fade.GetIsFade() && m_isStart)
		{
			m_isSceneEnd = true;
		}
	}
}

void SceneTitle::Draw()
{
	// �w�i��`��
	m_bg.Draw();

	// �}�b�v��`��
	m_map.Draw();
	m_map.DrawPipe();

	// �^�C�g����`��
	DrawRotaGraph(Game::kScreenWidth / 5 * 3,Game::kScreenHeight / 2,0.3f,0,m_titleHandle,true);
	m_playerAnim.Draw(Vec2(Game::kScreenWidth / 3,Game::kScreenHeight / 2 - 20),false);
	if ((m_frameCount % 60) > 30)
	{
		int textWidth = GetDrawFormatStringWidthToHandle(m_scoreFontHandle, "Press A to start");
		DrawStringToHandle((Game::kScreenWidth / 2 - textWidth / 2), (Game::kScreenHeight / 2 + 200), "Press A to start", GetColor(255, 255, 255), m_scoreFontHandle);
	}

	// �t�F�[�h��`��
	m_fade.Draw();

#ifdef _DEBUG
	// �V�[�����ƃt���[������\��(�f�o�b�O�p)
	DrawFormatString(0, 0, GetColor(255, 255, 255), "SceneTitle");
	DrawFormatString(0, 16, GetColor(255, 255, 255), "frame:%d",m_frameCount);
#endif // _DEBUG
}
