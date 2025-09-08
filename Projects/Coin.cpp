#include "Coin.h"
#include "Dxlib.h"

Coin::Coin():
	Character(),
	m_handle(-1),
	m_outPipeTime(0), // �y�ǂ���o�Ă��鎞�Ԃ�������
	m_isDelete(false)
{
}

Coin::~Coin()
{
}

void Coin::Init()
{
	m_nowAnim.Init(m_handle, Vec2(kCoinSize, kCoinSize), 1, 1, 2); // �A�j���[�V�����̏�����
	m_speed.x = kCoinMoveSpeed; // �R�C���̈ړ����x��ݒ�
}

void Coin::End()
{

}

void Coin::Update(const Map& map)
{
	m_outPipeTime++; // �y�ǂ���o�Ă��鎞�Ԃ��J�E���g�A�b�v

	//m_state = UpdateStatus(); // �X�l�C���̏�Ԃ��X�V
	m_nowAnim.Update(); // �A�j���[�V�����̍X�V

	// �y�ǂ���o�Ă���Ԃ͏d�͂�K�p���Ȃ�(�y�ǂ���o�Ă���悤�Ɍ����邽��)
	if (m_outPipeTime > 35)
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
}

void Coin::Draw()
{
	m_nowAnim.Draw(m_pos, m_isTurn);
	m_nowAnim.Draw(Vec2(m_pos.x + Game::kScreenWidth, m_pos.y), m_isTurn); // ��ʂ̉E���ɂ��`��
	m_nowAnim.Draw(Vec2(m_pos.x - Game::kScreenWidth, m_pos.y), m_isTurn); // ��ʂ̍����ɂ��`��
#ifdef _DEBUG
	// �����蔻��̕\���i�f�o�b�O�p�j
	DrawBox(m_pos.x - (kCoinSize / 2), m_pos.y - (kCoinSize / 2),
		m_pos.x + (kCoinSize / 2), m_pos.y + (kCoinSize / 2),
		GetColor(255, 0, 0), false); // �����蔻��̕\��
#endif // _DEBUG
}

void Coin::MapCollision(const Map& map)
{
	Vec2 coinSize = Vec2(kCoinSize, kCoinSize);
	Vec2 hitBlockPos;

	if (map.CheckCollision(m_pos, coinSize, hitBlockPos))
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
				m_pos = (Vec2(hitBlockPos.x + kChipSize / 2 + coinSize.x / 2, m_pos.y));
			}
			else
			{
				// �v���C���[���u���b�N�̍����ɂ���i�E�̕ǂɓ��������j
				m_pos = (Vec2(hitBlockPos.x - kChipSize / 2 - coinSize.x / 2, m_pos.y));
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
					m_pos = (Vec2(m_pos.x, hitBlockPos.y + kChipSize / 2 + coinSize.y / 2));
					m_speed.y = 0;
				}
			}
			else
			{
				// �v���C���[���u���b�N�̏㑤�ɂ���i�����瓖�������E���n�j
				if (m_speed.y > 0)	// �������Ȃ�(y�̑��x���v���X�Ȃ�)
				{
					m_pos = (Vec2(m_pos.x, hitBlockPos.y - kChipSize / 2 - coinSize.y / 2));
					m_speed.y = 0;
					m_isOnFloor = true;  // �n�ʂɐڒn
				}
			}
		}
	}
}

void Coin::PlayerCollision(const Player& player)
{
	// �v���C���[�Ƃ̓����蔻����s��
	Vec2 playerPos = player.GetPos();
	float distX = m_pos.x - playerPos.x;
	float distY = m_pos.y - playerPos.y;
	bool isCollided = abs(distX) < kCoinSize && abs(distY) < kCoinSize;
	if (distX == 0 && distY == 0)
	{
		isCollided = false; // �����ʒu�ɂ���ꍇ�͏Փ˂��Ȃ�
	}
	// �R�C�����������Ă��āA�v���C���[�ƏՓ˂��Ă���ꍇ
	if (m_isAlive && isCollided)
	{
		m_isDelete = true; // �R�C�����폜����t���O�𗧂Ă�
	}
}

void Coin::EdgeWarp()
{
	if (m_pos.y > 500)	// ��ԉ��̒i�ɂ���
	{
		bool isEnterPipe = m_pos.x < 0 || m_pos.x > Game::kScreenWidth; // ���E�̒[�ɂ��邩�ǂ������`�F�b�N
		if (isEnterPipe)
		{
			m_isTurn = !m_isTurn; // �����𔽓]
			m_speed.x = -m_speed.x; // ���x�𔽓]
			m_speed.y = 0; // Y�����̑��x�����Z�b�g
			m_pos.y = kCoinSummonY; // Y���W�������ʒu�ɐݒ�
			m_outPipeTime = 0; // �y�ǂ���o�Ă��鎞�Ԃ����Z�b�g
			if (m_isTurn)
			{
				m_pos.x = kCoinSummonLeftX; // ���[�Ɉʒu��ݒ�
			}
			else
			{
				m_pos.x = kCoinSummonRightX; // �E�[�Ɉʒu��ݒ�
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

void Coin::Summon(bool isSummonLeft)
{
	if (isSummonLeft)
	{
		m_outPipeTime = 0; // �y�ǂ���o�Ă��鎞�Ԃ����Z�b�g
		m_pos.x = kCoinSummonLeftX; // ���[�Ɉʒu��ݒ�
		m_pos.y = kCoinSummonY; // Y���W��ݒ�
		m_speed.x = kCoinMoveSpeed; // �E�����Ɉړ����鑬�x��ݒ�
		m_isTurn = true; // �������E�ɐݒ�
		m_isAlive = true; // ������Ԃ�ݒ�
	}
	else
	{
		m_outPipeTime = 0; // �y�ǂ���o�Ă��鎞�Ԃ����Z�b�g
		m_pos.x = kCoinSummonRightX; // �E�[�Ɉʒu��ݒ�
		m_pos.y = kCoinSummonY; // Y���W��ݒ�
		m_speed.x = -kCoinMoveSpeed; // �������Ɉړ����鑬�x��ݒ�
		m_isTurn = false; // ���������ɐݒ�
		m_isAlive = true; // ������Ԃ�ݒ�
	}
}