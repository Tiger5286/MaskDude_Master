#include "Snail.h"
#include "Dxlib.h"
#include "Animation.h"

Snail::Snail() :
	Enemy(),
	m_deadRotate(0), // 死亡アニメーションの回転角度を初期化
	m_state(SnailStatus::Move), // 初期状態を移動状態に設定
	m_outPipeTime(0),
	m_isFliped(false), // 反転状態を初期化
	m_flipedTime(0), // 反転している時間を初期化
	m_isDelete(false) // 削除状態を初期化
{
}

Snail::~Snail()
{
}

void Snail::Init()
{
	// 初期位置を設定
	m_pos = Vec2(100, 100); // 適当な初期位置
	m_speed = Vec2(0, 0); // 初期速度
	m_isAlive = true; // 生存状態を初期化
	// アニメーションの初期化
	m_moveAnim.Init(m_handle, Vec2(kGraphSnailSizeX, kGraphSnailSizeY), kAnimSnailMaxFrame, kAnimSnailOneAnimationFrame, kAnimSnailScale); // サイズやフレーム数は適宜調整

	m_flipedAnim.Init(m_handle, Vec2(kGraphSnailSizeX, kGraphSnailSizeY), kAnimSnailMaxFrame, 1, kAnimSnailScale); // 反転アニメーションも同様に初期化
	m_flipedAnim.SetRotate(135); // 反転アニメーションの回転角度を設定

	m_deadAnim.Init(m_handle, Vec2(kGraphSnailSizeX, kGraphSnailSizeY), kAnimSnailMaxFrame, 1, kAnimSnailScale); // 死亡アニメーションの初期化

	m_nowAnim = m_moveAnim; // 初期アニメーションを移動アニメーションに設定
}

void Snail::InitResult()
{
	m_pos = Vec2(GetRand(Game::kScreenWidth), 0);
	m_deadAnim.Init(m_handle, Vec2(kGraphSnailSizeX, kGraphSnailSizeY), kAnimSnailMaxFrame, 1, kAnimSnailScale); // 死亡アニメーションの初期化
	m_nowAnim = m_deadAnim;
	Dead();
}

void Snail::End()
{
}

void Snail::Update(const Map& map)
{
	m_outPipeTime++; // 土管から出てくる時間をカウントアップ

	// ひっくり返った時間のカウント
	if (m_isFliped)
	{
		m_flipedTime++; // ひっくり返った時間をカウントアップ
	}
	else
	{
		m_flipedTime = 0; // ひっくり返っていない場合はリセット
	}

	if (m_flipedTime == 420) // ひっくり返ってから7秒経過した場合
	{
		// ひっくり返った状態から元に戻る
		Attacked();
	}

	m_nowAnim.Update(); // アニメーションの更新

	// 死亡時のアニメーションを回転させる
	if (!m_isAlive) // スネイルが死亡している場合
	{
		m_deadRotate += 0.2f;
		if (m_deadRotate > 360)
		{
			m_deadRotate = 0; // 回転角度をリセット
		}
		m_nowAnim.SetRotate(m_deadRotate); // 死亡アニメーションの回転角度を更新
	}

	// 土管から出てくる間は重力を適用しない(土管から出てくるように見せるため)
	if (m_outPipeTime > 60)
	{
		Gravity(); // 重力を適用
	}

	// スネイルの移動処理
	m_pos.x += m_speed.x; // X座標を速度に応じて更新
	m_pos.y += m_speed.y; // Y座標を速度に応じて更新
	if (m_isAlive)
	{
		EdgeWarp(); // 画面の端に行くと反対側にワープする処理
		MapCollision(map); // マップとの当たり判定
	}
	else
	{
		if (m_pos.y > Game::kScreenHeight + 100) // スネイルが画面の下に落ちた場合
		{
			m_isDelete = true;
		}
	}
}

void Snail::UpdateResult()
{
	m_nowAnim.Update(); // アニメーションの更新

	// 死亡時のアニメーションを回転させる
	if (!m_isAlive) // スネイルが死亡している場合
	{
		m_deadRotate += 0.2f;
		if (m_deadRotate > 360)
		{
			m_deadRotate = 0; // 回転角度をリセット
		}
		m_nowAnim.SetRotate(m_deadRotate); // 死亡アニメーションの回転角度を更新
	}

	Gravity(); // 重力を適用

	// スネイルの移動処理
	m_pos.x += m_speed.x; // X座標を速度に応じて更新
	m_pos.y += m_speed.y; // Y座標を速度に応じて更新

	if (m_pos.y > Game::kScreenHeight)
	{
		m_pos = Vec2(GetRand(Game::kScreenWidth), 0);
	}
}

