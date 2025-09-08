#include "SceneManager.h"

SceneManager::SceneManager() :
	m_sceneType(NoneScene),
	m_pCurrentScene(nullptr),
	m_TitleBGMHandle(-1),
	m_GameBGMHandle(-1),
	m_ResultBGMHandle(-1)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	// BGMハンドルの設定
	m_TitleBGMHandle = LoadSoundMem("data/BGM/TitleSceneBGM.mp3");
	m_GameBGMHandle = LoadSoundMem("data/BGM/GameSceneBGM.mp3");
	m_ResultBGMHandle = LoadSoundMem("data/BGM/ResultSceneBGM.mp3");

	m_sceneType = TitleScene; // 初期シーンをタイトルに設定
	m_pCurrentScene = &m_sceneTitle; // 初期シーンをタイトルに設定
	m_pCurrentScene->SetBGMHandles(m_TitleBGMHandle, m_GameBGMHandle, m_ResultBGMHandle);
	m_pCurrentScene->Init(); // 初期シーンの初期化
}

void SceneManager::End()
{
	m_pCurrentScene->End(); // 現在のシーンの終了処理
}

void SceneManager::Update()
{
	m_pCurrentScene->Update(); // 現在のシーンの更新
	// タイトルからゲームへのシーン遷移
	if ((m_sceneType == TitleScene) && (m_pCurrentScene->GetSceneEnd()))
	{
		m_pCurrentScene->End();
		m_sceneType = GameScene;
		m_pCurrentScene = &m_sceneMain;
		m_pCurrentScene->SetBGMHandles(m_TitleBGMHandle, m_GameBGMHandle, m_ResultBGMHandle);
		m_pCurrentScene->Init();
	}
	// ゲームからリザルトへのシーン遷移
	if ((m_sceneType == GameScene) && (m_pCurrentScene->GetSceneEnd()))
	{
		// スコアをリザルトシーンに渡す
		m_sceneResult.SetScore(m_sceneMain.GetScore());

		m_pCurrentScene->End();
		m_sceneType = ResultScene;
		m_pCurrentScene = &m_sceneResult;
		m_pCurrentScene->SetBGMHandles(m_TitleBGMHandle, m_GameBGMHandle, m_ResultBGMHandle);
		m_pCurrentScene->Init();
	}
	// リザルトからタイトルへのシーン遷移
	if ((m_sceneType == ResultScene) && (m_pCurrentScene->GetSceneEnd()))
	{
		m_pCurrentScene->End();
		m_sceneType = TitleScene;
		m_pCurrentScene = &m_sceneTitle;
		m_pCurrentScene->SetBGMHandles(m_TitleBGMHandle, m_GameBGMHandle, m_ResultBGMHandle);
		m_pCurrentScene->Init();
	}
}

void SceneManager::Draw()
{
	m_pCurrentScene->Draw(); // 現在のシーンの描画
}