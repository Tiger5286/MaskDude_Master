#pragma once
#include <DxLib.h>
#include "Game.h"
#include "Bg.h"

class SceneBase
{
public:
	SceneBase() :
		// 背景のグラフィックハンドル
		m_terrainHandle(-1),
		m_smallPipeHandle(-1),
		m_bigPipeHandle(-1),
		// プレイヤーのグラフィックハンドル
		m_playerIdleHandle(-1),
		m_playerRunHandle(-1),
		m_playerJumpHandle(-1),
		m_playerFallHandle(-1),
		m_playerHitHandle(-1),
		m_playerDeathHandle(-1),
		m_playerSpawnHandle(-1),
		// フォントハンドル
		m_scoreFontHandle(-1),	// スコア表示用フォント
		m_titleFontHandle(-1),	// タイトル表示用フォント
		// サウンドハンドル
		// BGM
		m_titleBGMHandle(-1),	// タイトルBGM
		m_gameBGMHandle(-1),	// ゲームBGM
		m_resultBGMHandle(-1),	// リザルトBGM
		// SE
		m_JumpSEHandle(-1),	// ジャンプSE
		m_DeathSEHandle(-1),	// 死亡SE
		m_RespawnSEHandle(-1),	// リスポーンSE
		m_SnailAttackedSEHandle(-1),	// スネイルをひっくり返すSE
		m_SnailDefeatedSEHandle(-1),	// スネイル撃破SE
		m_CoinGetSEHandle(-1),	// コイン取得SE
		m_PressButtonSEHandle(-1),	// 決定SE
		m_CountDownSEHandle(-1),	// カウントダウンSE
		m_TimeUpSEHandle(-1),	// タイムアップSE

		m_frameCount(0),
		m_isSceneEnd(false),
		m_score(0)
	{
	};
	~SceneBase() {};

	/// <summary>
	/// 基底クラスの初期化
	/// </summary>
	void BaseInit()
	{
		LoadHandles();
		m_frameCount = 0;
		m_isSceneEnd = false;
		m_bg.Init();
	};

	/// <summary>
	/// 基底クラスの終了処理
	/// </summary>
	void BaseEnd()
	{
		DeleteHandles();
		m_bg.End();
	};

	virtual void Init(){};
	virtual void End(){};
	virtual void Update() {};
	virtual void Draw() {};

	void SetBGMHandles(int titleHandle, int gameHandle, int resultHandle)
	{
		m_titleBGMHandle = titleHandle;
		m_gameBGMHandle = gameHandle;
		m_resultBGMHandle = resultHandle;
		ChangeVolumeSoundMem(128, m_titleBGMHandle);
		ChangeVolumeSoundMem(128, m_gameBGMHandle);
		ChangeVolumeSoundMem(128, m_resultBGMHandle);
	};

	void LoadHandles()
	{
		// 背景
		m_terrainHandle = LoadGraph("data/Terrain.png");
		m_smallPipeHandle = LoadGraph("data/dokan.png");
		m_bigPipeHandle = LoadGraph("data/dokan2.png");

		// プレイヤー
		m_playerIdleHandle = LoadGraph("data/Player/Idle.png");
		m_playerRunHandle = LoadGraph("data/Player/Run.png");
		m_playerJumpHandle = LoadGraph("data/Player/Jump.png");
		m_playerFallHandle = LoadGraph("data/Player/Fall.png");
		m_playerHitHandle = LoadGraph("data/Player/Hit.png");
		m_playerDeathHandle = LoadGraph("data/Player/Death.png");
		m_playerSpawnHandle = LoadGraph("data/Player/Spawn.png");

		// フォント
		m_scoreFontHandle = CreateFontToHandle("HGPｺﾞｼｯｸE", 32, -1, -1,DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
		m_titleFontHandle = CreateFontToHandle("HGPｺﾞｼｯｸE", 64, -1, -1);

		// SE
		m_JumpSEHandle = LoadSoundMem("data/SE/Jump.mp3");
		m_DeathSEHandle = LoadSoundMem("data/SE/Death.mp3");
		m_RespawnSEHandle = LoadSoundMem("data/SE/Respawn.mp3");
		m_SnailAttackedSEHandle = LoadSoundMem("data/SE/Snail_Attacked.mp3");
		m_SnailDefeatedSEHandle = LoadSoundMem("data/SE/Snail_Defeated.mp3");
		m_CoinGetSEHandle = LoadSoundMem("data/SE/Coin.mp3");
		m_PressButtonSEHandle = LoadSoundMem("data/SE/PressButton.mp3");
		m_CountDownSEHandle = LoadSoundMem("data/SE/CountDown_Start.mp3");
		m_TimeUpSEHandle = LoadSoundMem("data/SE/Finish.mp3");
	}

	void DeleteHandles()
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
		// フォント
		DeleteFontToHandle(m_scoreFontHandle);
		DeleteFontToHandle(m_titleFontHandle);
		// SE
		DeleteSoundMem(m_JumpSEHandle);
		DeleteSoundMem(m_DeathSEHandle);
		DeleteSoundMem(m_RespawnSEHandle);
		DeleteSoundMem(m_SnailAttackedSEHandle);
		DeleteSoundMem(m_SnailDefeatedSEHandle);
		DeleteSoundMem(m_CoinGetSEHandle);
		DeleteSoundMem(m_PressButtonSEHandle);
		DeleteSoundMem(m_CountDownSEHandle);
		DeleteSoundMem(m_TimeUpSEHandle);
	}

	/// <summary>
	/// シーンが終了したことを検知
	/// </summary>
	/// <returns>シーンが終了した:true 終了していない:false</returns>
	bool GetSceneEnd() { return m_isSceneEnd; };
protected:
	/*グラフィックハンドル*/
	int m_terrainHandle;	// マップチップ
	int m_smallPipeHandle;	// 下の土管
	int m_bigPipeHandle;	// 上の土管
	// プレイヤー
	int m_playerIdleHandle;	// 待機
	int m_playerRunHandle;	// 走る
	int m_playerJumpHandle;	// ジャンプ
	int m_playerFallHandle;	// 落下
	int m_playerHitHandle;	// 被弾
	int m_playerDeathHandle; // 死亡
	int m_playerSpawnHandle; // スポーン

	/*フォントハンドル*/
	int m_scoreFontHandle; // スコア表示用フォント
	int m_titleFontHandle; // タイトル表示用フォント

	/*サウンドハンドル*/
	// BGM
	int m_titleBGMHandle; // タイトルBGM
	int m_gameBGMHandle; // ゲームBGM
	int m_resultBGMHandle; // リザルトBGM
	// SE
	int m_JumpSEHandle; // ジャンプSE
	int m_DeathSEHandle; // 死亡SE
	int m_RespawnSEHandle; // リスポーンSE
	int m_SnailAttackedSEHandle; // スネイルをひっくり返すSE
	int m_SnailDefeatedSEHandle; // スネイル撃破SE
	int m_CoinGetSEHandle; // コイン取得SE
	int m_PressButtonSEHandle; // 決定SE
	int m_CountDownSEHandle; // カウントダウンSE
	int m_TimeUpSEHandle; // タイムアップSE

	Bg m_bg; // 背景

	int m_frameCount;
	bool m_isSceneEnd;
	int m_score;
};