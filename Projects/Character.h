#pragma once
#include "Vec2.h"
#include "Game.h"

// �萔��`
namespace
{
	constexpr float kGravity = 0.5f;	// �d��
	constexpr float kMaxFallSpeed = 10.0f;	// �ō��������x

	constexpr int kChipSize = 32;	// �}�b�v�`�b�v�̃T�C�Y�i16x16�s�N�Z���j
}

/// <summary>
/// �L�����N�^�[�̊��N���X
/// </summary>

class Character
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Character():
		m_pos(),
		m_speed(),
		m_isOnFloor(false),
		m_isAlive(true),
		m_isTurn(false)
	{ }

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Character() {};

	/// <summary>
	/// ������
	/// </summary>
	virtual void Init() {};

	/// <summary>
	/// �I������
	/// </summary>
	virtual void End() {};

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update() {};

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() {};

	/// <summary>
	/// �d�͂�K�p����
	/// </summary>
	virtual void Gravity()
	{
		// �d�͂�K�p
		if (m_speed.y < kMaxFallSpeed)
		{
			m_speed.y += kGravity;
		}
	}

	/// <summary>
	/// ��ʂ̒[�ɍs���Ɣ��Α��Ƀ��[�v���鏈��
	/// </summary>
	virtual void EdgeWarp()
	{
		// ���E�̒[�ɍs���Ɣ��Α��Ƀ��[�v
		if (m_pos.x < 0)
		{
			m_pos.x = Game::kScreenWidth;
		}
		if (m_pos.x > Game::kScreenWidth)
		{
			m_pos.x = 0;
		}
	}

	/// <summary>
	/// �ʒu��ݒ肷��
	/// </summary>
	/// <param name="pos">�ݒ肷��ʒu</param>
	virtual void SetPos(Vec2 pos)
	{
		m_pos = pos;
	}

	/// <summary>
	/// ���x��ݒ肷��
	/// </summary>
	/// <param name="speed">�ݒ肷�鑬�x</param>
	virtual void SetSpeed(Vec2 speed)
	{
		m_speed = speed;
	}

	/// <summary>
	/// �n�ʂɐڒn���Ă��邩�ǂ�����ݒ肷��
	/// </summary>
	/// <param name="isOnFloor">true:�ڒn���Ă���@false:�ڒn���Ă��Ȃ�</param>
	virtual void SetOnFloor(bool isOnFloor) { m_isOnFloor = isOnFloor; }

	/// <summary>
	/// �ʒu���擾����
	/// </summary>
	/// <returns>�ʒu</returns>
	virtual Vec2 GetPos() const { return m_pos; }

	/// <summary>
	/// ���x���擾����
	/// </summary>
	/// <returns>���x</returns>
	virtual Vec2 GetSpeed() const { return m_speed; }

	/// <summary>
	/// �n�ʐڒn��Ԃ��擾����
	/// </summary>
	virtual bool GetIsOnFloor() const { return m_isOnFloor; }

	/// <summary>
	/// ������Ԃ��擾����
	/// </summary>
	virtual bool GetIsAlive() const { return m_isAlive; }

protected:

	Vec2 m_pos;
	Vec2 m_speed;
	bool m_isOnFloor;
	bool m_isAlive;
	bool m_isTurn;
};

// �֐����̑O��virtual������Ƃ��̊֐��́u���z�֐��v�ƂȂ�A�h�������N���X�ŃI�[�o�[���[�h���邱�Ƃ��ł���悤�ɂȂ