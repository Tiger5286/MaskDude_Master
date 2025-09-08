#include "Snail.h"
#include "Dxlib.h"
#include "Animation.h"

Snail::Snail() :
	Enemy(),
	m_deadRotate(0), // ���S�A�j���[�V�����̉�]�p�x��������
	m_state(SnailStatus::Move), // ������Ԃ��ړ���Ԃɐݒ�
	m_outPipeTime(0),
	m_isFliped(false), // ���]��Ԃ�������
	m_flipedTime(0), // ���]���Ă��鎞�Ԃ�������
	m_isDelete(false) // �폜��Ԃ�������
{
}

Snail::~Snail()
{
}

void Snail::Init()
{
	// �����ʒu��ݒ�
	m_pos = Vec2(100, 100); // �K���ȏ����ʒu
	m_speed = Vec2(0, 0); // �������x
	m_isAlive = true; // ������Ԃ�������
	// �A�j���[�V�����̏�����
	m_moveAnim.Init(m_handle, Vec2(kGraphSnailSizeX, kGraphSnailSizeY), kAnimSnailMaxFrame, kAnimSnailOneAnimationFrame, kAnimSnailScale); // �T�C�Y��t���[�����͓K�X����

	m_flipedAnim.Init(m_handle, Vec2(kGraphSnailSizeX, kGraphSnailSizeY), kAnimSnailMaxFrame, 1, kAnimSnailScale); // ���]�A�j���[�V���������l�ɏ�����
	m_flipedAnim.SetRotate(135); // ���]�A�j���[�V�����̉�]�p�x��ݒ�

	m_deadAnim.Init(m_handle, Vec2(kGraphSnailSizeX, kGraphSnailSizeY), kAnimSnailMaxFrame, 1, kAnimSnailScale); // ���S�A�j���[�V�����̏�����

	m_nowAnim = m_moveAnim; // �����A�j���[�V�������ړ��A�j���[�V�����ɐݒ�
}

void Snail::InitResult()
{
	m_pos = Vec2(GetRand(Game::kScreenWidth), 0);
	m_deadAnim.Init(m_handle, Vec2(kGraphSnailSizeX, kGraphSnailSizeY), kAnimSnailMaxFrame, 1, kAnimSnailScale); // ���S�A�j���[�V�����̏�����
	m_nowAnim = m_deadAnim;
	Dead();
}

void Snail::End()
{
}

void Snail::Update(const Map& map)
{
	m_outPipeTime++; // �y�ǂ���o�Ă��鎞�Ԃ��J�E���g�A�b�v

	// �Ђ�����Ԃ������Ԃ̃J�E���g
	if (m_isFliped)
	{
		m_flipedTime++; // �Ђ�����Ԃ������Ԃ��J�E���g�A�b�v
	}
	else
	{
		m_flipedTime = 0; // �Ђ�����Ԃ��Ă��Ȃ��ꍇ�̓��Z�b�g
	}

	if (m_flipedTime == 420) // �Ђ�����Ԃ��Ă���7�b�o�߂����ꍇ
	{
		// �Ђ�����Ԃ�����Ԃ��猳�ɖ߂�
		Attacked();
	}

	m_nowAnim.Update(); // �A�j���[�V�����̍X�V

	// ���S���̃A�j���[�V��������]������
	if (!m_isAlive) // �X�l�C�������S���Ă���ꍇ
	{
		m_deadRotate += 0.2f;
		if (m_deadRotate > 360)
		{
			m_deadRotate = 0; // ��]�p�x�����Z�b�g
		}
		m_nowAnim.SetRotate(m_deadRotate); // ���S�A�j���[�V�����̉�]�p�x���X�V
	}

	// �y�ǂ���o�Ă���Ԃ͏d�͂�K�p���Ȃ�(�y�ǂ���o�Ă���悤�Ɍ����邽��)
	if (m_outPipeTime > 60)
	{
		Gravity(); // �d�͂�K�p
	}

	// �X�l�C���̈ړ�����
	m_pos.x += m_speed.x; // X���W�𑬓x�ɉ����čX�V
	m_pos.y += m_speed.y; // Y���W�𑬓x�ɉ����čX�V
	if (m_isAlive)
	{
		EdgeWarp(); // ��ʂ̒[�ɍs���Ɣ��Α��Ƀ��[�v���鏈��
		MapCollision(map); // �}�b�v�Ƃ̓����蔻��
	}
	else
	{
		if (m_pos.y > Game::kScreenHeight + 100) // �X�l�C������ʂ̉��ɗ������ꍇ
		{
			m_isDelete = true;
		}
	}
}

