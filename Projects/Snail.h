#pragma once
#include "Enemy.h"
#include "Animation.h"
#include "Vec2.h"
#include "Map.h"
#include "Game.h"
#include <vector>

// 定数定義
namespace
{
	// スネイルのアニメーションに関する定数
	constexpr int kGraphSnailSizeX = 38; // スネイルのグラフィックの横サイズ (実際に描画しているのは38*2=76ピクセル)
	constexpr int kGraphSnailSizeY = 24; // スネイルのグラフィックの縦サイズ (実際に描画しているのは24*2=48ピクセル)
	constexpr int kAnimSnailMaxFrame = 10; // スネイルのアニメーションの最大フレーム数
	constexpr int kAnimSnailOneAnimationFrame = 4; // スネイルの1アニメーションフレームの時間
	constexpr int kAnimSnailScale = 2; // スネイルのアニメーションの拡大率

	// 当たり判定
	constexpr int kSnailHitBoxWidth = 48; // スネイルの当たり判定の幅
	constexpr int kSnailHitBoxHeight = 48; // スネイルの当たり判定の高さ
	//constexpr int kChipSize = 32;	// マップチップのサイズ（16x16ピクセル）

	// スネイルの移動に関する定数
	constexpr int kSnailMoveSpeed = 1; // スネイルの移動速度

	// スネイルの召喚位置
	constexpr int kSnailSummonPosY = 43; // スネイルの召喚位置Y座標
	constexpr int kSnailSummonPosLeftX = 50; // スネイルの召喚位置左端X座標
	constexpr int kSnailSummonPosRightX = Game::kScreenWidth - 50; // スネイルの召喚位置右端X座標

}

enum SnailStatus
{
	Move,	// 移動状態
	Fliped,	// ひっくり返った状態
	Dead    // 死亡状態
};

class Snail : public Enemy
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Snail();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Snail();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// リザルトシーンで使用する初期化処理
	/// </summary>
	void InitResult();

	/// <summary>
	/// 終了処理
	/// </summary>
	void End();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(const Map& map);

	/// <summary>
	/// リザルトシーンで使用する更新処理
	/// </summary>
	void UpdateResult();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// リザルトシーンで使用する描画処理
	/// </summary>
	void DrawResult();

	void MapCollision(const Map& map);

	void SnailCollision(std::vector<Snail> snails);

	void EdgeWarp() override;

	void Summon(bool isSummonLeft);

	void Attacked();

	void Dead();

	SnailStatus UpdateStatus();

	bool GetIsFliped() const { return m_isFliped; } // ひっくり返っているかどうかを取得

private:
	Animation m_nowAnim;	// 現在のアニメーション
	Animation m_moveAnim;	// 移動アニメーション
	Animation m_flipedAnim;	// ひっくり返ったアニメーション
	Animation m_deadAnim;	// 死亡アニメーション
	float m_deadRotate;
	SnailStatus m_state;

	int m_outPipeTime;	// 土管から出てくる時間をカウントする変数
	int m_flipedTime; // ひっくり返っている時間をカウントする変数
	bool m_isFliped;	// スネイルがひっくり返っているかどうか

public:
	bool m_isDelete; // スネイルが削除されるかどうか（死亡状態で削除される場合に使用）
};

