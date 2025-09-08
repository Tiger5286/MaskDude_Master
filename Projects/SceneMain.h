#pragma once
#include "SceneBase.h"
#include "Fade.h"
#include "map.h"
#include "Player.h"
#include "Snail.h"
#include "Coin.h"
#include <vector>

class SceneMain : public SceneBase
{
public:
	SceneMain();
	~SceneMain();

	void Init() override;
	void End() override;
	void Update() override;
	void Draw() override;

	int GetScore() { return m_score; }

private:
	void LoadAllGraph();
	void DeleteAllGraph();

	/*グラフィックハンドル*/
	int m_snailHandle;	// スネイルのグラフィックハンドル
	int m_coinHandle;	// コインのグラフィックハンドル

	// 当たり判定
	bool m_isTouch;	// プレイヤーがブロックに触れているか

	int m_gameTime; // ゲーム時間

	bool m_isStopGame; // ゲームを停止しているか

	bool m_isFadeOut; // フェードアウトしているか

	Fade m_fade; 

	Map m_map;
	Player m_player;
	std::vector<Coin> m_coins; // コインのリスト
	std::vector<Snail> m_snails;
};