void Snail::UpdateResult()
{
	m_nowAnim.Update(); // �A�j���[�V�����̍X�V

	// ���S���̃A�j���[�V��������]������
	if (!m_isAlive) // �X�l�C�������S���Ă���ꍇ
	{
		m_deadRotate += 0.2f;
		if (m_deadRotate > 360)
		{
			m_deadRotate = 0; // ��]�p�x�����Z�b�g
		}
		m_nowAnim.SetRotate(m_deadRotate); // ���S�A�j���[�V�����̉�]�p�x���X�V
	}

	Gravity(); // �d�͂�K�p

	// �X�l�C���̈ړ�����
	m_pos.x += m_speed.x; // X���W�𑬓x�ɉ����čX�V
	m_pos.y += m_speed.y; // Y���W�𑬓x�ɉ����čX�V

	if (m_pos.y > Game::kScreenHeight)
	{
		m_pos = Vec2(GetRand(Game::kScreenWidth), 0);
	}
}

void Snail::Draw()
{
	if (m_isFliped && (m_flipedTime > 300)) // �N���オ�钼�O�k��������
	{
		if ((m_outPipeTime % 8) < 4)
		{
			m_nowAnim.Draw(Vec2(m_pos.x + 2, m_pos.y + 5), m_isTurn);
		}
		else
		{
			m_nowAnim.Draw(Vec2(m_pos.x - 2, m_pos.y + 5), m_isTurn);
		}
	}
	else if (m_isFliped)
	{
		m_nowAnim.Draw(Vec2(m_pos.x,m_pos.y+5), m_isTurn);
		m_nowAnim.Draw(Vec2(m_pos.x + Game::kScreenWidth, m_pos.y+5), m_isTurn); // ��ʂ̉E���ɂ��`��
		m_nowAnim.Draw(Vec2(m_pos.x - Game::kScreenWidth, m_pos.y+5), m_isTurn); // ��ʂ̍����ɂ��`��
	}
	else
	{
		m_nowAnim.Draw(m_pos, m_isTurn);
		m_nowAnim.Draw(Vec2(m_pos.x + Game::kScreenWidth, m_pos.y), m_isTurn); // ��ʂ̉E���ɂ��`��
		m_nowAnim.Draw(Vec2(m_pos.x - Game::kScreenWidth, m_pos.y), m_isTurn); // ��ʂ̍����ɂ��`��
	}

#ifdef _DEBUG
	// �����蔻��̕\���i�f�o�b�O�p�j
	DrawBox(m_pos.x - (kSnailHitBoxWidth / 2), m_pos.y - (kSnailHitBoxHeight / 2),
		m_pos.x + (kSnailHitBoxWidth / 2), m_pos.y + (kSnailHitBoxHeight / 2),
		GetColor(255, 0, 0), false); // �����蔻��̕\��
#endif // _DEBUG
}

void Snail::DrawResult()
{
	m_nowAnim.Draw(m_pos,true);
}

