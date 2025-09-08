#include "Player.h"
#include "Dxlib.h"
#include "Game.h"

Player::Player() :
	Character(),
	m_idle(-1),
	m_run(-1),
	m_jump(-1),
	m_fall(-1),
	m_hit(-1),
	m_death(-1),
	m_spawn(-1),
	m_jumpSE(-1),
	m_deathSE(-1),
	m_respawnSE(-1),
	m_snailAttackedSE(-1),
	m_snailDefeatedSE(-1),
	m_state(PlayerState::Idle),
	m_isInvincible(false),
	m_invincibleTime(0),
	m_deathTime(0),
	m_isCanControl(false)
{
}

Player::~Player()
{
}

void Player::Init()
{
	m_pos = kFirstPos;
	m_speed = Vec2(0, 0);
	m_isOnFloor = false;
	m_isAlive = true;
	m_isTurn = false;
	m_state = PlayerState::Idle;
	// アニメーションの初期化
	m_noneAnim.Init(-1, Vec2(0, 0), 0, 0, 1.0f); // 何も表示しないアニメーション
	m_idleAnim.Init(m_idle,Vec2(kGraphPlayerSize, kGraphPlayerSize), kAnimIdleMaxFrame, kAnimOneAnimationFrame, kAnimScale);
	m_runAnim.Init(m_run, Vec2(kGraphPlayerSize, kGraphPlayerSize), kAnimRunMaxFrame, kAnimOneAnimationFrame, kAnimScale);
	m_jumpAnim.Init(m_jump, Vec2(kGraphPlayerSize, kGraphPlayerSize), kAnimJumpMaxFrame, kAnimOneAnimationFrame, kAnimScale);
	m_fallAnim.Init(m_fall, Vec2(kGraphPlayerSize, kGraphPlayerSize), kAnimFallMaxFrame, kAnimOneAnimationFrame, kAnimScale);
	m_deathAnim.Init(m_death, Vec2(kGraphDeathSize, kGraphDeathSize), kAnimDeathMaxFrame, kAnimOneAnimationFrame, kAnimScale);
	m_spawnAnim.Init(m_spawn, Vec2(kGraphSpawnSize, kGraphSpawnSize), kAnimSpawnMaxFrame, kAnimOneAnimationFrame, kAnimScale);
	// 初期アニメーションを設定
	m_nowAnim = m_idleAnim;
}

void Player::End()
{
}

void Player::Update()
{

	//状態の更新(アニメーション用)
	m_state = UpdateState();

	// アニメーションの更新
	m_nowAnim.Update();

	if (m_isAlive)
	{
		m_deathTime = 0; // 死亡してからの時間をリセット

		if (m_isInvincible)
		{
			m_invincibleTime++;
			if (m_invincibleTime > 120) // 無敵時間が終了したら
			{
				m_isInvincible = false; // 無敵状態を解除
				m_invincibleTime = 0; // 無敵時間をリセット
			}
		}

		// 重力
		Gravity();

		// 落下している間はジャンプ不可
		if (m_speed.y > kGravity)
		{
			m_isOnFloor = false;
		}

		// 自然に止まる力
		if (m_speed.x > 0)
		{
			m_speed.x -= 0.1f;
		}
		if (m_speed.x < 0)
		{
			m_speed.x += 0.1f;
		}
		if (m_speed.x > -0.1f && m_speed.x < 0.1f)
		{
			m_speed.x = 0.0f;
		}

		if (m_isCanControl)  // 操作可能な場合のみ操作を受け付ける
		{
			Control(); // 入力による移動
		}
		
		// 速度をもとに次の位置を出す
		m_pos += m_speed;
	}
	else
	{
		// 死亡している場合
		m_deathTime++;
		if (m_deathTime == 21)
		{
			m_nowAnim = m_noneAnim;
			m_speed = Vec2(0, 0);
			m_pos = kFirstPos; // 最初の位置に戻る
		}
		if (m_deathTime == 60 * 2) // 2秒後に復活アニメーションを開始
		{
			PlaySoundMem(m_respawnSE, DX_PLAYTYPE_BACK);
			m_nowAnim = m_spawnAnim;
		}
		if (m_deathTime == 60 * 2 + 21)
		{
			m_isInvincible = true; // 無敵状態にする
			m_isAlive = true; // 生き返る
		}
	}

	// 左右の端に行くと反対側にワープ
	EdgeWarp();
}

