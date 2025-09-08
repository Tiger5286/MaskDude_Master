#pragma once
#include "Character.h"
#include "Map.h"
#include "Animation.h"
#include "Game.h"
#include "Player.h"

namespace
{
	constexpr int kCoinSize = 32;

	constexpr float kCoinMoveSpeed = 1.5f; // コインの移動速度

	constexpr int kCoinSummonY = 43; // コインの召喚位置Y座標
	constexpr int kCoinSummonLeftX = 50; // コインの召喚位置左端X座標
	constexpr int kCoinSummonRightX = Game::kScreenWidth - 50; // コインの召喚位置右端X座標
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
	Animation m_nowAnim; // 現在のアニメーション

	int m_handle;
	int m_outPipeTime; // 土管から出てくる時間をカウントする
public:
	bool m_isDelete; // コインが削除されるかどうかのフラグ
};

