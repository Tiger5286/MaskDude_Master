#pragma once
#include "Vec2.h"
#include "Character.h"
#include "Animation.h"
#include "Map.h"
#include "Snail.h"
#include <vector>

// �萔��`
namespace
{
	// �v���C���[�̃T�C�Y
	constexpr int kGraphPlayerSize = 32;
	constexpr int kDrawPlayerSize = kGraphPlayerSize * 2;	// 64

	// �����蔻��
	constexpr int kHitBoxWidth = 44;
	constexpr int kHitBoxHeight = 64;
	// �U����������Y���̋���
	constexpr int kAttackHitBoxHeight = kHitBoxHeight / 2 + kChipSize + kSnailHitBoxHeight / 2;	// 64 / 2 + 32 + 48 / 2 = 64 + 16 = 80
	constexpr int kAttackHitBoxWidth = kHitBoxWidth / 2 + kSnailHitBoxWidth / 2;	// 44 / 2 + 48 / 2 = 22 + 24 = 46

	// ���������߂̏��
	constexpr float kMaxMoveSpeed = 4;	// �ō��ړ����x
	constexpr float kMoveSpeed = 0.3f;	// ���E�ړ��̑��x
	constexpr float kJumpPower = -14;		// �W�����v��
	//constexpr float kGravity = 0.5f;	// �d��
	//constexpr float kMaxFallSpeed = 15.0f;	// �ō��������x

	const Vec2 kFirstPos(128, 640);

	// �A�j���[�V�����֘A
	constexpr int kAnimIdleMaxFrame = 11;	// �ҋ@�A�j���[�V�����̃t���[����
	constexpr int kAnimRunMaxFrame = 12;	// ���s�A�j���[�V�����̃t���[����
	constexpr int kAnimJumpMaxFrame = 1;	// �W�����v�A�j���[�V�����̃t���[����
	constexpr int kAnimFallMaxFrame = 1;	// �����A�j���[�V�����̃t���[����
	constexpr int kAnimDeathMaxFrame = 7; // ���S�A�j���[�V�����̃t���[����
	constexpr int kAnimSpawnMaxFrame = 7; // �X�|�[���A�j���[�V�����̃t���[����

	constexpr int kGraphDeathSize = 96; // ���S�A�j���[�V�����̃O���t�B�b�N�T�C�Y
	constexpr int kGraphSpawnSize = 96; // �X�|�[���A�j���[�V�����̃O���t�B�b�N�T�C�Y

	constexpr int kAnimOneAnimationFrame = 3;	// �A�j���[�V������1�t���[���̎���
	constexpr int kAnimScale = 2;	// �A�j���[�V�����̊g�嗦
}

enum PlayerState
{
	None,	// �Ȃ�(�\�����Ȃ�)
	Idle,	// �ҋ@
	Run,	// ���s
	Jump,	// �W�����v
	Fall,	// ����
	Hit,	// ��_���[�W
	Death,	// ���S
	Spawn,	// �X�|�[��
};

/// <summary>
/// �v���C���[�N���X
/// </summary>
class Player : public Character	// Character�N���X���p��
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Player();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Player();

	/// <summary>
	/// ������
	/// </summary>
	void Init() override;

	/// <summary>
	/// �I������
	/// </summary>
	void End() override;

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	void Control();	// ���쏈��

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw() override;

	/// <summary>
/// �}�b�v�Ƃ̓����蔻����s��
/// </summary>
/// <param name="map">�}�b�v</param>
	void MapCollision(const Map& map,std::vector<Snail*>& pSnails);

	/// <summary>
	/// �v���C���[�̏�Ԃ��X�V����(�A�j���[�V�����p)
	/// </summary>
	/// <returns>�v���C���[�̏��</returns>
	PlayerState UpdateState();

	/// <summary>
	/// �O���t�B�b�N�n���h����K�p
	/// </summary>
	/// <param name="idle">�ҋ@��Ԃ̃O���t�B�b�N�n���h��</param>
	/// <param name="run">���s��Ԃ̃O���t�B�b�N�n���h��</param>
	/// <param name="jump">�W�����v���̃O���t�B�b�N�n���h��</param>
	/// <param name="fall">�������̃O���t�B�b�N�n���h��</param>
	/// <param name="hit">��_���[�W���̃O���t�B�b�N�n���h��</param>
	void SetHandle(int idle, int run, int jump, int fall, int hit, int death, int spawn)
	{
		m_idle = idle;
		m_run = run;
		m_jump = jump;
		m_fall = fall;
		m_hit = hit;
		m_death = death;
		m_spawn = spawn;
	}

	void SetSEHandle(int jump, int death, int respawn, int snailAttacked, int snailDefeated)
	{
		m_jumpSE = jump;
		m_deathSE = death;
		m_respawnSE = respawn;
		m_snailAttackedSE = snailAttacked;
		m_snailDefeatedSE = snailDefeated;
	}

	void SetCanControl(bool isCan) { m_isCanControl = isCan; } // ����\���ǂ����Z�b�g

	void CheckHitSnail(Snail& snail);	// �X�l�C���ɐG�ꂽ���ǂ���

private:
	void CheckAttackSnail(Snail& snail);	// �X�l�C���Ƃ̓����蔻��

	// �O���t�B�b�N�n���h��
	int m_idle;
	int m_run;
	int m_jump;
	int m_fall;
	int m_hit;
	int m_death;
	int m_spawn;

	// SE�n���h��
	int m_jumpSE;	// �W�����vSE
	int m_deathSE;	// ���SSE
	int m_respawnSE;	// ���X�|�[��SE
	int m_snailAttackedSE;	// �X�l�C���U��SE
	int m_snailDefeatedSE;	// �X�l�C�����jSE

	// �A�j���[�V����
	Animation m_nowAnim;	// ���݂̃A�j���[�V����

	Animation m_noneAnim;	// �Ȃ�(�\�����Ȃ�)�̃A�j���[�V����
	Animation m_idleAnim;	// �ҋ@�̃A�j���[�V����
	Animation m_runAnim;	// ���s�̃A�j���[�V����
	Animation m_jumpAnim;	// �W�����v�̃A�j���[�V����
	Animation m_fallAnim;	// �����̃A�j���[�V����
	Animation m_hitAnim;	// ��_���[�W�̃A�j���[�V����
	Animation m_deathAnim;	// ���S�̃A�j���[�V����
	Animation m_spawnAnim;	// �X�|�[���̃A�j���[�V����

	PlayerState m_state;	// �v���C���[�̏��

	bool m_isInvincible;	// ���G��Ԃ��ǂ���
	int m_invincibleTime;	// ���G��Ԃ̎���
	int m_deathTime;	//���S��̎���
	bool m_isCanControl; // ����\���ǂ���
};

