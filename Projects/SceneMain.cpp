#include "SceneMain.h"
#include "Dxlib.h"

SceneMain::SceneMain() :
	SceneBase(),
	// 敵のグラフィックハンドル
	m_snailHandle(-1),
	// コインのグラフィックハンドル
	m_coinHandle(-1),
	// フォントハンドル
	//m_scoreFontHandle(-1),	// スコア表示用フォント
	//m_titleFontHandle(-1),	// タイトル表示用フォント
	m_isTouch(false),	// プレイヤーと敵の当たり判定
	m_gameTime(0),	// ゲーム時間（フレーム数）
	m_isStopGame(true),	// ゲーム停止フラグ
	m_isFadeOut(false)	// フェードアウト中フラグ
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	BaseInit();
	// グラフィックをロード
	LoadAllGraph();
	
	// 初期化
	m_map.SetHandle(m_terrainHandle, m_smallPipeHandle, m_bigPipeHandle);
	m_map.Init();

	m_player.SetSEHandle(m_JumpSEHandle, m_DeathSEHandle, m_RespawnSEHandle, m_SnailAttackedSEHandle, m_SnailDefeatedSEHandle);
	m_player.SetHandle(m_playerIdleHandle, m_playerRunHandle, m_playerJumpHandle, m_playerFallHandle, m_playerHitHandle, m_playerDeathHandle, m_playerSpawnHandle);
	m_player.Init();

	m_fade.Init();

	m_gameTime = 0; // ゲーム時間初期化
	m_score = 0; // スコア初期化

	m_isStopGame = true; // 初期化時はゲームを停止
	m_isFadeOut = false; // フェードアウト中フラグをリセット
}

void SceneMain::End()
{
	BaseEnd();
	// グラフィックを削除
	DeleteAllGraph();

	StopSoundMem(m_gameBGMHandle); // ゲームBGMを停止

	// 終了処理
	m_map.End();
	m_player.End();

	m_snails.clear(); // スネイルのリストをクリア
	m_coins.clear(); // コインのリストをクリア
}

void SceneMain::Update()
{
	m_frameCount++;

	m_bg.Update();
	m_fade.Update();

	if (m_frameCount == 240)
	{
		m_player.SetCanControl(true); // プレイヤーの操作を有効化
		m_isStopGame = false; // ゲーム開始
	}

	if (m_isStopGame)
	{
		return; // ゲーム停止中は更新しない
	}
	m_gameTime++;

	m_map.Update();
	if ((m_frameCount % 300 == 0)) // 300フレームごとにスネイルを召喚
	{
		Snail newSnail;
		newSnail.SetHandle(m_snailHandle);
		newSnail.Init();
		newSnail.Summon(GetRand(1));
		m_snails.push_back(newSnail);
	}

	if (m_frameCount % 300 == 150) // 300フレームごとにコインを召喚
	{
		Coin newCoin;
		newCoin.SetHandle(m_coinHandle);
		newCoin.Init();
		newCoin.Summon(GetRand(1));
		m_coins.push_back(newCoin);
	}

	m_player.Update();

	// Snailのポインタリストを作成		// プレイヤーとの当たり判定に使用
	std::vector<Snail*> snailPtrs;
	for (auto& snail : m_snails)
	{
		snail.Update(m_map);
		snail.SnailCollision(m_snails); // スネイル同士の当たり判定を行う
		m_player.CheckHitSnail(snail);
		snailPtrs.push_back(&snail);
	}

	for (auto& coin : m_coins)
	{
		coin.Update(m_map); // コインの更新
		coin.PlayerCollision(m_player); // プレイヤーとの当たり判定を行う
	}

	// スネイルの削除処理
	for (auto it = m_snails.begin(); it != m_snails.end(); ) {
		if ((*it).m_isDelete)
		{
			m_score += 100; // スコアを加算
			it = m_snails.erase(it); // 削除後の位置に更新
		}
		else 
		{
			++it;
		}
	}

	// コインの削除処理
	for (auto it = m_coins.begin(); it != m_coins.end(); )
	{
		if ((*it).m_isDelete)
		{
			PlaySoundMem(m_CoinGetSEHandle, DX_PLAYTYPE_BACK); // コイン取得SEを再生
			m_score += 100; // スコアを加算
			it = m_coins.erase(it); // 削除後の位置に更新
		}
		else
		{
			++it;
		}
	}

	// プレイヤーのマップ判定（全スネイルを渡す）
	m_player.MapCollision(m_map, snailPtrs);

	// ゲームオーバー判定
	if (m_gameTime == 60 * 60)
	{
		m_player.SetCanControl(false); // プレイヤーの操作を無効化
	}

	if (m_gameTime == 60 * 62)
	{
		m_fade.StartFade(false); // フェードアウト開始
	}

	if ((m_gameTime >= 60 * 62) && !m_fade.GetIsFade())
	{
		m_isSceneEnd = true; // シーン終了
	}

#ifdef _DEBUG
	// デバッグ用：Rキーでゲームオーバー
	if (CheckHitKey(KEY_INPUT_R))
	{
		m_gameTime = 60 * 59; // ゲームオーバーまで残り1秒に設定
	}
	// デバッグ用：Tキーでスコア2000
	if (CheckHitKey(KEY_INPUT_T))
	{
		m_score = 2000;
	}
#endif // _DEBUG
}

