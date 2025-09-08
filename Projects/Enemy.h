#pragma once
#include "Character.h"
class Enemy : public Character
{
public:
	Enemy();
	~Enemy();

	virtual void Init() override;
	virtual void End() override;
	virtual void Update() override;
	virtual void Draw() override;

	void SetHandle(int handle) { m_handle = handle; }

protected:
	int m_handle;
};