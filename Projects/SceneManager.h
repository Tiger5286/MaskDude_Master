#pragma once
#include "SceneBase.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneResult.h"

enum SceneType
{
	NoneScene,
	TitleScene,
	GameScene,
	ResultScene
};

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Init();
	void End();
	void Update();
	void Draw();

private:

	SceneType m_sceneType;

	// 現在のシーン
	SceneBase* m_pCurrentScene;

	// シーンのインスタンス
	SceneMain m_sceneMain;
	SceneTitle m_sceneTitle;
	SceneResult m_sceneResult;

	int m_TitleBGMHandle; // タイトルBGM
	int m_GameBGMHandle; // ゲームBGM
	int m_ResultBGMHandle; // リザルトBGM
};

