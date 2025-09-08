#pragma once
#include "SceneBase.h"
#include "Fade.h"
#include "map.h"
#include "Player.h"
#include "Snail.h"
#include "Coin.h"
#include <vector>

class SceneMain : public SceneBase
{
public:
	SceneMain();
	~SceneMain();

	void Init() override;
	void End() override;
	void Update() override;
	void Draw() override;

	int GetScore() { return m_score; }

private:
	void LoadAllGraph();
	void DeleteAllGraph();

	/*�O���t�B�b�N�n���h��*/
	int m_snailHandle;	// �X�l�C���̃O���t�B�b�N�n���h��
	int m_coinHandle;	// �R�C���̃O���t�B�b�N�n���h��

	// �����蔻��
	bool m_isTouch;	// �v���C���[���u���b�N�ɐG��Ă��邩

	int m_gameTime; // �Q�[������

	bool m_isStopGame; // �Q�[�����~���Ă��邩

	bool m_isFadeOut; // �t�F�[�h�A�E�g���Ă��邩

	Fade m_fade; 

	Map m_map;
	Player m_player;
	std::vector<Coin> m_coins; // �R�C���̃��X�g
	std::vector<Snail> m_snails;
};