#pragma once
#include "SceneBase.h"
#include "Fade.h"
#include "Snail.h"

class SceneResult : public SceneBase
{
	public:

	SceneResult();
	~SceneResult();

	void Init() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void SetScore(int score) { m_resultScore = score; }

private:
	int m_dispScore;
	int m_resultScore;
	Fade m_fade;
	bool m_isPressButton;

	Snail m_resultSnail0;
	Snail m_resultSnail1;
	Snail m_resultSnail2;
	Snail m_resultSnail3;
	int m_snailHandle;
};

