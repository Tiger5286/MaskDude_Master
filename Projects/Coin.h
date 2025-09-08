#pragma once
#include "Character.h"
#include "Map.h"
#include "Animation.h"
#include "Game.h"
#include "Player.h"

namespace
{
	constexpr int kCoinSize = 32;

	constexpr float kCoinMoveSpeed = 1.5f; // �R�C���̈ړ����x

	constexpr int kCoinSummonY = 43; // �R�C���̏����ʒuY���W
	constexpr int kCoinSummonLeftX = 50; // �R�C���̏����ʒu���[X���W
	constexpr int kCoinSummonRightX = Game::kScreenWidth - 50; // �R�C���̏����ʒu�E�[X���W
}

class Coin : public Character
{
public:
	Coin();
	~Coin();

	void Init();
	void End();
	void Update(const Map& map);
	void Draw();

	void SetHandle(int handle) { m_handle = handle; }

	void MapCollision(const Map& map);

	void PlayerCollision(const Player& player);

	void EdgeWarp() override;

	void Summon(bool isSummonLeft);

private:
	Animation m_nowAnim; // ���݂̃A�j���[�V����

	int m_handle;
	int m_outPipeTime; // �y�ǂ���o�Ă��鎞�Ԃ��J�E���g����
public:
	bool m_isDelete; // �R�C�����폜����邩�ǂ����̃t���O
};

