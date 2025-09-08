#pragma once
#include "SceneBase.h"
#include "Map.h"
#include "Fade.h"
#include "Animation.h"


class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	~SceneTitle();

	void Init()	override;
	void End() override;
	void Update() override;
	void Draw() override;

private:
	Map m_map;
	Fade m_fade;
	Animation m_playerAnim;
	bool m_isStart;
	bool m_isInput;

	int m_titleHandle;
	int m_playerHandle;
};