void Player::Control()
{
	// 入力による移動
	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	bool isJumpB = (pad & PAD_INPUT_1);

	// ジャンプ
	if (isJumpB && m_isOnFloor)
	{
		PlaySoundMem(m_jumpSE, DX_PLAYTYPE_BACK);
		m_speed.y = kJumpPower;
		m_isOnFloor = false;
	}
	// 左右移動
	if (pad & PAD_INPUT_RIGHT && m_speed.x < kMaxMoveSpeed)
	{
		m_speed.x += kMoveSpeed;
		m_isTurn = false;
	}
	if (pad & PAD_INPUT_LEFT && m_speed.x > -kMaxMoveSpeed)
	{
		m_speed.x -= kMoveSpeed;
		m_isTurn = true;
	}
}

void Player::Draw()
{
	if ((m_invincibleTime % 2) == 0)
	{
		m_nowAnim.Draw(m_pos, m_isTurn);
		m_nowAnim.Draw(Vec2(m_pos.x + Game::kScreenWidth, m_pos.y), m_isTurn); // 画面の右側にも描画
		m_nowAnim.Draw(Vec2(m_pos.x - Game::kScreenWidth, m_pos.y), m_isTurn); // 画面の左側にも描画
	}

#ifdef _DEBUG
	// 当たり判定の表示
	DrawBox(m_pos.x - (kHitBoxWidth / 2),m_pos.y - (kHitBoxHeight / 2), m_pos.x + (kHitBoxWidth / 2), m_pos.y + (kHitBoxHeight / 2),GetColor(255,0,0),false);
	//DrawFormatString(m_pos.x, m_pos.y + 50, GetColor(255, 255, 255), "m_deathTime:%d",m_deathTime);
#endif // _DEBUG
}

// プレイヤーの状態を更新して、現在のアニメーションを設定する
PlayerState Player::UpdateState()
{
	PlayerState ans;

	if (m_isAlive)
	{
		if (m_speed.y < -1)
		{
			if (m_state != PlayerState::Jump)
			{
				m_nowAnim = m_jumpAnim;
			}
			ans = PlayerState::Jump;
		}
		else if (m_speed.y > 1)
		{
			if (m_state != PlayerState::Fall)
			{
				m_nowAnim = m_fallAnim;
			}
			ans = PlayerState::Fall;
		}
		else if (m_speed.x > 1 || m_speed.x < -1)
		{
			if (m_state != PlayerState::Run)
			{
				m_nowAnim = m_runAnim;
			}
			ans = PlayerState::Run;
		}
		else
		{
			if (m_state != PlayerState::Idle)
			{
				m_nowAnim = m_idleAnim;
			}
			ans = PlayerState::Idle;
		}
	}
	else
	{
		if (m_state != PlayerState::Death)
		{
			PlaySoundMem(m_deathSE, DX_PLAYTYPE_BACK);
			m_nowAnim = m_deathAnim;
		}
		ans = PlayerState::Death;
	}

	return ans;
}