void Snail::Draw()
{
	if (m_isFliped && (m_flipedTime > 300)) // 起き上がる直前震えさせる
	{
		if ((m_outPipeTime % 8) < 4)
		{
			m_nowAnim.Draw(Vec2(m_pos.x + 2, m_pos.y + 5), m_isTurn);
		}
		else
		{
			m_nowAnim.Draw(Vec2(m_pos.x - 2, m_pos.y + 5), m_isTurn);
		}
	}
	else if (m_isFliped)
	{
		m_nowAnim.Draw(Vec2(m_pos.x,m_pos.y+5), m_isTurn);
		m_nowAnim.Draw(Vec2(m_pos.x + Game::kScreenWidth, m_pos.y+5), m_isTurn); // 画面の右側にも描画
		m_nowAnim.Draw(Vec2(m_pos.x - Game::kScreenWidth, m_pos.y+5), m_isTurn); // 画面の左側にも描画
	}
	else
	{
		m_nowAnim.Draw(m_pos, m_isTurn);
		m_nowAnim.Draw(Vec2(m_pos.x + Game::kScreenWidth, m_pos.y), m_isTurn); // 画面の右側にも描画
		m_nowAnim.Draw(Vec2(m_pos.x - Game::kScreenWidth, m_pos.y), m_isTurn); // 画面の左側にも描画
	}

#ifdef _DEBUG
	// 当たり判定の表示（デバッグ用）
	DrawBox(m_pos.x - (kSnailHitBoxWidth / 2), m_pos.y - (kSnailHitBoxHeight / 2),
		m_pos.x + (kSnailHitBoxWidth / 2), m_pos.y + (kSnailHitBoxHeight / 2),
		GetColor(255, 0, 0), false); // 当たり判定の表示
#endif // _DEBUG
}

void Snail::DrawResult()
{
	m_nowAnim.Draw(m_pos,true);
}

void Snail::MapCollision(const Map& map)
{
	Vec2 SnailSize = Vec2(kSnailHitBoxWidth, kSnailHitBoxHeight);
	Vec2 hitBlockPos;

	if (map.CheckCollision(m_pos, SnailSize, hitBlockPos))
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
				m_pos = (Vec2(hitBlockPos.x + kChipSize / 2 + SnailSize.x / 2, m_pos.y));
			}
			else
			{
				// プレイヤーがブロックの左側にいる（右の壁に当たった）
				m_pos = (Vec2(hitBlockPos.x - kChipSize / 2 - SnailSize.x / 2, m_pos.y));
			}
		}
		else
		{
			// 縦から当たった場合（上下）
			if (dist.y > 0)
			{
				// プレイヤーがブロックの下側にいる（上から当たった）
				if (m_speed.y < 0)	// ジャンプ中なら(yの速度が速度がマイナスなら)
				{
					m_pos = (Vec2(m_pos.x, hitBlockPos.y + kChipSize / 2 + SnailSize.y / 2));
					m_speed.y = 0;
				}
			}
			else
			{
				// プレイヤーがブロックの上側にいる（下から当たった・着地）
				if (m_speed.y > 0)	// 落下中なら(yの速度がプラスなら)
				{
					m_pos = (Vec2(m_pos.x, hitBlockPos.y - kChipSize / 2 - SnailSize.y / 2));
					m_speed.y = 0;
					m_isOnFloor = true;  // 地面に接地
					if (m_isFliped)
					{
						m_speed.x = 0; // ひっくり返った状態で着地した場合、X方向の速度を0にする
					}
				}
			}
		}
	}
}




void Snail::SnailCollision(std::vector<Snail> snails)
{
	for (auto& snail : snails)
	{
		// スネイル同士の当たり判定を行う
		float distX = m_pos.x - snail.m_pos.x;
		float distY = m_pos.y - snail.m_pos.y;
		bool isCollided = abs(distX) < kSnailHitBoxWidth && abs(distY) < kSnailHitBoxHeight;
		if (distX == 0 && distY == 0)
		{
			isCollided = false; // 同じ位置にいる場合は衝突しない
		}
		// 両方のスネイルが生存しているかどうかをチェック
		bool isAlive = m_isAlive && snail.m_isAlive;
		// スネイル同士が衝突していて、両方とも生存している場合
		if (isAlive && isCollided)
		{
			if (!m_isFliped)
			{
				m_isTurn = !m_isTurn; // 向きを反転
				m_speed.x = -m_speed.x; // 速度を反転
			}
			if (abs(distX) < kSnailHitBoxWidth - 5)
			{
				if (distX < 0)
				{
					// 自分が右側
					while (abs(distX) < kSnailHitBoxWidth)
					{
						m_pos.x += 1; // 少しずつ右に移動
						distX = m_pos.x - snail.m_pos.x; // 再計算
					}
				}
				else
				{
					// 自分が左側
					while (abs(distX) < kSnailHitBoxWidth)
					{
						m_pos.x -= 1; // 少しずつ左に移動
						distX = m_pos.x - snail.m_pos.x; // 再計算
					}
				}
			}
		}
	}
}