void SceneMain::Draw()
{
	m_bg.Draw();
	m_map.Draw();
	for (auto& snail : m_snails)
	{
		snail.Draw();
	}
	for (auto& coin : m_coins)
	{
		coin.Draw();
	}
	//m_coin.Draw(); // コインの描画
	m_map.DrawPipe();
	m_player.Draw();

	// スコアを表示
	unsigned int black = GetColor(0, 0, 0);
	unsigned int white = GetColor(255,255,255);
	DrawFormatStringToHandle(Game::kScreenWidth / 2 + 15, 5, white, m_scoreFontHandle, "SCORE:%d", m_score);

	// 残り時間を表示
	float remainingTime = 60 - (m_gameTime / 60.0f);
	if (remainingTime < 0)
	{
		remainingTime = 0;
	}
	int timeWidth = GetDrawFormatStringWidthToHandle(m_scoreFontHandle, "TIME:%.1f", remainingTime);
	DrawFormatStringToHandle(Game::kScreenWidth / 2 - timeWidth - 15,5, white, m_scoreFontHandle, "TIME:%.1f", remainingTime);
	// カウントダウン表示
	if (m_frameCount > 60)
	{
		if (m_frameCount < 120)
		{
			int titleWidth = GetDrawFormatStringWidthToHandle(m_titleFontHandle, "3");
			DrawFormatStringToHandle((Game::kScreenWidth - titleWidth) / 2, (Game::kScreenHeight - 64) / 2, GetColor(255, 255, 255), m_titleFontHandle, "3");
			if (m_frameCount == 61)
			{
				PlaySoundMem(m_CountDownSEHandle, DX_PLAYTYPE_BACK); // カウントダウンSEを再生
			}
		}
		else if (m_frameCount < 180)
		{
			int titleWidth = GetDrawFormatStringWidthToHandle(m_titleFontHandle, "2");
			DrawFormatStringToHandle((Game::kScreenWidth - titleWidth) / 2, (Game::kScreenHeight - 64) / 2, GetColor(255, 255, 255), m_titleFontHandle, "2");
		}
		else if (m_frameCount < 240)
		{
			int titleWidth = GetDrawFormatStringWidthToHandle(m_titleFontHandle, "1");
			DrawFormatStringToHandle((Game::kScreenWidth - titleWidth) / 2, (Game::kScreenHeight - 64) / 2, GetColor(255, 255, 255), m_titleFontHandle, "1");
		}
		else if (m_frameCount < 300)
		{
			int titleWidth = GetDrawFormatStringWidthToHandle(m_titleFontHandle, "START!");
			DrawFormatStringToHandle((Game::kScreenWidth - titleWidth) / 2, (Game::kScreenHeight - 64) / 2, GetColor(255, 255, 255), m_titleFontHandle, "START!");
			if (m_frameCount == 241)
			{
				PlaySoundMem(m_gameBGMHandle, DX_PLAYTYPE_LOOP); // ゲームBGMをループ再生
			}
		}
	}

	// 時間切れ時のメッセージ表示
	if (m_gameTime >= 60 * 60)
	{
		int titleWidth = GetDrawFormatStringWidthToHandle(m_titleFontHandle, "FINISH!");
		DrawFormatStringToHandle((Game::kScreenWidth - titleWidth) / 2, (Game::kScreenHeight - 64) / 2, GetColor(255, 255, 255), m_titleFontHandle, "FINISH!");
		if (m_gameTime == 60 * 60)
		{
			PlaySoundMem(m_TimeUpSEHandle, DX_PLAYTYPE_BACK); // タイムアップSEを再生
		}
	}

	m_fade.Draw();

#ifdef _DEBUG
	// シーン名とフレーム数を表示(デバッグ用)
	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), "frame:%d", m_frameCount);
	// スコアを表示(デバッグ用)
	DrawFormatString(0, 32, GetColor(255, 255, 255), "score:%d", m_score);
	DrawFormatString(0, 32, GetColor(255, 255, 255), "gameTime:%d", m_gameTime);
#endif // _DEBUG
}

void SceneMain::LoadAllGraph()
{
	// 敵
	m_snailHandle = LoadGraph("data/Enemys/Snail.png");

	// コイン
	m_coinHandle = LoadGraph("data/Coin.png");
}

void SceneMain::DeleteAllGraph()
{
	// 背景
	DeleteGraph(m_terrainHandle);
	DeleteGraph(m_smallPipeHandle);
	DeleteGraph(m_bigPipeHandle);

	// プレイヤー
	DeleteGraph(m_playerIdleHandle);
	DeleteGraph(m_playerRunHandle);
	DeleteGraph(m_playerJumpHandle);
	DeleteGraph(m_playerFallHandle);
	DeleteGraph(m_playerHitHandle);
	DeleteGraph(m_playerDeathHandle);
	DeleteGraph(m_playerSpawnHandle);

	// 敵
	DeleteGraph(m_snailHandle);

	// コイン
	DeleteGraph(m_coinHandle);
}
