#pragma once
#include <DxLib.h>
#include "Game.h"
#include "Bg.h"

class SceneBase
{
public:
	SceneBase() :
		// �w�i�̃O���t�B�b�N�n���h��
		m_terrainHandle(-1),
		m_smallPipeHandle(-1),
		m_bigPipeHandle(-1),
		// �v���C���[�̃O���t�B�b�N�n���h��
		m_playerIdleHandle(-1),
		m_playerRunHandle(-1),
		m_playerJumpHandle(-1),
		m_playerFallHandle(-1),
		m_playerHitHandle(-1),
		m_playerDeathHandle(-1),
		m_playerSpawnHandle(-1),
		// �t�H���g�n���h��
		m_scoreFontHandle(-1),	// �X�R�A�\���p�t�H���g
		m_titleFontHandle(-1),	// �^�C�g���\���p�t�H���g
		// �T�E���h�n���h��
		// BGM
		m_titleBGMHandle(-1),	// �^�C�g��BGM
		m_gameBGMHandle(-1),	// �Q�[��BGM
		m_resultBGMHandle(-1),	// ���U���gBGM
		// SE
		m_JumpSEHandle(-1),	// �W�����vSE
		m_DeathSEHandle(-1),	// ���SSE
		m_RespawnSEHandle(-1),	// ���X�|�[��SE
		m_SnailAttackedSEHandle(-1),	// �X�l�C�����Ђ�����Ԃ�SE
		m_SnailDefeatedSEHandle(-1),	// �X�l�C�����jSE
		m_CoinGetSEHandle(-1),	// �R�C���擾SE
		m_PressButtonSEHandle(-1),	// ����SE
		m_CountDownSEHandle(-1),	// �J�E���g�_�E��SE
		m_TimeUpSEHandle(-1),	// �^�C���A�b�vSE

		m_frameCount(0),
		m_isSceneEnd(false),
		m_score(0)
	{
	};
	~SceneBase() {};

	/// <summary>
	/// ���N���X�̏�����
	/// </summary>
	void BaseInit()
	{
		LoadHandles();
		m_frameCount = 0;
		m_isSceneEnd = false;
		m_bg.Init();
	};

	/// <summary>
	/// ���N���X�̏I������
	/// </summary>
	void BaseEnd()
	{
		DeleteHandles();
		m_bg.End();
	};

	virtual void Init(){};
	virtual void End(){};
	virtual void Update() {};
	virtual void Draw() {};

	void SetBGMHandles(int titleHandle, int gameHandle, int resultHandle)
	{
		m_titleBGMHandle = titleHandle;
		m_gameBGMHandle = gameHandle;
		m_resultBGMHandle = resultHandle;
		ChangeVolumeSoundMem(128, m_titleBGMHandle);
		ChangeVolumeSoundMem(128, m_gameBGMHandle);
		ChangeVolumeSoundMem(128, m_resultBGMHandle);
	};

	void LoadHandles()
	{
		// �w�i
		m_terrainHandle = LoadGraph("data/Terrain.png");
		m_smallPipeHandle = LoadGraph("data/dokan.png");
		m_bigPipeHandle = LoadGraph("data/dokan2.png");

		// �v���C���[
		m_playerIdleHandle = LoadGraph("data/Player/Idle.png");
		m_playerRunHandle = LoadGraph("data/Player/Run.png");
		m_playerJumpHandle = LoadGraph("data/Player/Jump.png");
		m_playerFallHandle = LoadGraph("data/Player/Fall.png");
		m_playerHitHandle = LoadGraph("data/Player/Hit.png");
		m_playerDeathHandle = LoadGraph("data/Player/Death.png");
		m_playerSpawnHandle = LoadGraph("data/Player/Spawn.png");

		// �t�H���g
		m_scoreFontHandle = CreateFontToHandle("HGP�޼��E", 32, -1, -1,DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
		m_titleFontHandle = CreateFontToHandle("HGP�޼��E", 64, -1, -1);

		// SE
		m_JumpSEHandle = LoadSoundMem("data/SE/Jump.mp3");
		m_DeathSEHandle = LoadSoundMem("data/SE/Death.mp3");
		m_RespawnSEHandle = LoadSoundMem("data/SE/Respawn.mp3");
		m_SnailAttackedSEHandle = LoadSoundMem("data/SE/Snail_Attacked.mp3");
		m_SnailDefeatedSEHandle = LoadSoundMem("data/SE/Snail_Defeated.mp3");
		m_CoinGetSEHandle = LoadSoundMem("data/SE/Coin.mp3");
		m_PressButtonSEHandle = LoadSoundMem("data/SE/PressButton.mp3");
		m_CountDownSEHandle = LoadSoundMem("data/SE/CountDown_Start.mp3");
		m_TimeUpSEHandle = LoadSoundMem("data/SE/Finish.mp3");
	}

	void DeleteHandles()
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
		// �t�H���g
		DeleteFontToHandle(m_scoreFontHandle);
		DeleteFontToHandle(m_titleFontHandle);
		// SE
		DeleteSoundMem(m_JumpSEHandle);
		DeleteSoundMem(m_DeathSEHandle);
		DeleteSoundMem(m_RespawnSEHandle);
		DeleteSoundMem(m_SnailAttackedSEHandle);
		DeleteSoundMem(m_SnailDefeatedSEHandle);
		DeleteSoundMem(m_CoinGetSEHandle);
		DeleteSoundMem(m_PressButtonSEHandle);
		DeleteSoundMem(m_CountDownSEHandle);
		DeleteSoundMem(m_TimeUpSEHandle);
	}

	/// <summary>
	/// �V�[�����I���������Ƃ����m
	/// </summary>
	/// <returns>�V�[�����I������:true �I�����Ă��Ȃ�:false</returns>
	bool GetSceneEnd() { return m_isSceneEnd; };
protected:
	/*�O���t�B�b�N�n���h��*/
	int m_terrainHandle;	// �}�b�v�`�b�v
	int m_smallPipeHandle;	// ���̓y��
	int m_bigPipeHandle;	// ��̓y��
	// �v���C���[
	int m_playerIdleHandle;	// �ҋ@
	int m_playerRunHandle;	// ����
	int m_playerJumpHandle;	// �W�����v
	int m_playerFallHandle;	// ����
	int m_playerHitHandle;	// ��e
	int m_playerDeathHandle; // ���S
	int m_playerSpawnHandle; // �X�|�[��

	/*�t�H���g�n���h��*/
	int m_scoreFontHandle; // �X�R�A�\���p�t�H���g
	int m_titleFontHandle; // �^�C�g���\���p�t�H���g

	/*�T�E���h�n���h��*/
	// BGM
	int m_titleBGMHandle; // �^�C�g��BGM
	int m_gameBGMHandle; // �Q�[��BGM
	int m_resultBGMHandle; // ���U���gBGM
	// SE
	int m_JumpSEHandle; // �W�����vSE
	int m_DeathSEHandle; // ���SSE
	int m_RespawnSEHandle; // ���X�|�[��SE
	int m_SnailAttackedSEHandle; // �X�l�C�����Ђ�����Ԃ�SE
	int m_SnailDefeatedSEHandle; // �X�l�C�����jSE
	int m_CoinGetSEHandle; // �R�C���擾SE
	int m_PressButtonSEHandle; // ����SE
	int m_CountDownSEHandle; // �J�E���g�_�E��SE
	int m_TimeUpSEHandle; // �^�C���A�b�vSE

	Bg m_bg; // �w�i

	int m_frameCount;
	bool m_isSceneEnd;
	int m_score;
};