void Snail::EdgeWarp()
{
	if (m_pos.y > 500)	// 一番下の段にいる
	{
		bool isEnterPipe = m_pos.x < 0 || m_pos.x > Game::kScreenWidth; // 左右の端にいるかどうかをチェック
		if (isEnterPipe)
		{
			m_isTurn = !m_isTurn; // 向きを反転
			m_speed.x = -m_speed.x; // 速度を反転
			m_speed.y = 0; // Y方向の速度をリセット
			m_pos.y = kSnailSummonPosY; // Y座標を召喚位置に設定
			m_outPipeTime = 0; // 土管から出てくる時間をリセット
			if (m_isTurn)
			{
				m_pos.x = kSnailSummonPosLeftX; // 左端に位置を設定
			}
			else
			{
				m_pos.x = kSnailSummonPosRightX; // 右端に位置を設定
			}
		}
	}
	else
	{
		// 左右の端に行くと反対側にワープ
		if (m_pos.x < 0)	// 画面の左端を超えた場合
		{
			m_pos.x = Game::kScreenWidth;	// 画面の右端にワープ
		}
		if (m_pos.x > Game::kScreenWidth) // 画面の右端を超えた場合
		{
			m_pos.x = 0; // 画面の左端にワープ
		}
	}
}

void Snail::Summon(bool isSummonLeft)
{
	if (isSummonLeft)
	{
		m_outPipeTime = 0; // 土管から出てくる時間をリセット
		m_pos.x = kSnailSummonPosLeftX; // 左端に位置を設定
		m_pos.y = kSnailSummonPosY; // Y座標を設定
		m_speed.x = kSnailMoveSpeed; // 右方向に移動する速度を設定
		m_isTurn = true; // 向きを右に設定
		m_isAlive = true; // 生存状態を設定
	}
	else
	{
		m_outPipeTime = 0; // 土管から出てくる時間をリセット
		m_pos.x = kSnailSummonPosRightX; // 右端に位置を設定
		m_pos.y = kSnailSummonPosY; // Y座標を設定
		m_speed.x = -kSnailMoveSpeed; // 左方向に移動する速度を設定
		m_isTurn = false; // 向きを左に設定
		m_isAlive = true; // 生存状態を設定
	}
}

void Snail::Attacked()
{
	if (m_isOnFloor)
	{
		if (m_isFliped)
		{
			m_isFliped = false; // スネイルがひっくり返っていない状態にする
			if (m_isTurn)
			{
				m_speed.x = kSnailMoveSpeed; // 左方向に移動する速度を設定
			}
			else
			{
				m_speed.x = -kSnailMoveSpeed; // 右方向に移動する速度を設定
			}
		}
		else
		{
			m_isFliped = true; // スネイルがひっくり返った状態にする
		}
		m_speed.y = -5; // 上方向に跳ね返る
		m_isOnFloor = false; // 地面から離れる
		UpdateStatus();
	}
}

void Snail::Dead()
{
	m_isAlive = false; // スネイルを死亡状態にする
	m_speed.y = -7; // 上方向に跳ね返る
	if (m_isTurn)
	{
		m_speed.x = -kSnailMoveSpeed; // 左方向に移動する速度を設定
	}
	else
	{
		m_speed.x = kSnailMoveSpeed; // 右方向に移動する速度を設定
	}
	UpdateStatus(); // 状態を更新
}

SnailStatus Snail::UpdateStatus()
{
	SnailStatus status;

	if (m_isAlive)
	{
		if (m_isFliped)
		{
			status = SnailStatus::Fliped; // ひっくり返った状態
			m_nowAnim = m_flipedAnim; // 反転アニメーションを設定
		}
		else
		{
			status = SnailStatus::Move; // 移動状態
			m_nowAnim = m_moveAnim; // 移動アニメーションを設定
		}
	}
	else
	{
		status = SnailStatus::Dead; // 死亡状態
		m_nowAnim = m_deadAnim; // 死亡アニメーションを設定
	}

	return status;
}
