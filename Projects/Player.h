#pragma once
#include "Vec2.h"
#include "Character.h"
#include "Animation.h"
#include "Map.h"
#include "Snail.h"
#include <vector>

// 定数定義
namespace
{
	// プレイヤーのサイズ
	constexpr int kGraphPlayerSize = 32;
	constexpr int kDrawPlayerSize = kGraphPlayerSize * 2;	// 64

	// 当たり判定
	constexpr int kHitBoxWidth = 44;
	constexpr int kHitBoxHeight = 64;
	// 攻撃が当たるY軸の距離
	constexpr int kAttackHitBoxHeight = kHitBoxHeight / 2 + kChipSize + kSnailHitBoxHeight / 2;	// 64 / 2 + 32 + 48 / 2 = 64 + 16 = 80
	constexpr int kAttackHitBoxWidth = kHitBoxWidth / 2 + kSnailHitBoxWidth / 2;	// 44 / 2 + 48 / 2 = 22 + 24 = 46

	// 動かすための情報
	constexpr float kMaxMoveSpeed = 4;	// 最高移動速度
	constexpr float kMoveSpeed = 0.3f;	// 左右移動の速度
	constexpr float kJumpPower = -14;		// ジャンプ力
	//constexpr float kGravity = 0.5f;	// 重力
	//constexpr float kMaxFallSpeed = 15.0f;	// 最高落下速度

	const Vec2 kFirstPos(128, 640);

	// アニメーション関連
	constexpr int kAnimIdleMaxFrame = 11;	// 待機アニメーションのフレーム数
	constexpr int kAnimRunMaxFrame = 12;	// 走行アニメーションのフレーム数
	constexpr int kAnimJumpMaxFrame = 1;	// ジャンプアニメーションのフレーム数
	constexpr int kAnimFallMaxFrame = 1;	// 落下アニメーションのフレーム数
	constexpr int kAnimDeathMaxFrame = 7; // 死亡アニメーションのフレーム数
	constexpr int kAnimSpawnMaxFrame = 7; // スポーンアニメーションのフレーム数

	constexpr int kGraphDeathSize = 96; // 死亡アニメーションのグラフィックサイズ
	constexpr int kGraphSpawnSize = 96; // スポーンアニメーションのグラフィックサイズ

	constexpr int kAnimOneAnimationFrame = 3;	// アニメーションの1フレームの時間
	constexpr int kAnimScale = 2;	// アニメーションの拡大率
}

enum PlayerState
{
	None,	// なし(表示しない)
	Idle,	// 待機
	Run,	// 走行
	Jump,	// ジャンプ
	Fall,	// 落下
	Hit,	// 被ダメージ
	Death,	// 死亡
	Spawn,	// スポーン
};

/// <summary>
/// プレイヤークラス
/// </summary>
class Player : public Character	// Characterクラスを継承
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void End() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	void Control();	// 操作処理

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
/// マップとの当たり判定を行う
/// </summary>
/// <param name="map">マップ</param>
	void MapCollision(const Map& map,std::vector<Snail*>& pSnails);

	/// <summary>
	/// プレイヤーの状態を更新する(アニメーション用)
	/// </summary>
	/// <returns>プレイヤーの状態</returns>
	PlayerState UpdateState();

	/// <summary>
	/// グラフィックハンドルを適用
	/// </summary>
	/// <param name="idle">待機状態のグラフィックハンドル</param>
	/// <param name="run">走行状態のグラフィックハンドル</param>
	/// <param name="jump">ジャンプ中のグラフィックハンドル</param>
	/// <param name="fall">落下中のグラフィックハンドル</param>
	/// <param name="hit">被ダメージ時のグラフィックハンドル</param>
	void SetHandle(int idle, int run, int jump, int fall, int hit, int death, int spawn)
	{
		m_idle = idle;
		m_run = run;
		m_jump = jump;
		m_fall = fall;
		m_hit = hit;
		m_death = death;
		m_spawn = spawn;
	}

	void SetSEHandle(int jump, int death, int respawn, int snailAttacked, int snailDefeated)
	{
		m_jumpSE = jump;
		m_deathSE = death;
		m_respawnSE = respawn;
		m_snailAttackedSE = snailAttacked;
		m_snailDefeatedSE = snailDefeated;
	}

	void SetCanControl(bool isCan) { m_isCanControl = isCan; } // 操作可能かどうかセット

	void CheckHitSnail(Snail& snail);	// スネイルに触れたかどうか

private:
	void CheckAttackSnail(Snail& snail);	// スネイルとの当たり判定

	// グラフィックハンドル
	int m_idle;
	int m_run;
	int m_jump;
	int m_fall;
	int m_hit;
	int m_death;
	int m_spawn;

	// SEハンドル
	int m_jumpSE;	// ジャンプSE
	int m_deathSE;	// 死亡SE
	int m_respawnSE;	// リスポーンSE
	int m_snailAttackedSE;	// スネイル攻撃SE
	int m_snailDefeatedSE;	// スネイル撃破SE

	// アニメーション
	Animation m_nowAnim;	// 現在のアニメーション

	Animation m_noneAnim;	// なし(表示しない)のアニメーション
	Animation m_idleAnim;	// 待機のアニメーション
	Animation m_runAnim;	// 走行のアニメーション
	Animation m_jumpAnim;	// ジャンプのアニメーション
	Animation m_fallAnim;	// 落下のアニメーション
	Animation m_hitAnim;	// 被ダメージのアニメーション
	Animation m_deathAnim;	// 死亡のアニメーション
	Animation m_spawnAnim;	// スポーンのアニメーション

	PlayerState m_state;	// プレイヤーの状態

	bool m_isInvincible;	// 無敵状態かどうか
	int m_invincibleTime;	// 無敵状態の時間
	int m_deathTime;	//死亡後の時間
	bool m_isCanControl; // 操作可能かどうか
};

