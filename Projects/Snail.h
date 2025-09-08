#pragma once
#include "Enemy.h"
#include "Animation.h"
#include "Vec2.h"
#include "Map.h"
#include "Game.h"
#include <vector>

// �萔��`
namespace
{
	// �X�l�C���̃A�j���[�V�����Ɋւ���萔
	constexpr int kGraphSnailSizeX = 38; // �X�l�C���̃O���t�B�b�N�̉��T�C�Y (���ۂɕ`�悵�Ă���̂�38*2=76�s�N�Z��)
	constexpr int kGraphSnailSizeY = 24; // �X�l�C���̃O���t�B�b�N�̏c�T�C�Y (���ۂɕ`�悵�Ă���̂�24*2=48�s�N�Z��)
	constexpr int kAnimSnailMaxFrame = 10; // �X�l�C���̃A�j���[�V�����̍ő�t���[����
	constexpr int kAnimSnailOneAnimationFrame = 4; // �X�l�C����1�A�j���[�V�����t���[���̎���
	constexpr int kAnimSnailScale = 2; // �X�l�C���̃A�j���[�V�����̊g�嗦

	// �����蔻��
	constexpr int kSnailHitBoxWidth = 48; // �X�l�C���̓����蔻��̕�
	constexpr int kSnailHitBoxHeight = 48; // �X�l�C���̓����蔻��̍���
	//constexpr int kChipSize = 32;	// �}�b�v�`�b�v�̃T�C�Y�i16x16�s�N�Z���j

	// �X�l�C���̈ړ��Ɋւ���萔
	constexpr int kSnailMoveSpeed = 1; // �X�l�C���̈ړ����x

	// �X�l�C���̏����ʒu
	constexpr int kSnailSummonPosY = 43; // �X�l�C���̏����ʒuY���W
	constexpr int kSnailSummonPosLeftX = 50; // �X�l�C���̏����ʒu���[X���W
	constexpr int kSnailSummonPosRightX = Game::kScreenWidth - 50; // �X�l�C���̏����ʒu�E�[X���W

}

enum SnailStatus
{
	Move,	// �ړ����
	Fliped,	// �Ђ�����Ԃ������
	Dead    // ���S���
};

class Snail : public Enemy
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Snail();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Snail();

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// ���U���g�V�[���Ŏg�p���鏉��������
	/// </summary>
	void InitResult();

	/// <summary>
	/// �I������
	/// </summary>
	void End();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(const Map& map);

	/// <summary>
	/// ���U���g�V�[���Ŏg�p����X�V����
	/// </summary>
	void UpdateResult();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���U���g�V�[���Ŏg�p����`�揈��
	/// </summary>
	void DrawResult();

	void MapCollision(const Map& map);

	void SnailCollision(std::vector<Snail> snails);

	void EdgeWarp() override;

	void Summon(bool isSummonLeft);

	void Attacked();

	void Dead();

	SnailStatus UpdateStatus();

	bool GetIsFliped() const { return m_isFliped; } // �Ђ�����Ԃ��Ă��邩�ǂ������擾

private:
	Animation m_nowAnim;	// ���݂̃A�j���[�V����
	Animation m_moveAnim;	// �ړ��A�j���[�V����
	Animation m_flipedAnim;	// �Ђ�����Ԃ����A�j���[�V����
	Animation m_deadAnim;	// ���S�A�j���[�V����
	float m_deadRotate;
	SnailStatus m_state;

	int m_outPipeTime;	// �y�ǂ���o�Ă��鎞�Ԃ��J�E���g����ϐ�
	int m_flipedTime; // �Ђ�����Ԃ��Ă��鎞�Ԃ��J�E���g����ϐ�
	bool m_isFliped;	// �X�l�C�����Ђ�����Ԃ��Ă��邩�ǂ���

public:
	bool m_isDelete; // �X�l�C�����폜����邩�ǂ����i���S��Ԃō폜�����ꍇ�Ɏg�p�j
};