void Snail::MapCollision(const Map& map)
{
	Vec2 SnailSize = Vec2(kSnailHitBoxWidth, kSnailHitBoxHeight);
	Vec2 hitBlockPos;

	if (map.CheckCollision(m_pos, SnailSize, hitBlockPos))
	{
		// �����蔻�肪�������ꍇ�̏���
		Vec2 dist;
		dist.x = m_pos.x - hitBlockPos.x;
		dist.y = m_pos.y - hitBlockPos.y;

		// X������Y�����̂ǂ���̋������傫�����Ŕ���
		if (abs(dist.x) > abs(dist.y))
		{
			// �����瓖�������ꍇ�i���E�̕ǁj
			if (dist.x > 0)
			{
				// �v���C���[���u���b�N�̉E���ɂ���i���̕ǂɓ��������j
				m_pos = (Vec2(hitBlockPos.x + kChipSize / 2 + SnailSize.x / 2, m_pos.y));
			}
			else
			{
				// �v���C���[���u���b�N�̍����ɂ���i�E�̕ǂɓ��������j
				m_pos = (Vec2(hitBlockPos.x - kChipSize / 2 - SnailSize.x / 2, m_pos.y));
			}
		}
		else
		{
			// �c���瓖�������ꍇ�i�㉺�j
			if (dist.y > 0)
			{
				// �v���C���[���u���b�N�̉����ɂ���i�ォ�瓖�������j
				if (m_speed.y < 0)	// �W�����v���Ȃ�(y�̑��x�����x���}�C�i�X�Ȃ�)
				{
					m_pos = (Vec2(m_pos.x, hitBlockPos.y + kChipSize / 2 + SnailSize.y / 2));
					m_speed.y = 0;
				}
			}
			else
			{
				// �v���C���[���u���b�N�̏㑤�ɂ���i�����瓖�������E���n�j
				if (m_speed.y > 0)	// �������Ȃ�(y�̑��x���v���X�Ȃ�)
				{
					m_pos = (Vec2(m_pos.x, hitBlockPos.y - kChipSize / 2 - SnailSize.y / 2));
					m_speed.y = 0;
					m_isOnFloor = true;  // �n�ʂɐڒn
					if (m_isFliped)
					{
						m_speed.x = 0; // �Ђ�����Ԃ�����ԂŒ��n�����ꍇ�AX�����̑��x��0�ɂ���
					}
				}
			}
		}
	}
}




void Snail::SnailCollision(std::vector<Snail> snails)
{
	for (auto& snail : snails)
	{
		// �X�l�C�����m�̓����蔻����s��
		float distX = m_pos.x - snail.m_pos.x;
		float distY = m_pos.y - snail.m_pos.y;
		bool isCollided = abs(distX) < kSnailHitBoxWidth && abs(distY) < kSnailHitBoxHeight;
		if (distX == 0 && distY == 0)
		{
			isCollided = false; // �����ʒu�ɂ���ꍇ�͏Փ˂��Ȃ�
		}
		// �����̃X�l�C�����������Ă��邩�ǂ������`�F�b�N
		bool isAlive = m_isAlive && snail.m_isAlive;
		// �X�l�C�����m���Փ˂��Ă��āA�����Ƃ��������Ă���ꍇ
		if (isAlive && isCollided)
		{
			if (!m_isFliped)
			{
				m_isTurn = !m_isTurn; // �����𔽓]
				m_speed.x = -m_speed.x; // ���x�𔽓]
			}
			if (abs(distX) < kSnailHitBoxWidth - 5)
			{
				if (distX < 0)
				{
					// �������E��
					while (abs(distX) < kSnailHitBoxWidth)
					{
						m_pos.x += 1; // �������E�Ɉړ�
						distX = m_pos.x - snail.m_pos.x; // �Čv�Z
					}
				}
				else
				{
					// ����������
					while (abs(distX) < kSnailHitBoxWidth)
					{
						m_pos.x -= 1; // ���������Ɉړ�
						distX = m_pos.x - snail.m_pos.x; // �Čv�Z
					}
				}
			}
		}
	}
}

