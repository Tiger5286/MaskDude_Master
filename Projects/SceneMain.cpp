#include "SceneMain.h"
#include "Dxlib.h"

SceneMain::SceneMain() :
	SceneBase(),
	// �G�̃O���t�B�b�N�n���h��
	m_snailHandle(-1),
	// �R�C���̃O���t�B�b�N�n���h��
	m_coinHandle(-1),
	// �t�H���g�n���h��
	//m_scoreFontHandle(-1),	// �X�R�A�\���p�t�H���g
	//m_titleFontHandle(-1),	// �^�C�g���\���p�t�H���g
	m_isTouch(false),	// �v���C���[�ƓG�̓����蔻��
	m_gameTime(0),	// �Q�[�����ԁi�t���[�����j
	m_isStopGame(true),	// �Q�[����~�t���O
	m_isFadeOut(false)	// �t�F�[�h�A�E�g���t���O
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	BaseInit();
	// �O���t�B�b�N�����[�h
	LoadAllGraph();
	
	// ������
	m_map.SetHandle(m_terrainHandle, m_smallPipeHandle, m_bigPipeHandle);
	m_map.Init();

	m_player.SetSEHandle(m_JumpSEHandle, m_DeathSEHandle, m_RespawnSEHandle, m_SnailAttackedSEHandle, m_SnailDefeatedSEHandle);
	m_player.SetHandle(m_playerIdleHandle, m_playerRunHandle, m_playerJumpHandle, m_playerFallHandle, m_playerHitHandle, m_playerDeathHandle, m_playerSpawnHandle);
	m_player.Init();

	m_fade.Init();

	m_gameTime = 0; // �Q�[�����ԏ�����
	m_score = 0; // �X�R�A������

	m_isStopGame = true; // ���������̓Q�[�����~
	m_isFadeOut = false; // �t�F�[�h�A�E�g���t���O�����Z�b�g
}

void SceneMain::End()
{
	BaseEnd();
	// �O���t�B�b�N���폜
	DeleteAllGraph();

	StopSoundMem(m_gameBGMHandle); // �Q�[��BGM���~

	// �I������
	m_map.End();
	m_player.End();

	m_snails.clear(); // �X�l�C���̃��X�g���N���A
	m_coins.clear(); // �R�C���̃��X�g���N���A
}

void SceneMain::Update()
{
	m_frameCount++;

	m_bg.Update();
	m_fade.Update();

	if (m_frameCount == 240)
	{
		m_player.SetCanControl(true); // �v���C���[�̑����L����
		m_isStopGame = false; // �Q�[���J�n
	}

	if (m_isStopGame)
	{
		return; // �Q�[����~���͍X�V���Ȃ�
	}
	m_gameTime++;

	m_map.Update();
	if ((m_frameCount % 300 == 0)) // 300�t���[�����ƂɃX�l�C��������
	{
		Snail newSnail;
		newSnail.SetHandle(m_snailHandle);
		newSnail.Init();
		newSnail.Summon(GetRand(1));
		m_snails.push_back(newSnail);
	}

	if (m_frameCount % 300 == 150) // 300�t���[�����ƂɃR�C��������
	{
		Coin newCoin;
		newCoin.SetHandle(m_coinHandle);
		newCoin.Init();
		newCoin.Summon(GetRand(1));
		m_coins.push_back(newCoin);
	}

	m_player.Update();

	// Snail�̃|�C���^���X�g���쐬		// �v���C���[�Ƃ̓����蔻��Ɏg�p
	std::vector<Snail*> snailPtrs;
	for (auto& snail : m_snails)
	{
		snail.Update(m_map);
		snail.SnailCollision(m_snails); // �X�l�C�����m�̓����蔻����s��
		m_player.CheckHitSnail(snail);
		snailPtrs.push_back(&snail);
	}

	for (auto& coin : m_coins)
	{
		coin.Update(m_map); // �R�C���̍X�V
		coin.PlayerCollision(m_player); // �v���C���[�Ƃ̓����蔻����s��
	}

	// �X�l�C���̍폜����
	for (auto it = m_snails.begin(); it != m_snails.end(); ) {
		if ((*it).m_isDelete)
		{
			m_score += 100; // �X�R�A�����Z
			it = m_snails.erase(it); // �폜��̈ʒu�ɍX�V
		}
		else 
		{
			++it;
		}
	}

	// �R�C���̍폜����
	for (auto it = m_coins.begin(); it != m_coins.end(); )
	{
		if ((*it).m_isDelete)
		{
			PlaySoundMem(m_CoinGetSEHandle, DX_PLAYTYPE_BACK); // �R�C���擾SE���Đ�
			m_score += 100; // �X�R�A�����Z
			it = m_coins.erase(it); // �폜��̈ʒu�ɍX�V
		}
		else
		{
			++it;
		}
	}

	// �v���C���[�̃}�b�v����i�S�X�l�C����n���j
	m_player.MapCollision(m_map, snailPtrs);

	// �Q�[���I�[�o�[����
	if (m_gameTime == 60 * 60)
	{
		m_player.SetCanControl(false); // �v���C���[�̑���𖳌���
	}

	if (m_gameTime == 60 * 62)
	{
		m_fade.StartFade(false); // �t�F�[�h�A�E�g�J�n
	}

	if ((m_gameTime >= 60 * 62) && !m_fade.GetIsFade())
	{
		m_isSceneEnd = true; // �V�[���I��
	}

#ifdef _DEBUG
	// �f�o�b�O�p�FR�L�[�ŃQ�[���I�[�o�[
	if (CheckHitKey(KEY_INPUT_R))
	{
		m_gameTime = 60 * 59; // �Q�[���I�[�o�[�܂Ŏc��1�b�ɐݒ�
	}
	// �f�o�b�O�p�FT�L�[�ŃX�R�A2000
	if (CheckHitKey(KEY_INPUT_T))
	{
		m_score = 2000;
	}
#endif // _DEBUG
}

