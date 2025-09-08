#pragma once
#include "Vec2.h"
#include "Game.h"

// 定数定義
namespace
{
	constexpr float kGravity = 0.5f;	// 重力
	constexpr float kMaxFallSpeed = 10.0f;	// 最高落下速度

	constexpr int kChipSize = 32;	// マップチップのサイズ（16x16ピクセル）
}

/// <summary>
/// キャラクターの基底クラス
/// </summary>

class Character
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Character():
		m_pos(),
		m_speed(),
		m_isOnFloor(false),
		m_isAlive(true),
		m_isTurn(false)
	{ }

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Character() {};

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() {};

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void End() {};

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() {};

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() {};

	/// <summary>
	/// 重力を適用する
	/// </summary>
	virtual void Gravity()
	{
		// 重力を適用
		if (m_speed.y < kMaxFallSpeed)
		{
			m_speed.y += kGravity;
		}
	}

	/// <summary>
	/// 画面の端に行くと反対側にワープする処理
	/// </summary>
	virtual void EdgeWarp()
	{
		// 左右の端に行くと反対側にワープ
		if (m_pos.x < 0)
		{
			m_pos.x = Game::kScreenWidth;
		}
		if (m_pos.x > Game::kScreenWidth)
		{
			m_pos.x = 0;
		}
	}

	/// <summary>
	/// 位置を設定する
	/// </summary>
	/// <param name="pos">設定する位置</param>
	virtual void SetPos(Vec2 pos)
	{
		m_pos = pos;
	}

	/// <summary>
	/// 速度を設定する
	/// </summary>
	/// <param name="speed">設定する速度</param>
	virtual void SetSpeed(Vec2 speed)
	{
		m_speed = speed;
	}

	/// <summary>
	/// 地面に接地しているかどうかを設定する
	/// </summary>
	/// <param name="isOnFloor">true:接地している　false:接地していない</param>
	virtual void SetOnFloor(bool isOnFloor) { m_isOnFloor = isOnFloor; }

	/// <summary>
	/// 位置を取得する
	/// </summary>
	/// <returns>位置</returns>
	virtual Vec2 GetPos() const { return m_pos; }

	/// <summary>
	/// 速度を取得する
	/// </summary>
	/// <returns>速度</returns>
	virtual Vec2 GetSpeed() const { return m_speed; }

	/// <summary>
	/// 地面接地状態を取得する
	/// </summary>
	virtual bool GetIsOnFloor() const { return m_isOnFloor; }

	/// <summary>
	/// 生存状態を取得する
	/// </summary>
	virtual bool GetIsAlive() const { return m_isAlive; }

protected:

	Vec2 m_pos;
	Vec2 m_speed;
	bool m_isOnFloor;
	bool m_isAlive;
	bool m_isTurn;
};

// 関数名の前にvirtualをつけるとその関数は「仮想関数」となり、派生したクラスでオーバーロードすることができるようになる