#include "Coin.h"
#include "Dxlib.h"

Coin::Coin():
	Character(),
	m_handle(-1),
	m_outPipeTime(0), // 土管から出てくる時間を初期化
	m_isDelete(false)
{
}

Coin::~Coin()
{
}

void Coin::Init()
{
	m_nowAnim.Init(m_handle, Vec2(kCoinSize, kCoinSize), 1, 1, 2); // アニメーションの初期化
	m_speed.x = kCoinMoveSpeed; // コインの移動速度を設定
}

void Coin::End()
{

}

void Coin::Update(const Map& map)
{
	m_outPipeTime++; // 土管から出てくる時間をカウントアップ

	//m_state = UpdateStatus(); // スネイルの状態を更新
	m_nowAnim.Update(); // アニメーションの更新

	// 土管から出てくる間は重力を適用しない(土管から出てくるように見せるため)
	if (m_outPipeTime > 35)
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
}

void Coin::Draw()
{
	m_nowAnim.Draw(m_pos, m_isTurn);
	m_nowAnim.Draw(Vec2(m_pos.x + Game::kScreenWidth, m_pos.y), m_isTurn); // 画面の右側にも描画
	m_nowAnim.Draw(Vec2(m_pos.x - Game::kScreenWidth, m_pos.y), m_isTurn); // 画面の左側にも描画
#ifdef _DEBUG
	// 当たり判定の表示（デバッグ用）
	DrawBox(m_pos.x - (kCoinSize / 2), m_pos.y - (kCoinSize / 2),
		m_pos.x + (kCoinSize / 2), m_pos.y + (kCoinSize / 2),
		GetColor(255, 0, 0), false); // 当たり判定の表示
#endif // _DEBUG
}

void Coin::MapCollision(const Map& map)
{
	Vec2 coinSize = Vec2(kCoinSize, kCoinSize);
	Vec2 hitBlockPos;

	if (map.CheckCollision(m_pos, coinSize, hitBlockPos))
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
				m_pos = (Vec2(hitBlockPos.x + kChipSize / 2 + coinSize.x / 2, m_pos.y));
			}
			else
			{
				// プレイヤーがブロックの左側にいる（右の壁に当たった）
				m_pos = (Vec2(hitBlockPos.x - kChipSize / 2 - coinSize.x / 2, m_pos.y));
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
					m_pos = (Vec2(m_pos.x, hitBlockPos.y + kChipSize / 2 + coinSize.y / 2));
					m_speed.y = 0;
				}
			}
			else
			{
				// プレイヤーがブロックの上側にいる（下から当たった・着地）
				if (m_speed.y > 0)	// 落下中なら(yの速度がプラスなら)
				{
					m_pos = (Vec2(m_pos.x, hitBlockPos.y - kChipSize / 2 - coinSize.y / 2));
					m_speed.y = 0;
					m_isOnFloor = true;  // 地面に接地
				}
			}
		}
	}
}

void Coin::PlayerCollision(const Player& player)
{
	// プレイヤーとの当たり判定を行う
	Vec2 playerPos = player.GetPos();
	float distX = m_pos.x - playerPos.x;
	float distY = m_pos.y - playerPos.y;
	bool isCollided = abs(distX) < kCoinSize && abs(distY) < kCoinSize;
	if (distX == 0 && distY == 0)
	{
		isCollided = false; // 同じ位置にいる場合は衝突しない
	}
	// コインが生存していて、プレイヤーと衝突している場合
	if (m_isAlive && isCollided)
	{
		m_isDelete = true; // コインを削除するフラグを立てる
	}
}

void Coin::EdgeWarp()
{
	if (m_pos.y > 500)	// 一番下の段にいる
	{
		bool isEnterPipe = m_pos.x < 0 || m_pos.x > Game::kScreenWidth; // 左右の端にいるかどうかをチェック
		if (isEnterPipe)
		{
			m_isTurn = !m_isTurn; // 向きを反転
			m_speed.x = -m_speed.x; // 速度を反転
			m_speed.y = 0; // Y方向の速度をリセット
			m_pos.y = kCoinSummonY; // Y座標を召喚位置に設定
			m_outPipeTime = 0; // 土管から出てくる時間をリセット
			if (m_isTurn)
			{
				m_pos.x = kCoinSummonLeftX; // 左端に位置を設定
			}
			else
			{
				m_pos.x = kCoinSummonRightX; // 右端に位置を設定
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

void Coin::Summon(bool isSummonLeft)
{
	if (isSummonLeft)
	{
		m_outPipeTime = 0; // 土管から出てくる時間をリセット
		m_pos.x = kCoinSummonLeftX; // 左端に位置を設定
		m_pos.y = kCoinSummonY; // Y座標を設定
		m_speed.x = kCoinMoveSpeed; // 右方向に移動する速度を設定
		m_isTurn = true; // 向きを右に設定
		m_isAlive = true; // 生存状態を設定
	}
	else
	{
		m_outPipeTime = 0; // 土管から出てくる時間をリセット
		m_pos.x = kCoinSummonRightX; // 右端に位置を設定
		m_pos.y = kCoinSummonY; // Y座標を設定
		m_speed.x = -kCoinMoveSpeed; // 左方向に移動する速度を設定
		m_isTurn = false; // 向きを左に設定
		m_isAlive = true; // 生存状態を設定
	}
}