void SceneMain::Draw()
{
	m_bg.Draw();
	m_map.Draw();
	for (auto& snail : m_snails)
	{
		snail.Draw();
	}
	for (auto& coin : m_coins)
	{
		coin.Draw();
	}
	//m_coin.Draw(); // �R�C���̕`��
	m_map.DrawPipe();
	m_player.Draw();

	// �X�R�A��\��
	unsigned int black = GetColor(0, 0, 0);
	unsigned int white = GetColor(255,255,255);
	DrawFormatStringToHandle(Game::kScreenWidth / 2 + 15, 5, white, m_scoreFontHandle, "SCORE:%d", m_score);

	// �c�莞�Ԃ�\��
	float remainingTime = 60 - (m_gameTime / 60.0f);
	if (remainingTime < 0)
	{
		remainingTime = 0;
	}
	int timeWidth = GetDrawFormatStringWidthToHandle(m_scoreFontHandle, "TIME:%.1f", remainingTime);
	DrawFormatStringToHandle(Game::kScreenWidth / 2 - timeWidth - 15,5, white, m_scoreFontHandle, "TIME:%.1f", remainingTime);
	// �J�E���g�_�E���\��
	if (m_frameCount > 60)
	{
		if (m_frameCount < 120)
		{
			int titleWidth = GetDrawFormatStringWidthToHandle(m_titleFontHandle, "3");
			DrawFormatStringToHandle((Game::kScreenWidth - titleWidth) / 2, (Game::kScreenHeight - 64) / 2, GetColor(255, 255, 255), m_titleFontHandle, "3");
			if (m_frameCount == 61)
			{
				PlaySoundMem(m_CountDownSEHandle, DX_PLAYTYPE_BACK); // �J�E���g�_�E��SE���Đ�
			}
		}
		else if (m_frameCount < 180)
		{
			int titleWidth = GetDrawFormatStringWidthToHandle(m_titleFontHandle, "2");
			DrawFormatStringToHandle((Game::kScreenWidth - titleWidth) / 2, (Game::kScreenHeight - 64) / 2, GetColor(255, 255, 255), m_titleFontHandle, "2");
		}
		else if (m_frameCount < 240)
		{
			int titleWidth = GetDrawFormatStringWidthToHandle(m_titleFontHandle, "1");
			DrawFormatStringToHandle((Game::kScreenWidth - titleWidth) / 2, (Game::kScreenHeight - 64) / 2, GetColor(255, 255, 255), m_titleFontHandle, "1");
		}
		else if (m_frameCount < 300)
		{
			int titleWidth = GetDrawFormatStringWidthToHandle(m_titleFontHandle, "START!");
			DrawFormatStringToHandle((Game::kScreenWidth - titleWidth) / 2, (Game::kScreenHeight - 64) / 2, GetColor(255, 255, 255), m_titleFontHandle, "START!");
			if (m_frameCount == 241)
			{
				PlaySoundMem(m_gameBGMHandle, DX_PLAYTYPE_LOOP); // �Q�[��BGM�����[�v�Đ�
			}
		}
	}

	// ���Ԑ؂ꎞ�̃��b�Z�[�W�\��
	if (m_gameTime >= 60 * 60)
	{
		int titleWidth = GetDrawFormatStringWidthToHandle(m_titleFontHandle, "FINISH!");
		DrawFormatStringToHandle((Game::kScreenWidth - titleWidth) / 2, (Game::kScreenHeight - 64) / 2, GetColor(255, 255, 255), m_titleFontHandle, "FINISH!");
		if (m_gameTime == 60 * 60)
		{
			PlaySoundMem(m_TimeUpSEHandle, DX_PLAYTYPE_BACK); // �^�C���A�b�vSE���Đ�
		}
	}

	m_fade.Draw();

#ifdef _DEBUG
	// �V�[�����ƃt���[������\��(�f�o�b�O�p)
	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), "frame:%d", m_frameCount);
	// �X�R�A��\��(�f�o�b�O�p)
	DrawFormatString(0, 32, GetColor(255, 255, 255), "score:%d", m_score);
	DrawFormatString(0, 32, GetColor(255, 255, 255), "gameTime:%d", m_gameTime);
#endif // _DEBUG
}

void SceneMain::LoadAllGraph()
{
	// �G
	m_snailHandle = LoadGraph("data/Enemys/Snail.png");

	// �R�C��
	m_coinHandle = LoadGraph("data/Coin.png");
}

void SceneMain::DeleteAllGraph()
{
	// �w�i
	DeleteGraph(m_terrainHandle);
	DeleteGraph(m_smallPipeHandle);
	DeleteGraph(m_bigPipeHandle);

	// �v���C���[
	DeleteGraph(m_playerIdleHandle);
	DeleteGraph(m_playerRunHandle);
	DeleteGraph(m_playerJumpHandle);
	DeleteGraph(m_playerFallHandle);
	DeleteGraph(m_playerHitHandle);
	DeleteGraph(m_playerDeathHandle);
	DeleteGraph(m_playerSpawnHandle);

	// �G
	DeleteGraph(m_snailHandle);

	// �R�C��
	DeleteGraph(m_coinHandle);
}
