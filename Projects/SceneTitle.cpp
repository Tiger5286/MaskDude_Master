#include "SceneTitle.h"
#include "Dxlib.h"

namespace
{
	// プレイヤーのアニメーションに使う定数
	constexpr int kPlayerGraphSize = 32;
	constexpr int kPlayerAnimFrame = 11;
	constexpr int kPlayerOneAnimFrame = 3;
	constexpr int kPlayerScale = 10;
}

SceneTitle::SceneTitle():
	SceneBase(),
	m_isStart(false),
	m_isInput(false),
	m_titleHandle(-1),
	m_playerHandle(-1)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	BaseInit();

	m_isStart = false;
	m_map.SetHandle(m_terrainHandle, m_smallPipeHandle, m_bigPipeHandle);
	m_map.Init();
	m_fade.Init();
	PlaySoundMem(m_titleBGMHandle, DX_PLAYTYPE_LOOP); // タイトルBGMをループ再生
	m_titleHandle = LoadGraph("data/Title.png");
	m_playerHandle = LoadGraph("data/Player/Idle.png");
	// プレイヤーのアニメーションを初期化(タイトル表示に使用)
	m_playerAnim.Init(m_playerHandle, Vec2(kPlayerGraphSize, kPlayerGraphSize), kPlayerAnimFrame, kPlayerOneAnimFrame, kPlayerScale);
}

void SceneTitle::End()
{
	StopSoundMem(m_titleBGMHandle); // タイトルBGMを停止
	BaseEnd();
	// メモリを解放
	DeleteGraph(m_titleHandle);
	DeleteGraph(m_playerHandle);
}

void SceneTitle::Update()
{
	m_frameCount++;

	m_bg.Update();
	m_fade.Update();
	m_playerAnim.Update();

	// コントローラーの入力を保存
	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// 1秒後から入力受付
	if (m_frameCount > 60)
	{
		// Aボタンを押したら開始
		if (pad & PAD_INPUT_1)
		{
			if (!m_isStart)
			{
				PlaySoundMem(m_PressButtonSEHandle, DX_PLAYTYPE_BACK); // 決定SEを再生
				m_fade.StartFade(false);	// フェードアウト開始
			}
			m_isStart = true;
		}
		
		// フェードが完了したらシーン移行
		if (!m_fade.GetIsFade() && m_isStart)
		{
			m_isSceneEnd = true;
		}
	}
}

void SceneTitle::Draw()
{
	// 背景を描画
	m_bg.Draw();

	// マップを描画
	m_map.Draw();
	m_map.DrawPipe();

	// タイトルを描画
	DrawRotaGraph(Game::kScreenWidth / 5 * 3,Game::kScreenHeight / 2,0.3f,0,m_titleHandle,true);
	m_playerAnim.Draw(Vec2(Game::kScreenWidth / 3,Game::kScreenHeight / 2 - 20),false);
	if ((m_frameCount % 60) > 30)
	{
		int textWidth = GetDrawFormatStringWidthToHandle(m_scoreFontHandle, "Press A to start");
		DrawStringToHandle((Game::kScreenWidth / 2 - textWidth / 2), (Game::kScreenHeight / 2 + 200), "Press A to start", GetColor(255, 255, 255), m_scoreFontHandle);
	}

	// フェードを描画
	m_fade.Draw();

#ifdef _DEBUG
	// シーン名とフレーム数を表示(デバッグ用)
	DrawFormatString(0, 0, GetColor(255, 255, 255), "SceneTitle");
	DrawFormatString(0, 16, GetColor(255, 255, 255), "frame:%d",m_frameCount);
#endif // _DEBUG
}
