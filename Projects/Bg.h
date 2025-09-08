#pragma once
class Bg
{
public:
	Bg();
	~Bg();

	void Init();
	void End();
	void Update();
	void Draw();

	void SetRandomBg();

private:
	int m_Handle;
	float m_offsetY;
};