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
	// BGM�n���h���̐ݒ�
	m_TitleBGMHandle = LoadSoundMem("data/BGM/TitleSceneBGM.mp3");
	m_GameBGMHandle = LoadSoundMem("data/BGM/GameSceneBGM.mp3");
	m_ResultBGMHandle = LoadSoundMem("data/BGM/ResultSceneBGM.mp3");

	m_sceneType = TitleScene; // �����V�[�����^�C�g���ɐݒ�
	m_pCurrentScene = &m_sceneTitle; // �����V�[�����^�C�g���ɐݒ�
	m_pCurrentScene->SetBGMHandles(m_TitleBGMHandle, m_GameBGMHandle, m_ResultBGMHandle);
	m_pCurrentScene->Init(); // �����V�[���̏�����
}

void SceneManager::End()
{
	m_pCurrentScene->End(); // ���݂̃V�[���̏I������
}

void SceneManager::Update()
{
	m_pCurrentScene->Update(); // ���݂̃V�[���̍X�V
	// �^�C�g������Q�[���ւ̃V�[���J��
	if ((m_sceneType == TitleScene) && (m_pCurrentScene->GetSceneEnd()))
	{
		m_pCurrentScene->End();
		m_sceneType = GameScene;
		m_pCurrentScene = &m_sceneMain;
		m_pCurrentScene->SetBGMHandles(m_TitleBGMHandle, m_GameBGMHandle, m_ResultBGMHandle);
		m_pCurrentScene->Init();
	}
	// �Q�[�����烊�U���g�ւ̃V�[���J��
	if ((m_sceneType == GameScene) && (m_pCurrentScene->GetSceneEnd()))
	{
		// �X�R�A�����U���g�V�[���ɓn��
		m_sceneResult.SetScore(m_sceneMain.GetScore());

		m_pCurrentScene->End();
		m_sceneType = ResultScene;
		m_pCurrentScene = &m_sceneResult;
		m_pCurrentScene->SetBGMHandles(m_TitleBGMHandle, m_GameBGMHandle, m_ResultBGMHandle);
		m_pCurrentScene->Init();
	}
	// ���U���g����^�C�g���ւ̃V�[���J��
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
	m_pCurrentScene->Draw(); // ���݂̃V�[���̕`��
}