void Player::MapCollision(const Map& map,std::vector<Snail*>& pSnails)
{
	Vec2 playerSize = Vec2(kHitBoxWidth, kHitBoxHeight);
	Vec2 hitBlockPos;

	if (map.CheckCollision(m_pos, playerSize, hitBlockPos))
	{
		// 当たり判定があった場合の処理
		Vec2 dist;
		dist.x = m_pos.x - hitBlockPos.x;
		dist.y = m_pos.y - hitBlockPos.y;

		// X方向とY方向のどちらの距離が大きいかで判定
		if (abs(dist.x) > abs(dist.y))
		{
			// 横から当たった場合（左右の壁）
			if (dist.x > 0)
			{
				// プレイヤーがブロックの右側にいる（左の壁に当たった）
				m_pos = (Vec2(hitBlockPos.x + kChipSize / 2 + playerSize.x / 2, m_pos.y));
			}
			else
			{
				// プレイヤーがブロックの左側にいる（右の壁に当たった）
				m_pos = (Vec2(hitBlockPos.x - kChipSize / 2 - playerSize.x / 2, m_pos.y));
			}
			// X方向の速度を0にする
			m_speed = (Vec2(0, m_speed.y));
		}
		else
		{
			// 縦から当たった場合（上下）
			if (dist.y > 0)
			{
				// プレイヤーがブロックの下側にいる（上から当たった）
				if (m_speed.y < 0)	// ジャンプ中なら(yの速度が速度がマイナスなら)
				{
					for (auto& snail : pSnails)
					{
						if (snail != nullptr)
						{
							CheckAttackSnail(*snail);	// スネイルに攻撃が当たったかチェック
						}
					}
					m_pos = (Vec2(m_pos.x, hitBlockPos.y + kChipSize / 2 + playerSize.y / 2));
					if (m_speed.y < -5)
					{
						m_speed.y = -5;
					}
				}
			}
			else
			{
				// プレイヤーがブロックの上側にいる（下から当たった・着地）
				if (m_speed.y > 0)	// 落下中なら(yの速度がプラスなら)
				{
					m_pos = (Vec2(m_pos.x, hitBlockPos.y - kChipSize / 2 - playerSize.y / 2));
					m_speed = (Vec2(m_speed.x, 0));
					m_isOnFloor = true;  // 地面に接地
				}
			}
		}
	}
}

void Player::CheckAttackSnail(Snail& snail)
{
	float distY = abs(m_pos.y - snail.GetPos().y);
	float distX = abs(m_pos.x - snail.GetPos().x);
	bool isAttackDistY = (distY < kAttackHitBoxHeight && distY > kAttackHitBoxHeight - kSnailHitBoxHeight);	// 攻撃判定のY方向の距離
	if (isAttackDistY && (distX < kAttackHitBoxWidth))
	{
		// スネイルに攻撃が当たった
		PlaySoundMem(m_snailAttackedSE, DX_PLAYTYPE_BACK);
		snail.Attacked();	// スネイルを攻撃する
	}
}

void Player::CheckHitSnail(Snail& snail)
{
	float distY = abs(m_pos.y - snail.GetPos().y);
	float distX = abs(m_pos.x - snail.GetPos().x);
	float hitBoxWidth = kHitBoxWidth / 2 + kSnailHitBoxWidth / 2;	// プレイヤーの当たり判定とスネイルの当たり判定の幅を合わせる
	float hitBoxHeight = kHitBoxHeight / 2 + kSnailHitBoxHeight / 2;	// プレイヤーの当たり判定とスネイルの当たり判定の高さを合わせる
	bool isHit = (distY < hitBoxHeight && distX < hitBoxWidth);
	if (isHit && snail.GetIsAlive())
	{
		// スネイルに触れた
		//printfDx("スネイルに触れた\n");
		bool isFliped = snail.GetIsFliped();
		if (isFliped)
		{
			// スネイルがひっくり返っている場合は倒す
			PlaySoundMem(m_snailDefeatedSE, DX_PLAYTYPE_BACK);
			snail.Dead();	// スネイルを倒す
		}
		else
		{
			// プレイヤーが倒される
			if (!m_isInvincible)	// 無敵状態でなければ
			{
				m_isAlive = false;
			}
		}
	}

}
