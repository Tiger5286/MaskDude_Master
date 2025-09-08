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

	// ���݂̃V�[��
	SceneBase* m_pCurrentScene;

	// �V�[���̃C���X�^���X
	SceneMain m_sceneMain;
	SceneTitle m_sceneTitle;
	SceneResult m_sceneResult;

	int m_TitleBGMHandle; // �^�C�g��BGM
	int m_GameBGMHandle; // �Q�[��BGM
	int m_ResultBGMHandle; // ���U���gBGM
};