void Snail::EdgeWarp()
{
	if (m_pos.y > 500)	// ��ԉ��̒i�ɂ���
	{
		bool isEnterPipe = m_pos.x < 0 || m_pos.x > Game::kScreenWidth; // ���E�̒[�ɂ��邩�ǂ������`�F�b�N
		if (isEnterPipe)
		{
			m_isTurn = !m_isTurn; // �����𔽓]
			m_speed.x = -m_speed.x; // ���x�𔽓]
			m_speed.y = 0; // Y�����̑��x�����Z�b�g
			m_pos.y = kSnailSummonPosY; // Y���W�������ʒu�ɐݒ�
			m_outPipeTime = 0; // �y�ǂ���o�Ă��鎞�Ԃ����Z�b�g
			if (m_isTurn)
			{
				m_pos.x = kSnailSummonPosLeftX; // ���[�Ɉʒu��ݒ�
			}
			else
			{
				m_pos.x = kSnailSummonPosRightX; // �E�[�Ɉʒu��ݒ�
			}
		}
	}
	else
	{
		// ���E�̒[�ɍs���Ɣ��Α��Ƀ��[�v
		if (m_pos.x < 0)	// ��ʂ̍��[�𒴂����ꍇ
		{
			m_pos.x = Game::kScreenWidth;	// ��ʂ̉E�[�Ƀ��[�v
		}
		if (m_pos.x > Game::kScreenWidth) // ��ʂ̉E�[�𒴂����ꍇ
		{
			m_pos.x = 0; // ��ʂ̍��[�Ƀ��[�v
		}
	}
}

void Snail::Summon(bool isSummonLeft)
{
	if (isSummonLeft)
	{
		m_outPipeTime = 0; // �y�ǂ���o�Ă��鎞�Ԃ����Z�b�g
		m_pos.x = kSnailSummonPosLeftX; // ���[�Ɉʒu��ݒ�
		m_pos.y = kSnailSummonPosY; // Y���W��ݒ�
		m_speed.x = kSnailMoveSpeed; // �E�����Ɉړ����鑬�x��ݒ�
		m_isTurn = true; // �������E�ɐݒ�
		m_isAlive = true; // ������Ԃ�ݒ�
	}
	else
	{
		m_outPipeTime = 0; // �y�ǂ���o�Ă��鎞�Ԃ����Z�b�g
		m_pos.x = kSnailSummonPosRightX; // �E�[�Ɉʒu��ݒ�
		m_pos.y = kSnailSummonPosY; // Y���W��ݒ�
		m_speed.x = -kSnailMoveSpeed; // �������Ɉړ����鑬�x��ݒ�
		m_isTurn = false; // ���������ɐݒ�
		m_isAlive = true; // ������Ԃ�ݒ�
	}
}

void Snail::Attacked()
{
	if (m_isOnFloor)
	{
		if (m_isFliped)
		{
			m_isFliped = false; // �X�l�C�����Ђ�����Ԃ��Ă��Ȃ���Ԃɂ���
			if (m_isTurn)
			{
				m_speed.x = kSnailMoveSpeed; // �������Ɉړ����鑬�x��ݒ�
			}
			else
			{
				m_speed.x = -kSnailMoveSpeed; // �E�����Ɉړ����鑬�x��ݒ�
			}
		}
		else
		{
			m_isFliped = true; // �X�l�C�����Ђ�����Ԃ�����Ԃɂ���
		}
		m_speed.y = -5; // ������ɒ��˕Ԃ�
		m_isOnFloor = false; // �n�ʂ��痣���
		UpdateStatus();
	}
}

void Snail::Dead()
{
	m_isAlive = false; // �X�l�C�������S��Ԃɂ���
	m_speed.y = -7; // ������ɒ��˕Ԃ�
	if (m_isTurn)
	{
		m_speed.x = -kSnailMoveSpeed; // �������Ɉړ����鑬�x��ݒ�
	}
	else
	{
		m_speed.x = kSnailMoveSpeed; // �E�����Ɉړ����鑬�x��ݒ�
	}
	UpdateStatus(); // ��Ԃ��X�V
}

SnailStatus Snail::UpdateStatus()
{
	SnailStatus status;

	if (m_isAlive)
	{
		if (m_isFliped)
		{
			status = SnailStatus::Fliped; // �Ђ�����Ԃ������
			m_nowAnim = m_flipedAnim; // ���]�A�j���[�V������ݒ�
		}
		else
		{
			status = SnailStatus::Move; // �ړ����
			m_nowAnim = m_moveAnim; // �ړ��A�j���[�V������ݒ�
		}
	}
	else
	{
		status = SnailStatus::Dead; // ���S���
		m_nowAnim = m_deadAnim; // ���S�A�j���[�V������ݒ�
	}

	return status;
}
